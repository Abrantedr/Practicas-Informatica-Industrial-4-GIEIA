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
  uint8_t _id; // Dirección del dispositivo ModbusRTU
  std::vector<bool> _DO; // Registro de salidas digitales
  std::vector<uint16_t> _AO; // Registro de salidas analógicas
  std::vector<bool> _DI; // Registro de entradas digitales
  std::vector<uint16_t> _AI; // Registro de entradas analógicas

  unsigned _petRecibidas = 0; // Número de peticiones recibidas
  unsigned _byteRecibidos = 0; // Número de bytes recibidos (sin CRC)
  unsigned _byteEnviados = 0; // Número de bytes enviados (sin CRC)

  Mensaje atiende03(Mensaje& recibido); // Lectura de salidas analógicas
  Mensaje atiende06(Mensaje& recibido); // Fuerza una única salida analógica
  Mensaje atiende16(Mensaje& recibido); // Fuerza múltiples salidas analógicas
  Mensaje atiende01(Mensaje& recibido); // Lectura de salidas digitales
  Mensaje atiende05(Mensaje& recibido); // Fuerza una única salida digital
  Mensaje atiende15(Mensaje& recibido); // Fuerza múltiples salidas digitales
  Mensaje atiende02(Mensaje& recibido); // Lectura de entradas digitales
  Mensaje atiende04(Mensaje& recibido); // Lectura de entradas analógicas

  // Comprueba si el acceso a un registro está fuera de rango
  template <class T, class U=uint16_t> bool dentroDeRango(
      const std::vector<T>& registro, U offset, U numPos=0) const;

  // Genera un mensaje de error dado un mensaje recibido y un código de
  // error
  static Mensaje generaError(Mensaje& recibido, uint8_t errorCode);

  // Comprueba si un mensaje es válido
  static bool esValido(Mensaje& recibido, uint8_t funcion);

  // Actualiza los valores de entrada analógicas según varios parámetros
  void actualizaAI();

  // Muestra el valor de un registro por std::cerr (debugging)
  template <class T> void muestraRegistro(
      const std::vector<T>& registro,
      const std::string& identificador) const;

public:
  // Constructor donde se especifica el identificador del dispositivo.
  explicit ModbusRTU(uint8_t id);

  // Recibe como parámetro un mensaje de petición Modbus/RTU y devuelve
  // la respuesta correspondiente.
  Mensaje peticion(Mensaje& recibido);
};

#endif //_MODBUSRTU_
