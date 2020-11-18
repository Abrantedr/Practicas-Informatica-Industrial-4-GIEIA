//
// Created by Rubén Abrante Delgado on 17/11/2020.
//

#ifndef _MODBUSRTU_
#define _MODBUSRTU_

#include <cstdint>
#include <vector>

#include "Mensaje.hpp"


class ModbusRTU {

private:
  uint8_t _id;
  std::vector<bool> _DO; // Registro de salidas digitales
  std::vector<uint16_t> _AO; // Registro de salidas analógicas

  // Lectura de salidas analógicas
  Mensaje atiende03(Mensaje& recibido);

  // Fuerza una única salida analógica
  Mensaje atiende06(Mensaje& recibido);

  // Fuerza múltiples salidas analógicas
  Mensaje atiende16(Mensaje& recibido);

  // Lectura de salidas digitales
  Mensaje atiende01(Mensaje& recibido);

  // Fuerza una única salida digital
  Mensaje atiende05(Mensaje& recibido);

  // Fuerza múltiples salidas digitales
  Mensaje atiende15(Mensaje& recibido);

  // Comprueba si el acceso a un registro está fuera de rango
  template <class T> bool dentroDeRango(const std::vector<T>& registro,
      uint16_t offset, uint16_t numPos=0) const;

  // Genera un mensaje de error dado un mensaje recibido y un código de
  // error
  static Mensaje generaError(Mensaje& recibido, uint8_t errorCode);

public:
  // Constructor donde se especifica el identificador del dispositivo.
  explicit ModbusRTU(uint8_t id);

  // Recibe como parámetro un mensaje de petición Modbus/RTU y devuelve
  // la respuesta correspondiente.
  Mensaje peticion(Mensaje& recibido);
};

#endif //_MODBUSRTU_
