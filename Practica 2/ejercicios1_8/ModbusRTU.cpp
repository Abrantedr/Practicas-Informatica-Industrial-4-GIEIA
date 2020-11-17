//
// Created by abran on 17/11/2020.
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

}


Mensaje ModbusRTU::peticion(Mensaje& recibido) {
  Mensaje respuesta;

  std::cerr << "Recibido mensaje " << recibido << std::endl;

  uint8_t id = recibido.getByteAt(0);
  uint8_t funcion = recibido.getByteAt(1);

  std::cerr << "El mensaje es para " << (int)id << " y la funcion es "
      << (int)funcion << std::endl;

  switch (funcion) {
    case 1:
      respuesta = atiende01(recibido);
      break;
    case 3:
      respuesta = atiende03(recibido);
      break;
    case 5:
      respuesta = atiende05(recibido);
      break;
    case 15:
      respuesta = atiende15(recibido);
      break;
    default:
      std::cerr << "Funcion aun no implementada" << std::endl;
  }
  return respuesta;
}


Mensaje ModbusRTU::atiende03(Mensaje& recibido) {
  Mensaje respuesta;

  std::cerr << "Entramos en metodo atiende03 con mensaje " << recibido
      << std::endl;

  respuesta.pushByte_back(recibido.getByteAt(0)); // Dirección
  respuesta.pushByte_back(recibido.getByteAt(1)); // Función

  uint16_t offset = recibido.getWordAt(2);
  uint16_t numPos = recibido.getWordAt(4);

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

  respuesta.pushByte_back(recibido.getByteAt(0)); // Dirección
  respuesta.pushByte_back(recibido.getByteAt(1)); // Función

  uint16_t offset = recibido.getWordAt(2);
  uint16_t numPos = recibido.getWordAt(4);

  unsigned numBytes = ((numPos - 1) / 8) + 1;
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


Mensaje ModbusRTU::atiende05(Mensaje &recibido) {
  Mensaje respuesta;

  std::cerr << "Entramos en metodo atiende05 con mensaje " << recibido
      << std::endl;

  uint16_t offset = recibido.getWordAt(2);

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

  // TODO: comprobar que el numero de posiciones y el numero de datos
  //  concuerdan antes de realizar la escritura

  std::cerr << "Entramos en metodo atiende15 con mensaje " << recibido
      << std::endl;

  respuesta.pushByte_back(recibido.getByteAt(0)); // Dirección
  respuesta.pushByte_back(recibido.getByteAt(1)); // Función

  uint16_t offset = recibido.getWordAt(2);
  uint16_t numPos = recibido.getWordAt(4);

  unsigned k = 7; // Índice a partir del cual obtenemos los datos
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

  respuesta.pushWord_back(offset); // Offset
  respuesta.pushWord_back(numPos); // Número de bytes

  respuesta.aniadeCRC(); // CRC

  std::cerr << "Respuesta: " << respuesta << std::endl;
  return respuesta;
}