// Autor: Rubén Abrante Delgado. Fecha: 02/12/2020

#include <iostream>
#include "ModbusRTU.hpp"

// Necesario para desplazar el registro de salidas analógicas
#define INICIO_AO (40100)
#define FIN_AO (40120)
#define DESPLAZA_AO (INICIO_AO - 40001)

// Errores
#define FUNCION_INVALIDA (1)
#define DIRECCION_INVALIDA (2)
#define DATO_INVALIDO (3)

ModbusRTU::ModbusRTU(uint8_t id) : _id(id), _DO(20), _AO(FIN_AO - INICIO_AO + 1)
{
  // Inicializamos las primeras 20 salidas digitales: 0 para las 10 primeras
  // 1 para las restantes
  for (std::size_t i = 10; i < _DO.size(); i++) {
    _DO.at(i) = true;
  }
  
  // Inicializamos los registros analógicos de salida con valores 1, 3, 5, 7...
  unsigned k = 1;
  for (std::size_t i = 0; i < _AO.size(); i++) {
    _AO.at(i) = k;
    k += 2;
  }
}

// Recibe como parámetro un mensaje de petición Modbus/RTU y devuelve la
// respuesta correspondiente
Mensaje ModbusRTU::peticion(Mensaje recibido) {
  Mensaje respuesta;
  std::cerr << "Recibiendo mensaje " << recibido << std::endl;
  
  uint8_t id, funcion;
  // Si el mensaje recibido tiene al menos una id y una función
  if (recibido.size() >= 2) { 
    id = recibido.getId();
    funcion = recibido.getFuncion();
  }
  
  std::cerr << "El mensaje es para " << (int)id << " y la función es "
      << (int)funcion << std::endl;
      
  if (_id != id || !recibido.crcOK())
    return respuesta; // Devolvemos un mensaje vacío
  
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
    case 6:
      respuesta = atiende06(recibido);
      break;
    case 16:
      respuesta = atiende16(recibido);
      break;
    default:
      std::cerr << "Función no implementada" << std::endl;
      return generaError(recibido, FUNCION_INVALIDA);
  }
  std::cerr << "Respuesta " << respuesta << std::endl;
  return respuesta;
}

Mensaje ModbusRTU::atiende01(Mensaje recibido) {
  Mensaje respuesta;
  std::cerr << "Entramos en método atiende01 con mensaje " << recibido
      << std::endl;
  
  uint16_t offset = recibido.getOffset();
  uint16_t numPos = recibido.getWordAt(4);
  uint8_t numBytes = (numPos - 1) / 8 + 1;
  
  if (!dentroDeRango<bool>(_DO, offset, numPos))
    return generaError(recibido, DIRECCION_INVALIDA);
  
  // Mensaje de respuesta
  respuesta.pushByte_back(recibido.getId());
  respuesta.pushByte_back(recibido.getFuncion());
  respuesta.pushByte_back(numBytes);
  unsigned posBit = 0;
  uint8_t dato = 0;
  for (std::size_t i = offset; i < (offset + numPos); i++) {
    if (_DO.at(i))
      dato |= 1 << posBit;
    posBit++;
    if (posBit == 8) { // Hemos rellenado un byte
      respuesta.pushByte_back(dato);
      posBit = 0;
      dato = 0;
    }
  }
  if (posBit) // Se ha quedado un byte medio relleno
    respuesta.pushByte_back(dato);
  respuesta.aniadeCRC();
  
  return respuesta;
}

Mensaje ModbusRTU::atiende03(Mensaje recibido) {
  Mensaje respuesta;
  std::cerr << "Entramos en método atiende03 con mensaje " << recibido
      << std::endl;
  
  uint16_t offset = recibido.getOffset() - DESPLAZA_AO;
  uint16_t numPos = recibido.getWordAt(4);
  
  if (!dentroDeRango<uint16_t>(_AO, offset, numPos))
    return generaError(recibido, DIRECCION_INVALIDA);
  
  // Mensaje de respuesta
  respuesta.pushByte_back(recibido.getId());
  respuesta.pushByte_back(recibido.getFuncion());
  respuesta.pushByte_back(numPos * 2);
  
  for (std::size_t i = offset; i < (offset + numPos); i++) {
    respuesta.pushWord_back(_AO.at(i));
  }
  respuesta.aniadeCRC();
  
  return respuesta;
}

