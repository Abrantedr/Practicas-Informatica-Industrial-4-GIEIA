// Programa de prueba de la clase ModbusServe

#define TITULO "ModbusRTU: error mal formado"

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

  {
    Mensaje peti({ 0x36, 0x03, 0x56, 0x11 });
    Mensaje resp({ 0x36, 0x83, 0x03, 0xB0, 0xFF });
    testea(mbs, "Leer salidas analogicas NO tamaño mínimo", peti, resp);
  }
  {
    Mensaje peti({ 0x36, 0x03, 0x00, 0x00, 0x00, 0xEC, 0x40 });
    Mensaje resp({ 0x36, 0x83, 0x03, 0xB0, 0xFF });
    testea(mbs, "Leer salidas analogicas mal formado (mas pequeño)", peti, resp);
  }
  {
    Mensaje peti({ 0x36, 0x03, 0x00, 0x00, 0x00, 0x0A, 0x14, 0x22, 0x33, 0x2E, 0x2D });
    Mensaje resp({ 0x36, 0x83, 0x03, 0xB0, 0xFF });
    testea(mbs, "Leer salidas analogicas mal formado (tamaño más grande)", peti, resp);
  }
  {
    Mensaje peti({ 0x36, 0x01, 0xD7, 0xD0 });
    Mensaje resp({ 0x36, 0x81, 0x03, 0xB1, 0x9F });
    testea(mbs, "Leer salidas digitales NO tamaño mínimo", peti, resp);
  }
  {
    Mensaje peti({ 0x36, 0x01, 0x00, 0x00, 0x00, 0xED, 0xF8 });
    Mensaje resp({ 0x36, 0x81, 0x03, 0xB1, 0x9F });
    testea(mbs, "Leer salidas digitales mal formado (mas pequeño)", peti, resp);
  }
  {
    Mensaje peti({ 0x36, 0x01, 0x00, 0x00, 0x00, 0x14, 0xDA, 0xC2, 0x49 });
    Mensaje resp({ 0x36, 0x81, 0x03, 0xB1, 0x9F });
    testea(mbs, "Leer salidas digitales mal formado (mas grande)", peti, resp);
  }
  {
    Mensaje peti({ 0x36, 0x05, 0x00, 0x01, 0xFF, 0xFF, 0x99, 0xFD });
    Mensaje resp({ 0x36, 0x85, 0x03, 0xB3, 0x5F });
    testea(mbs, "Forzamos única salida digital 1 con valor incorrecto FF FF", peti, resp);
  }
  {
    Mensaje peti({ 0x36, 0x05, 0x00, 0x05, 0x01, 0x00, 0xD8, 0x1C });
    Mensaje resp({ 0x36, 0x85, 0x03, 0xB3, 0x5F });
    testea(mbs, "Forzamos única salida digital 5 con valor incorrecto 01 00", peti, resp);
  }
  {
    Mensaje peti({ 0x36, 0x05, 0x00, 0x03, 0xFF, 0xAC, 0x78 });
    Mensaje resp({ 0x36, 0x85, 0x03, 0xB3, 0x5F });
    testea(mbs, "Forzamos única salida digital tamaño incorrecto", peti, resp);
  }
  {
    Mensaje peti({ 0x36, 0x05, 0xD6, 0x13 });
    Mensaje resp({ 0x36, 0x85, 0x03, 0xB3, 0x5F });
    testea(mbs, "Forzamos única salida digital no tamaño mínimo", peti, resp);
  }
  {
    Mensaje peti({ 0x36, 0x05, 0x00, 0x01, 0xFF, 0x00, 0x43, 0x3D, 0x6B });
    Mensaje resp({ 0x36, 0x85, 0x03, 0xB3, 0x5F });
    testea(mbs, "Forzamos única salida digital 1 muy grande", peti, resp);
  }
  {
    Mensaje peti({ 0x36, 0x0F, 0x56, 0x14 });
    Mensaje resp({ 0x36, 0x8F, 0x03, 0xB5, 0xFF });
    testea(mbs, "Escribimos varias digitales NO tamaño mínimo", peti, resp);
  }
  {
    Mensaje peti({ 0x36, 0x0F, 0x00, 0x00, 0x00, 0x14, 0x00, 0xAA, 0xAA, 0x0A, 0x3F, 0x93 });
    Mensaje resp({ 0x36, 0x8F, 0x03, 0xB5, 0xFF });
    testea(mbs, "Escribimos varias digitales num. datos y num. posiciones no coinciden", peti, resp);
  }
  {
    Mensaje peti({ 0x36, 0x0F, 0x00, 0x00, 0x00, 0x14, 0x03, 0xAA, 0xAA, 0x3E, 0x3E });
    Mensaje resp({ 0x36, 0x8F, 0x03, 0xB5, 0xFF });
    testea(mbs, "Escribimos varias digitales numero datos incorrecto", peti, resp);
  }
  {
    Mensaje peti({ 0x36, 0x0F, 0x00, 0x00, 0x00, 0x14, 0x03, 0xAA, 0xAA, 0x0A, 0xAE, 0x16, 0xAC });
    Mensaje resp({ 0x36, 0x8F, 0x03, 0xB5, 0xFF });
    testea(mbs, "Escribimos varias digitales más largo", peti, resp);
  }
  {
    Mensaje peti({ 0x36, 0x01, 0x00, 0x00, 0x00, 0x14, 0x38, 0x42 });
    Mensaje resp({ 0x36, 0x01, 0x03, 0x55, 0x55, 0x05, 0xD7, 0x4A });
    testea(mbs, "Leemos las salidas digitales de la 1 a la 20", peti, resp);
    //Salidas digitales no modificadas [1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0]
  }
  {
    Mensaje peti({ 0x36, 0x10, 0x17, 0xDC });
    Mensaje resp({ 0x36, 0x90, 0x03, 0xBD, 0xCF });
    testea(mbs, "Escribimos varias analogicas NO tamaño mínimo", peti, resp);
  }
  {
    Mensaje peti({ 0x36, 0x10, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x01, 0x00, 0x02, 0x37, 0x1A });
    Mensaje resp({ 0x36, 0x90, 0x03, 0xBD, 0xCF });
    testea(mbs, "Escritura analogica múltiple  num. datos y num. posiciones no coinciden", peti, resp);
  }
  {
    Mensaje peti({ 0x36, 0x10, 0x00, 0x00, 0x00, 0x0B, 0x16, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x00, 0x03, 0x00, 0x04, 0x00, 0x05, 0x00, 0x06, 0x00, 0x07, 0x00, 0x08, 0x00, 0x09, 0x00, 0x5B, 0xCD });
    Mensaje resp({ 0x36, 0x90, 0x03, 0xBD, 0xCF });
    testea(mbs, "Escritura analogica múltiple  num. datos insuficientes", peti, resp);
  }
  {
    Mensaje peti({ 0x36, 0x10, 0x00, 0x00, 0x00, 0x0B, 0x16, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x00, 0x03, 0x00, 0x04, 0x00, 0x05, 0x00, 0x06, 0x00, 0x07, 0x00, 0x08, 0x00, 0x09, 0x00, 0x0A, 0x18, 0xFC, 0x0F });
    Mensaje resp({ 0x36, 0x90, 0x03, 0xBD, 0xCF });
    testea(mbs, "Escritura analogica múltiple  muy largo", peti, resp);
  }
  {
    Mensaje peti({ 0x36, 0x03, 0x00, 0x00, 0x00, 0x0A, 0xC1, 0x8A });
    Mensaje resp({ 0x36, 0x03, 0x14, 0x00, 0x00, 0x00, 0x04, 0x00, 0x08, 0x00, 0x0C, 0x00, 0x10, 0x00, 0x14, 0x00, 0x18, 0x00, 0x1C, 0x00, 0x20, 0x00, 0x24, 0x2A, 0xBD });
    testea(mbs, "Leemos las salidas analogicas de la 1 a la 10", peti, resp);
    //Salidas analogicas no modificadas [0 4 8 12 16 20 24 28 32 36]
  }
  {
    Mensaje peti({ 0x36, 0x06, 0x96, 0x12 });
    Mensaje resp({ 0x36, 0x86, 0x03, 0xB3, 0xAF });
    testea(mbs, "Escribimos unica analogicas NO tamaño mínimo", peti, resp);
  }
  {
    Mensaje peti({ 0x36, 0x06, 0x00, 0x00, 0x0B, 0xAD, 0x4B });
    Mensaje resp({ 0x36, 0x86, 0x03, 0xB3, 0xAF });
    testea(mbs, "Escribimos unica analogicas faltan bytes", peti, resp);
  }
  {
    Mensaje peti({ 0x36, 0x06, 0x00, 0x02, 0x03, 0x08, 0x18, 0xBB, 0x17 });
    Mensaje resp({ 0x36, 0x86, 0x03, 0xB3, 0xAF });
    testea(mbs, "Escribimos unica analogicas muy larga", peti, resp);
  }
  {
    Mensaje peti({ 0x36, 0x03, 0x00, 0x00, 0x00, 0x0A, 0xC1, 0x8A });
    Mensaje resp({ 0x36, 0x03, 0x14, 0x00, 0x00, 0x00, 0x04, 0x00, 0x08, 0x00, 0x0C, 0x00, 0x10, 0x00, 0x14, 0x00, 0x18, 0x00, 0x1C, 0x00, 0x20, 0x00, 0x24, 0x2A, 0xBD });
    testea(mbs, "Leemos las salidas analogicas de la 1 a la 10", peti, resp);
    //Salidas analogicas no modificadas [0 4 8 12 16 20 24 28 32 36]
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