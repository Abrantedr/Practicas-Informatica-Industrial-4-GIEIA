// Programa de prueba de la clase ModbusServe

#define TITULO "ModbusRTU: Escritura salidas analógicas múltiples (función 0x10)"

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

  // Escribimos analogicas multiples
  {
    Mensaje peti({ 0x06, 0x10, 0x00, 0x00, 0x00, 0x01, 0x02, 0x10, 0x00, 0x8D, 0xA0 });
    Mensaje resp({ 0x06, 0x10, 0x00, 0x00, 0x00, 0x01, 0x00, 0x7E });
    testea(mbs, "Formazos salidas analogicas de la 1 a la 1 a los valores 4096", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x03, 0x00, 0x00, 0x00, 0x0A, 0xC4, 0x7A });
    Mensaje resp({ 0x06, 0x03, 0x14, 0x10, 0x00, 0x00, 0x04, 0x00, 0x08, 0x00, 0x0C, 0x00, 0x10, 0x00, 0x14, 0x00, 0x18, 0x00, 0x1C, 0x00, 0x20, 0x00, 0x24, 0xBF, 0x3B });
    testea(mbs, "Leemos las salidas analogicas de la 1 a la 10", peti, resp);
    //Salidas analogicas valen ahora [4096 4 8 12 16 20 24 28 32 36]
  }
  {
    Mensaje peti({ 0x06, 0x10, 0x00, 0x01, 0x00, 0x03, 0x06, 0x10, 0x04, 0x10, 0x08, 0x10, 0x0C, 0xC7, 0x95 });
    Mensaje resp({ 0x06, 0x10, 0x00, 0x01, 0x00, 0x03, 0xD0, 0x7F });
    testea(mbs, "Formazos salidas analogicas de la 2 a la 4 a los valores [4100 4104 4108]", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x03, 0x00, 0x00, 0x00, 0x0A, 0xC4, 0x7A });
    Mensaje resp({ 0x06, 0x03, 0x14, 0x10, 0x00, 0x10, 0x04, 0x10, 0x08, 0x10, 0x0C, 0x00, 0x10, 0x00, 0x14, 0x00, 0x18, 0x00, 0x1C, 0x00, 0x20, 0x00, 0x24, 0xA7, 0xE2 });
    testea(mbs, "Leemos las salidas analogicas de la 1 a la 10", peti, resp);
    //Salidas analogicas valen ahora [4096 4100 4104 4108 16 20 24 28 32 36]
  }
  {
    Mensaje peti({ 0x06, 0x10, 0x00, 0x03, 0x00, 0x07, 0x0E, 0x20, 0x0C, 0x10, 0x10, 0x10, 0x14, 0x10, 0x18, 0x10, 0x1C, 0x10, 0x20, 0x10, 0x24, 0x9A, 0x94 });
    Mensaje resp({ 0x06, 0x10, 0x00, 0x03, 0x00, 0x07, 0x70, 0x7C });
    testea(mbs, "Formazos salidas analogicas de la 4 a la 10 a los valores [8204 4112 4116 4120 4124 4128 4132]", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x03, 0x00, 0x00, 0x00, 0x0A, 0xC4, 0x7A });
    Mensaje resp({ 0x06, 0x03, 0x14, 0x10, 0x00, 0x10, 0x04, 0x10, 0x08, 0x20, 0x0C, 0x10, 0x10, 0x10, 0x14, 0x10, 0x18, 0x10, 0x1C, 0x10, 0x20, 0x10, 0x24, 0x5D, 0x24 });
    testea(mbs, "Leemos las salidas analogicas de la 1 a la 10", peti, resp);
    //Salidas analogicas valen ahora [4096 4100 4104 8204 4112 4116 4120 4124 4128 4132]
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
