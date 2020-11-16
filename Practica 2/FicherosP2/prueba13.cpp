// Programa de prueba de la clase ModbusServe

#define TITULO "ModbusRTU: Escritura digital salida única (función 0x05)"

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
    Mensaje peti({ 0x06, 0x05, 0x00, 0x00, 0x00, 0x00, 0xCC, 0x7D });
    Mensaje resp({ 0x06, 0x05, 0x00, 0x00, 0x00, 0x00, 0xCC, 0x7D });
    testea(mbs, "Forzamos salida digital unica 1 a valor 0", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x01, 0x00, 0x00, 0x00, 0x14, 0x3D, 0xB2 });
    Mensaje resp({ 0x06, 0x01, 0x03, 0x54, 0x55, 0x05, 0x83, 0x7A });
    testea(mbs, "Leemos las salidas digitales de la 1 a la 20", peti, resp);
    //Salidas digitales valen ahora [0 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0]
  }
  {
    Mensaje peti({ 0x06, 0x05, 0x00, 0x01, 0xFF, 0x00, 0xDC, 0x4D });
    Mensaje resp({ 0x06, 0x05, 0x00, 0x01, 0xFF, 0x00, 0xDC, 0x4D });
    testea(mbs, "Forzamos salida digital unica 2 a valor 1", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x01, 0x00, 0x00, 0x00, 0x14, 0x3D, 0xB2 });
    Mensaje resp({ 0x06, 0x01, 0x03, 0x56, 0x55, 0x05, 0x22, 0xBA });
    testea(mbs, "Leemos las salidas digitales de la 1 a la 20", peti, resp);
    //Salidas digitales valen ahora [0 1 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0]
  }
  {
    Mensaje peti({ 0x06, 0x05, 0x00, 0x09, 0xFF, 0x00, 0x5D, 0x8F });
    Mensaje resp({ 0x06, 0x05, 0x00, 0x09, 0xFF, 0x00, 0x5D, 0x8F });
    testea(mbs, "Forzamos salida digital unica 10 a valor 1", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x01, 0x00, 0x00, 0x00, 0x14, 0x3D, 0xB2 });
    Mensaje resp({ 0x06, 0x01, 0x03, 0x56, 0x57, 0x05, 0x23, 0xDA });
    testea(mbs, "Leemos las salidas digitales de la 1 a la 20", peti, resp);
    //Salidas digitales valen ahora [0 1 1 0 1 0 1 0 1 1 1 0 1 0 1 0 1 0 1 0]
  }
  {
    Mensaje peti({ 0x06, 0x05, 0x00, 0x04, 0x00, 0x00, 0x8D, 0xBC });
    Mensaje resp({ 0x06, 0x05, 0x00, 0x04, 0x00, 0x00, 0x8D, 0xBC });
    testea(mbs, "Forzamos salida digital unica 5 a valor 0", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x01, 0x00, 0x00, 0x00, 0x14, 0x3D, 0xB2 });
    Mensaje resp({ 0x06, 0x01, 0x03, 0x46, 0x57, 0x05, 0x22, 0x1F });
    testea(mbs, "Leemos las salidas digitales de la 1 a la 20", peti, resp);
    //Salidas digitales valen ahora [0 1 1 0 0 0 1 0 1 1 1 0 1 0 1 0 1 0 1 0]
  }
  {
    Mensaje peti({ 0x06, 0x05, 0x00, 0x13, 0xFF, 0x00, 0x7C, 0x48 });
    Mensaje resp({ 0x06, 0x05, 0x00, 0x13, 0xFF, 0x00, 0x7C, 0x48 });
    testea(mbs, "Forzamos salida digital unica 20 a valor 1", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x01, 0x00, 0x00, 0x00, 0x14, 0x3D, 0xB2 });
    Mensaje resp({ 0x06, 0x01, 0x03, 0x46, 0x57, 0x0D, 0x23, 0xD9 });
    testea(mbs, "Leemos las salidas digitales de la 1 a la 20", peti, resp);
    //Salidas digitales valen ahora [0 1 1 0 0 0 1 0 1 1 1 0 1 0 1 0 1 0 1 1]
  }
  {
    Mensaje peti({ 0x06, 0x05, 0x00, 0x01, 0xFF, 0x00, 0xDC, 0x4D });
    Mensaje resp({ 0x06, 0x05, 0x00, 0x01, 0xFF, 0x00, 0xDC, 0x4D });
    testea(mbs, "Dejamos salida digital unica 2 a valor 1", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x01, 0x00, 0x00, 0x00, 0x14, 0x3D, 0xB2 });
    Mensaje resp({ 0x06, 0x01, 0x03, 0x46, 0x57, 0x0D, 0x23, 0xD9 });
    testea(mbs, "Leemos las salidas digitales de la 1 a la 20", peti, resp);
    //Salidas digitales valen ahora [0 1 1 0 0 0 1 0 1 1 1 0 1 0 1 0 1 0 1 1]
  }
  {
    Mensaje peti({ 0x06, 0x05, 0x00, 0x04, 0x00, 0x00, 0x8D, 0xBC });
    Mensaje resp({ 0x06, 0x05, 0x00, 0x04, 0x00, 0x00, 0x8D, 0xBC });
    testea(mbs, "Dejamos salida digital unica 5 a valor 0", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x01, 0x00, 0x00, 0x00, 0x14, 0x3D, 0xB2 });
    Mensaje resp({ 0x06, 0x01, 0x03, 0x46, 0x57, 0x0D, 0x23, 0xD9 });
    testea(mbs, "Leemos las salidas digitales de la 1 a la 20", peti, resp);
    //Salidas digitales valen ahora [0 1 1 0 0 0 1 0 1 1 1 0 1 0 1 0 1 0 1 1]
  }
  {
    Mensaje peti({ 0x06, 0x05, 0x00, 0x13, 0xFF, 0x00, 0x7C, 0x48 });
    Mensaje resp({ 0x06, 0x05, 0x00, 0x13, 0xFF, 0x00, 0x7C, 0x48 });
    testea(mbs, "Dejamos salida digital unica 20 a valor 1", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x01, 0x00, 0x00, 0x00, 0x14, 0x3D, 0xB2 });
    Mensaje resp({ 0x06, 0x01, 0x03, 0x46, 0x57, 0x0D, 0x23, 0xD9 });
    testea(mbs, "Leemos las salidas digitales de la 1 a la 20", peti, resp);
    //Salidas digitales valen ahora [0 1 1 0 0 0 1 0 1 1 1 0 1 0 1 0 1 0 1 1]
  }
  {
    Mensaje peti({ 0x06, 0x05, 0x00, 0x02, 0xFF, 0x00, 0x2C, 0x4D });
    Mensaje resp({ 0x06, 0x05, 0x00, 0x02, 0xFF, 0x00, 0x2C, 0x4D });
    testea(mbs, "Dejamos salida digital unica 3 a valor 1", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x01, 0x00, 0x00, 0x00, 0x14, 0x3D, 0xB2 });
    Mensaje resp({ 0x06, 0x01, 0x03, 0x46, 0x57, 0x0D, 0x23, 0xD9 });
    testea(mbs, "Leemos las salidas digitales de la 1 a la 20", peti, resp);
    //Salidas digitales valen ahora [0 1 1 0 0 0 1 0 1 1 1 0 1 0 1 0 1 0 1 1]
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
