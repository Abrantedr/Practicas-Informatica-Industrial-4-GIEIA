// Autor: Rubén Abrante Delgado. Fecha: 02/12/2020

//
// Clase que implementa un servidor Modbus/RTU
//

#ifndef _MODBUSRTU_
#define _MODBUSRTU_

#include "Mensaje.hpp"

class ModbusRTU {
  
public:
  // Constructor donde se especifica el identificador del dispositivo
  ModbusRTU(uint8_t id);
  
  // Recibe como parámetro un mensaje de petición Modbus/RTU y devuelve la
  // respuesta correspondiente
  Mensaje peticion(Mensaje recibido);

private:
  // Dirección del dispositivo
  uint8_t _id; 
  
  // Registros de salida analógicos y digitales
  std::vector<bool> _DO;
  std::vector<uint16_t> _AO;
  
  // Funciones
  Mensaje atiende01(Mensaje recibido);
  Mensaje atiende03(Mensaje recibido);
  Mensaje atiende05(Mensaje recibido);
  Mensaje atiende06(Mensaje recibido);
  Mensaje atiende16(Mensaje recibido);
  
  // Control de errores
  Mensaje generaError(Mensaje recibido, uint8_t errorCode);
  template <class T, class U=uint16_t> bool dentroDeRango
      (const std::vector<T> &registro, U offset, U numPos=0) const;
};

#endif
