//
// Created by Rubén Abrante Delgado on 17/11/2020.
//

#include <iostream>

#include "ModbusRTU.hpp"


ModbusRTU::ModbusRTU(uint8_t id) : _id(id), _DO(20), _AO(10) {
  // Las primeros 20 salidas digitales con un valor inicial de 0 en las
  // posiciones pares y de 1 las posiciones impares.
  for (std::size_t i = 0; i < _DO.size(); i += 2) {
    _DO.at(i) = true;
  }

  // Los primeros 10 registros analógicos de salida, con un valor
  // inicial de 0, 4, 8, 12, ....
  for (std::size_t i = 0; i < _AO.size(); ++i) {
    _AO.at(i) = i * 4;
  }

  std::cerr << "Se ha creado un ModbusRTU con ID: " << (int)_id
      << std::endl;

}


Mensaje ModbusRTU::peticion(Mensaje& recibido) {
  Mensaje respuesta;

  std::cerr << "Recibido mensaje " << recibido << std::endl;

  uint8_t id = recibido.getByteAt(0);
  uint8_t funcion = recibido.getByteAt(1);

  std::cerr << "El mensaje es para " << (int)id << " y la funcion es "
      << (int)funcion << std::endl;

  if (id != _id || !recibido.crcOK()) {
    std::cerr << "ID invalida o CRC incorrecto." << std::endl;
    std::cerr << "Respuesta: " << respuesta << std::endl;
    return respuesta;
  }

  switch (funcion) {
    case 1: // Lectura de salidas digitales
      respuesta = atiende01(recibido);
      break;
    case 3: // Lectura de salidas analógicas
      respuesta = atiende03(recibido);
      break;
    case 5: // Fuerza una única salida digital
      respuesta = atiende05(recibido);
      break;
    case 6: // Fuerza una única salida analógica
      respuesta = atiende06(recibido);
      break;
    case 15: // Fuerza múltiples salidas digitales
      respuesta = atiende15(recibido);
      break;
    case 16: // Fuerza múltiples salidas analógicas
      respuesta = atiende16(recibido);
      break;
    default:
      std::cerr << "Funcion no implementada" << std::endl;
      respuesta = generaError(recibido, 0x01);
  }
  return respuesta;
}


template <class T> bool ModbusRTU::dentroDeRango(const std::vector<T>& registro,
    uint16_t offset, uint16_t numPos) const {
  return (offset >= 0) && (offset < registro.size())
      && ((offset + numPos) >= offset)
      && ((offset + numPos) <= registro.size());
}


Mensaje ModbusRTU::generaError(Mensaje& recibido, uint8_t errorCode) {
  Mensaje respuesta;

  std::cerr << "Generando mensaje de error " << (int)errorCode
      << std::endl;
  respuesta.pushByte_back(recibido.getByteAt(0)); // Direcc.
  respuesta.pushByte_back(recibido.getByteAt(1) | (1 << 7)); // Funcion
  respuesta.pushByte_back(errorCode);
  respuesta.aniadeCRC();

  std::cerr << "Mensaje de error: " << respuesta << std::endl;
  return respuesta;
}


Mensaje ModbusRTU::atiende03(Mensaje& recibido) {
  Mensaje respuesta;

  std::cerr << "Entramos en metodo atiende03 con mensaje " << recibido
      << std::endl;

  uint16_t offset = recibido.getWordAt(2);
  uint16_t numPos = recibido.getWordAt(4);

  if (!dentroDeRango<uint16_t>(_AO, offset, numPos))
    return generaError(recibido, 0x02);

  respuesta.pushByte_back(recibido.getByteAt(0)); // Dirección
  respuesta.pushByte_back(recibido.getByteAt(1)); // Función
  respuesta.pushByte_back(numPos * 2); // Número de bytes

  for (std::size_t i = offset; i < (offset + numPos); ++i) { // Datos
    std::cerr << "Accediendo AO[" << i << "]: " << _AO.at(i)
        << std::endl;
    respuesta.pushWord_back(_AO.at(i));
  }

  respuesta.aniadeCRC(); // CRC

  std::cerr << "Respuesta: " << respuesta << std::endl;
  return respuesta;
}


