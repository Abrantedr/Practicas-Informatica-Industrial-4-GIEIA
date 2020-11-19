//
// Created by Rubén Abrante Delgado on 19/11/2020.
//

#ifndef _MODBUSTCP_
#define _MODBUSTCP_


#include <cstdint>
#include "ModbusRTU.hpp"

class ModbusTCP {

private:
  uint16_t _puerto;
  ModbusRTU _mbusRTU;

public:
  //Constructor que indica el puerto TCP donde debe escuchar y el
  // identificador de la unidad ModbusRTU conectada.
  ModbusTCP(uint16_t puerto, uint8_t unitId);

  /* Abre la conexión TCP , acepta conexiones de los clientes redirigiendo
   * las peticiones a la unidad ModbusRTU y devolviendo la respuesta al
   * cliente. Debe controlar los errores y las desconexiones de los
   * clientes. Cuando haya atendido a numClientes clientes debe cerrar
   * el socket y terminar. En caso de error en la comunicación TCP deberán
   * lanzar una excepción del tipo std::runtime_error con un mensaje adecuado */
  void atiende(unsigned numClientes);

};


#endif // _MODBUSTCP_
