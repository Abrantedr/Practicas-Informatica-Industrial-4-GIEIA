// Programa de prueba de la clase ModbusServe

#define TITULO "ModbusRTU: errores entradas digitales y analógicas"

#include <iostream>
#include <iomanip>
#include <vector>

#include "Mensaje.hpp"
#include "ModbusRTU.hpp"

#include <sys/types.h>
#include <unistd.h>

int pruebas = 0;
int fallos = 0;

#define ID_DEV  247


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


// Errores de fuera de rango entradas

  {
    Mensaje peti({ 0xF7, 0x04, 0x00, 0x14, 0x00, 0x01, 0x65, 0x58 });
    Mensaje resp({ 0xF7, 0x84, 0x02, 0x22, 0xF3 });
    testea(mbs, "Error, lectura entrada analogica en 21", peti, resp);
  }
  {
    Mensaje peti({ 0xF7, 0x04, 0x00, 0x09, 0x00, 0x10, 0x35, 0x52 });
    Mensaje resp({ 0xF7, 0x84, 0x02, 0x22, 0xF3 });
    testea(mbs, "Error, lectura entrada analogica de 10 a 25", peti, resp);
  }
  {
    Mensaje peti({ 0xF7, 0x04, 0x00, 0x00, 0x00, 0x15, 0x25, 0x53 });
    Mensaje resp({ 0xF7, 0x84, 0x02, 0x22, 0xF3 });
    testea(mbs, "Error, lectura entrada analogica de 1 a 21", peti, resp);
  }
  {
    Mensaje peti({ 0xF7, 0x02, 0x00, 0x14, 0x00, 0x01, 0xED, 0x58 });
    Mensaje resp({ 0xF7, 0x82, 0x02, 0x21, 0x53 });
    testea(mbs, "Error, Leemos las entrada digital 21", peti, resp);
  }
  {
    Mensaje peti({ 0xF7, 0x02, 0x00, 0x00, 0x00, 0x15, 0xAD, 0x53 });
    Mensaje resp({ 0xF7, 0x82, 0x02, 0x21, 0x53 });
    testea(mbs, "Error, Leemos las entrada digital de 1 a 21", peti, resp);
  }
  {
    Mensaje peti({ 0xF7, 0x02, 0x00, 0x0E, 0x00, 0x0B, 0x4C, 0x98 });
    Mensaje resp({ 0xF7, 0x82, 0x02, 0x21, 0x53 });
    testea(mbs, "Error, Leemos las entrada digital de 15 a 25", peti, resp);
  }


// Errores mal formados entradas

  {
    Mensaje peti({ 0xF7, 0x04, 0x46, 0x43 });
    Mensaje resp({ 0xF7, 0x84, 0x03, 0xE3, 0x33 });
    testea(mbs, "Leer entradas analogicas NO tamaño mínimo", peti, resp);
  }
  {
    Mensaje peti({ 0xF7, 0x04, 0x00, 0x00, 0x00, 0xD0, 0xE5 });
    Mensaje resp({ 0xF7, 0x84, 0x03, 0xE3, 0x33 });
    testea(mbs, "Leer entradas analogicas mal formado (tamaño menor)", peti, resp);
  }
  {
    Mensaje peti({ 0xF7, 0x04, 0x00, 0x00, 0x00, 0x14, 0x5B, 0xD2, 0xB0 });
    Mensaje resp({ 0xF7, 0x84, 0x03, 0xE3, 0x33 });
    testea(mbs, "Leer entradas analogicas mal formado (tamaño muy grande)", peti, resp);
  }
  {
    Mensaje peti({ 0xF7, 0x02, 0xC6, 0x41 });
    Mensaje resp({ 0xF7, 0x82, 0x03, 0xE0, 0x93 });
    testea(mbs, "Leer entradas digitales NO tamaño mínimo", peti, resp);
  }
  {
    Mensaje peti({ 0xF7, 0x02, 0x00, 0x00, 0x00, 0xD0, 0x6D });
    Mensaje resp({ 0xF7, 0x82, 0x03, 0xE0, 0x93 });
    testea(mbs, "Leer entradas digitales mal formado (tamaño menor)", peti, resp);
  }
  {
    Mensaje peti({ 0xF7, 0x02, 0x00, 0x00, 0x00, 0x14, 0x7C, 0x92, 0xCC });
    Mensaje resp({ 0xF7, 0x82, 0x03, 0xE0, 0x93 });
    testea(mbs, "Leer entradas digitales mal formado (tamaño muy grande)", peti, resp);
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
