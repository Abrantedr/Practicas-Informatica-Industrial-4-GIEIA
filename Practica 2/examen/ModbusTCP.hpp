// Autor: Rubén Abrante Delgado. Fecha: 02/12/2020

//
// Clase que implementa un servidor ModbusTCP
//

#ifndef _MODBUSTCP_
#define _MODBUSTCP_

#include "ModbusRTU.hpp"

class ModbusTCP {
  
private:
  uint16_t _puerto; // Puerto de escucha
  ModbusRTU _mbusRTU; // Unidad Modbus/RTU conectada

public:
  // Constructor que indica el puerto TCP donde debe escuchar y el identificador
  // de la unidad Modbus/RTU conectada
  ModbusTCP(uint16_t puerto, uint8_t unitId);
  
  /* Abre la conexión TCP, acepta la conexión de UN cliente redirigiendo las
   * peticiones a la unidad ModbusRTU y devolviendo la respuesta al cliente.
   * Controla la desconexión del cliente y, cuando esto ocurra, cerrará el
   * socket y terminará. No realiza control de errores */
  void atiende();
};

#endif
