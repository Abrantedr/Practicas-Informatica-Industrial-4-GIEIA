// Programa de prueba de la clase ModbusServe

#define TITULO "ModbusRTU: error de ID y CRC"

#include <iostream>
#include <iomanip>
#include <vector>

#include "Mensaje.hpp"
#include "ModbusRTU.hpp"

int pruebas = 0;
int fallos = 0;

#define ID_DEV  6


void testea(ModbusRTU &mbs, std::string titulo, Mensaje peticion,
    Mensaje respuestaOK) {

  pruebas++;

  std::cout << "\n\nTest '" << titulo << "' enviamos " << peticion.toString() << "\n";

  Mensaje respuesta = mbs.peticion(peticion);

  if(respuesta.toString() == respuestaOK.toString()) {
    std::cout << "CORRECTO: la respuesta fue " << respuesta.toString() << std::endl;
  } else {
    std::cout << "FALLO "  << ++fallos << ": la respuesta debería ser "
        << respuestaOK.toString() << " pero es " << respuesta.toString() << std::endl;
  }
}

int main (int argc, char *argv[]) {

  std::cout << "\n***  " << TITULO << "  ***\n" << std::endl;


  ModbusRTU mbs(ID_DEV);

  {
    Mensaje peti({ 0x08, 0x01, 0x00, 0x00, 0x00, 0x14, 0x3C, 0x9C });
    Mensaje resp;
    testea(mbs, "Leemos las salidas digitales con ID incorrecto", peti, resp);
  }
  {
    Mensaje peti({ 0x04, 0x03, 0x00, 0x00, 0x00, 0x0A, 0xC5, 0x98 });
    Mensaje resp;
    testea(mbs, "Leemos las salidas analogicas con ID incorrecto", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x01, 0x00, 0x00, 0x00, 0x14, 0x21, 0x4D });
    Mensaje resp;
    testea(mbs, "Error CRC incorrecto, leyendo las salidas digitales de la 1 a la 20", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x06, 0x00, 0x04, 0x16, 0x10, 0xC8, 0x1D });
    Mensaje resp;
    testea(mbs, "Error CRC incorrecto, forzando salida analogica unica 5 a valor 5648", peti, resp);
  }


  // Resumen final
  if (fallos == 0)
    std::cout << "\n :-) Todas las " << pruebas << " pruebas correctas\n"
        << std::endl;
  else
    std::cout << "\n :-( Se han producido " << fallos << " FALLOs de "
        << pruebas << " pruebas (" << std::fixed <<  std::setprecision(0)
        << fallos*100.0/pruebas << "%)\n"
        << std::endl;
  return fallos;
}
