// Programa de prueba de la clase ModbusServe

#define TITULO "ModbusRTU: error función inválida"

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
    Mensaje peti({ 0x06, 0x07, 0x00, 0x21, 0x00, 0x33, 0xA5, 0xA2 });
    Mensaje resp({ 0x06, 0x87, 0x01, 0x33, 0xF1 });
    testea(mbs, "Funcion inválida", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x09, 0x00, 0x63, 0x03, 0xE8, 0x2C, 0xDC });
    Mensaje resp({ 0x06, 0x89, 0x01, 0x37, 0x91 });
    testea(mbs, "Funcion inválida", peti, resp);
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
