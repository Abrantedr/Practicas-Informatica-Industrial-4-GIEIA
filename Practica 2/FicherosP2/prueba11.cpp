// Programa de prueba de la clase ModbusServe

#define TITULO "ModbusRTU: lectura salidas analógicas (función 0x03)"

#include <iostream>
#include <iomanip>
#include <vector>
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
    Mensaje peti({ 0x06, 0x03, 0x00, 0x00, 0x00, 0x01, 0x85, 0xBD });
    Mensaje resp({ 0x06, 0x03, 0x02, 0x00, 0x00, 0x0D, 0x84 });
    testea(mbs, "Leemos las salidas analogicas de la 1 a la 1", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x03, 0x00, 0x01, 0x00, 0x01, 0xD4, 0x7D });
    Mensaje resp({ 0x06, 0x03, 0x02, 0x00, 0x04, 0x0C, 0x47 });
    testea(mbs, "Leemos las salidas analogicas de la 2 a la 2", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x03, 0x00, 0x09, 0x00, 0x01, 0x55, 0xBF });
    Mensaje resp({ 0x06, 0x03, 0x02, 0x00, 0x24, 0x0D, 0x9F });
    testea(mbs, "Leemos las salidas analogicas de la 10 a la 10", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x03, 0x00, 0x00, 0x00, 0x02, 0xC5, 0xBC });
    Mensaje resp({ 0x06, 0x03, 0x04, 0x00, 0x00, 0x00, 0x04, 0x8D, 0x30 });
    testea(mbs, "Leemos las salidas analogicas de la 1 a la 2", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x03, 0x00, 0x02, 0x00, 0x06, 0x65, 0xBF });
    Mensaje resp({ 0x06, 0x03, 0x0C, 0x00, 0x08, 0x00, 0x0C, 0x00, 0x10, 0x00, 0x14, 0x00, 0x18, 0x00, 0x1C, 0x0B, 0x1E });
    testea(mbs, "Leemos las salidas analogicas de la 3 a la 8", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x03, 0x00, 0x00, 0x00, 0x0A, 0xC4, 0x7A });
    Mensaje resp({ 0x06, 0x03, 0x14, 0x00, 0x00, 0x00, 0x04, 0x00, 0x08, 0x00, 0x0C, 0x00, 0x10, 0x00, 0x14, 0x00, 0x18, 0x00, 0x1C, 0x00, 0x20, 0x00, 0x24, 0x7D, 0xA8 });
    testea(mbs, "Leemos las salidas analogicas de la 1 a la 10", peti, resp);
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