Mensaje ModbusRTU::atiende05(Mensaje recibido) {
  std::cerr << "Entramos en método atiende05 con mensaje " << recibido
      << std::endl;
  
  uint16_t offset = recibido.getOffset();
  
  if (!dentroDeRango<bool>(_DO, offset))
    return generaError(recibido, DIRECCION_INVALIDA);
  
  if (recibido.getByteAt(4) == 0xFF && recibido.getByteAt(5) == 0x00) {
    _DO.at(offset) = true;
  } else if (recibido.getByteAt(4) == 0x00 && recibido.getByteAt(5) == 0x00) {
    _DO.at(offset) = false;
  } else {
    return generaError(recibido, DATO_INVALIDO);
  }
  
  return recibido;
}

Mensaje ModbusRTU::atiende06(Mensaje recibido) {
  std::cerr << "Entramos en método atiende06 con mensaje " << recibido
      << std::endl;
  
  uint16_t offset = recibido.getOffset() - DESPLAZA_AO;
  uint16_t valor = recibido.getWordAt(4);
  
  if (!dentroDeRango<uint16_t>(_AO, offset))
    return generaError(recibido, DIRECCION_INVALIDA);
  
  _AO.at(offset) = valor;
  
  return recibido;
}

Mensaje ModbusRTU::atiende16(Mensaje recibido) {
  Mensaje respuesta;
  std::cerr << "Entramos en método atiende16 con mensaje " << recibido
      << std::endl;
  
  uint16_t offset = recibido.getOffset() - DESPLAZA_AO;
  uint16_t numPos = recibido.getWordAt(4);
  uint8_t numDatos = recibido.getByteAt(6);
  uint16_t numBytes = numPos * 2;
  
  if (!dentroDeRango<uint16_t>(_AO, offset, numPos))
    return generaError(recibido, DIRECCION_INVALIDA);
  
  if (numBytes == numDatos) { // El paquete está bien formado
    unsigned k = 7; // A partir de aquí solo hay datos y CRC
    uint16_t dato = recibido.getWordAt(k);
    for (std::size_t i = offset; i < (offset + numPos); i++) {
      _AO.at(i) = dato;
      k += 2; // Salta una palabra
      dato = recibido.getWordAt(k);
    }
  } else {
    return generaError(recibido, DATO_INVALIDO);
  }
  
  // Mensaje de respuesta
  respuesta.pushByte_back(recibido.getId());
  respuesta.pushByte_back(recibido.getFuncion());
  respuesta.pushWord_back(recibido.getOffset());
  respuesta.pushWord_back(numPos);
  respuesta.aniadeCRC();
  
  return respuesta;
}

Mensaje ModbusRTU::generaError(Mensaje recibido, uint8_t errorCode) {
  Mensaje respuesta;
  std::cerr << "Entramos en método generaError con mensaje " << recibido
      << " y error " << (int)errorCode << std::endl;
  
  // Mensaje de respuesta
  respuesta.pushByte_back(recibido.getId());
  respuesta.pushByte_back(recibido.getFuncion() | (1 << 7)); // Fun. con MSB a 1
  respuesta.pushByte_back(errorCode);
  respuesta.aniadeCRC();
  
  return respuesta;
}

template <class T, class U> bool ModbusRTU::dentroDeRango
      (const std::vector<T> &registro, U offset, U numPos) const {
  return (offset >= 0) && (offset < registro.size())
      && ((offset + numPos) >= offset)
      && ((offset + numPos) <= registro.size());
}
