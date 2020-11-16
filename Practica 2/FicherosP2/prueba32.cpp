// Programa de prueba de la clase ModbusServe

#define TITULO "ModbusRTU: entradas analógicas y digitales del 14 al 20"

#include <iostream>
#include <iomanip>
#include <vector>

#include "Mensaje.hpp"
#include "ModbusRTU.hpp"

int pruebas = 0;
int fallos = 0;

#define ID_DEV  54


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

// Registros de entrada analógicos y digitales del 14 al 20 (ID 54)

  {
    Mensaje peti({ 0x36, 0x04, 0x00, 0x70, 0x00, 0x01, 0x34, 0x56 });
    Mensaje resp({ 0x36, 0x04, 0x02, 0x00, 0x00, 0x4C, 0xF4 });
    testea(mbs, "Leemos las entradas analogicas de la 14 a la 14", peti, resp);
  }
  {
    Mensaje peti({ 0x36, 0x04, 0x00, 0x71, 0x00, 0x01, 0x65, 0x96 });
    Mensaje resp({ 0x36, 0x04, 0x02, 0x04, 0x57, 0x0F, 0xCA });
    testea(mbs, "Leemos las entradas analogicas de la 15 a la 15", peti, resp);
  }
  {
    Mensaje peti({ 0x36, 0x04, 0x00, 0x75, 0x00, 0x01, 0x24, 0x57 });
    Mensaje resp({ 0x36, 0x04, 0x02, 0x04, 0x57, 0x0F, 0xCA });
    testea(mbs, "Leemos las entradas analogicas de la 19 a la 19", peti, resp);
  }
  {
    Mensaje peti({ 0x36, 0x04, 0x00, 0x70, 0x00, 0x07, 0xB4, 0x54 });
    Mensaje resp({ 0x36, 0x04, 0x0E, 0x00, 0x00, 0x04, 0x57, 0x00, 0x00, 0x04, 0x57, 0x00, 0x00, 0x04, 0x57, 0x00, 0x00, 0x76, 0x75 });
    testea(mbs, "Leemos las entradas analogicas de la 14 a la 20", peti, resp);
  }
  {
    Mensaje peti({ 0x36, 0x04, 0x00, 0x71, 0x00, 0x05, 0x64, 0x55 });
    Mensaje resp({ 0x36, 0x04, 0x0A, 0x04, 0x57, 0x00, 0x00, 0x04, 0x57, 0x00, 0x00, 0x04, 0x57, 0x37, 0x5A });
    testea(mbs, "Leemos las entradas analogicas de la 15 a la 19", peti, resp);
  }
  {
    Mensaje peti({ 0x36, 0x02, 0x00, 0x0D, 0x00, 0x01, 0x2C, 0x4E });
    Mensaje resp({ 0x36, 0x02, 0x01, 0x00, 0xAF, 0xFC });
    testea(mbs, "Leemos las entradas digitales de la 14 a la 14", peti, resp);
  }
  {
    Mensaje peti({ 0x36, 0x02, 0x00, 0x0E, 0x00, 0x01, 0xDC, 0x4E });
    Mensaje resp({ 0x36, 0x02, 0x01, 0x01, 0x6E, 0x3C });
    testea(mbs, "Leemos las entradas digitales de la 15 a la 15", peti, resp);
  }
  {
    Mensaje peti({ 0x36, 0x02, 0x00, 0x12, 0x00, 0x01, 0x1D, 0x88 });
    Mensaje resp({ 0x36, 0x02, 0x01, 0x00, 0xAF, 0xFC });
    testea(mbs, "Leemos las entradas digitales de la 19 a la 19", peti, resp);
  }
  {
    Mensaje peti({ 0x36, 0x02, 0x00, 0x0D, 0x00, 0x07, 0xAC, 0x4C });
    Mensaje resp({ 0x36, 0x02, 0x01, 0x02, 0x2E, 0x3D });
    testea(mbs, "Leemos las entradas digitales de la 14 a la 20", peti, resp);
  }
  {
    Mensaje peti({ 0x36, 0x02, 0x00, 0x0E, 0x00, 0x05, 0xDD, 0x8D });
    Mensaje resp({ 0x36, 0x02, 0x01, 0x01, 0x6E, 0x3C });
    testea(mbs, "Leemos las entradas digitales de la 15 a la 19", peti, resp);
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
