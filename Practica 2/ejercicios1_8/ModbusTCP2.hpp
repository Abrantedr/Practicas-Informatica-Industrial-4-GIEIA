//
// Created by Rubén Abrante Delgado on 20/11/2020.
//

#ifndef _MODBUSTCP2_
#define _MODBUSTCP2_

#include "ModbusRTU.hpp"

class ModbusTCP2 {

private:
  uint16_t _puerto; // Puerto TCP por el que escucha el Modbus/TCP

  ModbusRTU _mbusRTU1; // Unidad 1 Modbus/RTU conectada
  ModbusRTU _mbusRTU2; // Unidad 2 Modbus/RTU conectada

  uint8_t _uId1; // Identificador 1 de la unidad ModbusRTU1
  uint8_t _uId2; // Identificador 2 de la unidad ModbusRTU2

public:
  //Constructor que indica el puerto TCP donde debe escuchar y el
  // identificador de las dos unidades ModbusRTU conectadas.
  ModbusTCP2(uint16_t puerto, uint8_t unId1, uint8_t uId2);

  /* Abre la conexión TCP , acepta conexiones de los clientes redirigiendo
   * las peticiones a la unidad ModbusRTU y devolviendo la respuesta al
   * cliente. Debe controlar los errores y las desconexiones de los
   * clientes. Cuando haya atendido a numClientes clientes debe cerrar
   * el socket y terminar. En caso de error en la comunicación TCP deberán
   * lanzar una excepción del tipo std::runtime_error con un mensaje adecuado */
  void atiende(unsigned numClientes);

};


#endif // _MODBUSTCP2_
