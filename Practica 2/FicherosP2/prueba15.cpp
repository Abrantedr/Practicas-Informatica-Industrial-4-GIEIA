// Programa de prueba de la clase ModbusServe

#define TITULO "ModbusRTU: Escritura salidas analógica única (función 0x06)"

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
    Mensaje peti({ 0x06, 0x06, 0x00, 0x00, 0x03, 0x00, 0x88, 0x8D });
    Mensaje resp({ 0x06, 0x06, 0x00, 0x00, 0x03, 0x00, 0x88, 0x8D });
    testea(mbs, "Formazos salida analogica unica 1 a valor 768", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x03, 0x00, 0x00, 0x00, 0x0A, 0xC4, 0x7A });
    Mensaje resp({ 0x06, 0x03, 0x14, 0x03, 0x00, 0x00, 0x04, 0x00, 0x08, 0x00, 0x0C, 0x00, 0x10, 0x00, 0x14, 0x00, 0x18, 0x00, 0x1C, 0x00, 0x20, 0x00, 0x24, 0x39, 0x9B });
    testea(mbs, "Leemos las salidas analogicas de la 1 a la 10", peti, resp);
    //Salidas analogicas valen ahora [768 4 8 12 16 20 24 28 32 36]
  }
  {
    Mensaje peti({ 0x06, 0x06, 0x00, 0x09, 0x03, 0x24, 0x58, 0x94 });
    Mensaje resp({ 0x06, 0x06, 0x00, 0x09, 0x03, 0x24, 0x58, 0x94 });
    testea(mbs, "Formazos salida analogica unica 10 a valor 804", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x03, 0x00, 0x00, 0x00, 0x0A, 0xC4, 0x7A });
    Mensaje resp({ 0x06, 0x03, 0x14, 0x03, 0x00, 0x00, 0x04, 0x00, 0x08, 0x00, 0x0C, 0x00, 0x10, 0x00, 0x14, 0x00, 0x18, 0x00, 0x1C, 0x00, 0x20, 0x03, 0x24, 0x39, 0x6B });
    testea(mbs, "Leemos las salidas analogicas de la 1 a la 10", peti, resp);
    //Salidas analogicas valen ahora [768 4 8 12 16 20 24 28 32 804]
  }
  {
    Mensaje peti({ 0x06, 0x06, 0x00, 0x04, 0x03, 0x10, 0xC8, 0x80 });
    Mensaje resp({ 0x06, 0x06, 0x00, 0x04, 0x03, 0x10, 0xC8, 0x80 });
    testea(mbs, "Formazos salida analogica unica 5 a valor 784", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x03, 0x00, 0x00, 0x00, 0x0A, 0xC4, 0x7A });
    Mensaje resp({ 0x06, 0x03, 0x14, 0x03, 0x00, 0x00, 0x04, 0x00, 0x08, 0x00, 0x0C, 0x03, 0x10, 0x00, 0x14, 0x00, 0x18, 0x00, 0x1C, 0x00, 0x20, 0x03, 0x24, 0x3D, 0x6F });
    testea(mbs, "Leemos las salidas analogicas de la 1 a la 10", peti, resp);
    //Salidas analogicas valen ahora [768 4 8 12 784 20 24 28 32 804]
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