Mensaje ModbusRTU::atiende01(Mensaje& recibido) {
  Mensaje respuesta;

  std::cerr << "Entramos en metodo atiende01 con mensaje " << recibido
      << std::endl;

  uint16_t offset = recibido.getWordAt(2);
  uint16_t numPos = recibido.getWordAt(4);

  if (!dentroDeRango<bool>(_DO, offset, numPos))
    return generaError(recibido, 0x02);

  unsigned numBytes = ((numPos - 1) / 8) + 1;
  respuesta.pushByte_back(recibido.getByteAt(0)); // Dirección
  respuesta.pushByte_back(recibido.getByteAt(1)); // Función
  respuesta.pushByte_back(numBytes); // Número de bytes

  unsigned posBit = 0;
  uint8_t dato = 0;
  for (std::size_t i = offset; i < (offset + numPos); ++i) {
    std::cerr << "Accediendo DO[" << i << "]: " << _DO.at(i)
        << std::endl;
    if (_DO.at(i))
      dato |= 1 << posBit;
    ++posBit;
    if (posBit == 8) { // Hemos rellenado un byte
      respuesta.pushByte_back(dato);
      posBit = 0;
      dato = 0;
    }
  }

  if (posBit > 0) { // Queda un byte a medio rellenar
    respuesta.pushByte_back(dato);
  }

  respuesta.aniadeCRC(); // CRC

  std::cerr << "Respuesta: " << respuesta << std::endl;
  return respuesta;
}


Mensaje ModbusRTU::atiende05(Mensaje& recibido) {

  std::cerr << "Entramos en metodo atiende05 con mensaje " << recibido
      << std::endl;

  uint16_t offset = recibido.getWordAt(2);

  if (!dentroDeRango<bool>(_DO, offset))
    return generaError(recibido, 0x02);

  if (
      (recibido.getByteAt(4) == 0xFF) &&
      (recibido.getByteAt(5) == 0x00)) {
    _DO.at(offset) = true;
    std::cerr << "Ponemos a 1 la DO[" << offset << "]: "
        << _DO.at(offset) << std::endl;
  } else if (
      (recibido.getByteAt(4) == 0x00) &&
      (recibido.getByteAt(5) == 0x00)) {
    _DO.at(offset) = false;
    std::cerr << "Ponemos a 0 la DO[" << offset << "]: "
        << _DO.at(offset) << std::endl;
  }
  return recibido;
}


Mensaje ModbusRTU::atiende15(Mensaje& recibido) {
  Mensaje respuesta;

  std::cerr << "Entramos en metodo atiende15 con mensaje " << recibido
      << std::endl;

  uint16_t offset = recibido.getWordAt(2);
  uint16_t numPos = recibido.getWordAt(4);

  if (!dentroDeRango<bool>(_DO, offset, numPos))
    return generaError(recibido, 0x02);

  unsigned k = 7; // A partir de aquí solo existen datos y el CRC
  uint8_t dato = recibido.getByteAt(k);
  unsigned posBit = 0;
  for (std::size_t i = offset; i < (offset + numPos); ++i) {
    std::cerr << "Accediendo DO[" << i << "]: " << _DO.at(i)
        << std::endl;
    _DO.at(i) = dato & (1 << posBit);
    ++posBit;
    if (posBit == 8) { // Hemos rellenado un byte
      posBit = 0;
      k++; // Pasamos al siguiente dato
      dato = recibido.getByteAt(k);
    }
  }

  respuesta.pushByte_back(recibido.getByteAt(0)); // Dirección
  respuesta.pushByte_back(recibido.getByteAt(1)); // Función
  respuesta.pushWord_back(offset); // Offset
  respuesta.pushWord_back(numPos); // Número de bytes
  respuesta.aniadeCRC(); // CRC

  std::cerr << "Respuesta: " << respuesta << std::endl;
  return respuesta;
}


Mensaje ModbusRTU::atiende06(Mensaje& recibido) {

  std::cerr << "Entramos en metodo atiende06 con mensaje " << recibido
            << std::endl;

  uint16_t offset = recibido.getWordAt(2);
  uint16_t valor = recibido.getWordAt(4);

  if (!dentroDeRango<uint16_t>(_AO, offset))
    return generaError(recibido, 0x02);

  std::cerr << "Ponemos a " << (int)valor << " la AO[" << offset
      << "]: " << _AO.at(offset) << std::endl;
  _AO.at(offset) = valor;

  return recibido;
}


Mensaje ModbusRTU::atiende16(Mensaje& recibido) {
  Mensaje respuesta;

  std::cerr << "Entramos en metodo atiende16 con mensaje " << recibido
      << std::endl;

  uint16_t offset = recibido.getWordAt(2);
  uint16_t numPos = recibido.getWordAt(4);

  if (!dentroDeRango<uint16_t>(_AO, offset, numPos))
    return generaError(recibido, 0x02);

  unsigned k = 7; // A partir de aquí solo existen datos y el CRC
  uint16_t dato = recibido.getWordAt(k);
  for (std::size_t i = offset; i < (offset + numPos); ++i) {
    _AO.at(i) = dato;
    k += 2; // Se salta una palabra
    dato = recibido.getWordAt(k);
  }

  respuesta.pushByte_back(recibido.getByteAt(0)); // Dirección
  respuesta.pushByte_back(recibido.getByteAt(1)); // Función
  respuesta.pushWord_back(offset); // Offset
  respuesta.pushWord_back(numPos); // Número de bytes
  respuesta.aniadeCRC(); // CRC

  std::cerr << "Respuesta: " << respuesta << std::endl;
  return respuesta;
}