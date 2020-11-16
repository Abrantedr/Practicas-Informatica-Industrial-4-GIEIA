// Programa de prueba de la clase ModbusServe

#define TITULO "ModbusRTU: lectura de salidas digitales (función 0x01)"

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
    Mensaje peti({ 0x06, 0x01, 0x00, 0x00, 0x00, 0x01, 0xFC, 0x7D });
    Mensaje resp({ 0x06, 0x01, 0x01, 0x01, 0x91, 0x3C });
    testea(mbs, "Leemos las salidas digitales de la 1 a la 1", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x01, 0x00, 0x01, 0x00, 0x01, 0xAD, 0xBD });
    Mensaje resp({ 0x06, 0x01, 0x01, 0x00, 0x50, 0xFC });
    testea(mbs, "Leemos las salidas digitales de la 2 a la 2", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x01, 0x00, 0x02, 0x00, 0x08, 0x9D, 0xBB });
    Mensaje resp({ 0x06, 0x01, 0x01, 0x55, 0x90, 0xC3 });
    testea(mbs, "Leemos las salidas digitales de la 3 a la 10", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x01, 0x00, 0x09, 0x00, 0x0B, 0xAC, 0x78 });
    Mensaje resp({ 0x06, 0x01, 0x02, 0xAA, 0x02, 0xF3, 0x5D });
    testea(mbs, "Leemos las salidas digitales de la 10 a la 20", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x01, 0x00, 0x00, 0x00, 0x14, 0x3D, 0xB2 });
    Mensaje resp({ 0x06, 0x01, 0x03, 0x55, 0x55, 0x05, 0xD2, 0xBA });
    testea(mbs, "Leemos las salidas digitales de la 1 a la 20", peti, resp);
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
