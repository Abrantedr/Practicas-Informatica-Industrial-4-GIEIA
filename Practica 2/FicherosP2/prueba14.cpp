// Programa de prueba de la clase ModbusServe

#define TITULO "ModbusRTU: Escrituras de salidas digitales (función 0x0F)"

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


// Escribimos digitales multiples
  {
    Mensaje peti({ 0x06, 0x0F, 0x00, 0x08, 0x00, 0x01, 0x01, 0x00, 0x8E, 0xB0 });
    Mensaje resp({ 0x06, 0x0F, 0x00, 0x08, 0x00, 0x01, 0x14, 0x7E });
    testea(mbs, "Formazos salidas digitales de la 9 a la 9 a los valores 0", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x01, 0x00, 0x00, 0x00, 0x14, 0x3D, 0xB2 });
    Mensaje resp({ 0x06, 0x01, 0x03, 0x55, 0x54, 0x05, 0xD3, 0x2A });
    testea(mbs, "Leemos las salidas digitales de la 1 a la 20", peti, resp);
    //Salidas digitales valen ahora [1 0 1 0 1 0 1 0 0 0 1 0 1 0 1 0 1 0 1 0]
  }
  {
    Mensaje peti({ 0x06, 0x0F, 0x00, 0x00, 0x00, 0x03, 0x01, 0x02, 0x4F, 0x70 });
    Mensaje resp({ 0x06, 0x0F, 0x00, 0x00, 0x00, 0x03, 0x14, 0x7D });
    testea(mbs, "Formazos salidas digitales de la 1 a la 3 a los valores [0 1 0]", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x01, 0x00, 0x00, 0x00, 0x14, 0x3D, 0xB2 });
    Mensaje resp({ 0x06, 0x01, 0x03, 0x52, 0x54, 0x05, 0x62, 0xEB });
    testea(mbs, "Leemos las salidas digitales de la 1 a la 20", peti, resp);
    //Salidas digitales valen ahora [0 1 0 0 1 0 1 0 0 0 1 0 1 0 1 0 1 0 1 0]
  }
  {
    Mensaje peti({ 0x06, 0x0F, 0x00, 0x04, 0x00, 0x06, 0x01, 0x3A, 0xAF, 0x63 });
    Mensaje resp({ 0x06, 0x0F, 0x00, 0x04, 0x00, 0x06, 0x95, 0xBF });
    testea(mbs, "Formazos salidas digitales de la 5 a la 10 a los valores [0 1 0 1 1 1]", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x01, 0x00, 0x00, 0x00, 0x14, 0x3D, 0xB2 });
    Mensaje resp({ 0x06, 0x01, 0x03, 0xA2, 0x57, 0x05, 0x62, 0x28 });
    testea(mbs, "Leemos las salidas digitales de la 1 a la 20", peti, resp);
    //Salidas digitales valen ahora [0 1 0 0 0 1 0 1 1 1 1 0 1 0 1 0 1 0 1 0]
  }
  {
    Mensaje peti({ 0x06, 0x0F, 0x00, 0x09, 0x00, 0x0B, 0x02, 0x54, 0x05, 0x3C, 0xAE });
    Mensaje resp({ 0x06, 0x0F, 0x00, 0x09, 0x00, 0x0B, 0xC5, 0xB9 });
    testea(mbs, "Formazos salidas digitales de la 10 a la 20 a los valores [0 0 1 0 1 0 1 0 1 0 1]", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x01, 0x00, 0x00, 0x00, 0x14, 0x3D, 0xB2 });
    Mensaje resp({ 0x06, 0x01, 0x03, 0xA2, 0xA9, 0x0A, 0x62, 0x4C });
    testea(mbs, "Leemos las salidas digitales de la 1 a la 20", peti, resp);
    //Salidas digitales valen ahora [0 1 0 0 0 1 0 1 1 0 0 1 0 1 0 1 0 1 0 1]
  }
  {
    Mensaje peti({ 0x06, 0x0F, 0x00, 0x00, 0x00, 0x14, 0x03, 0x5D, 0x56, 0x05, 0x8F, 0xDE });
    Mensaje resp({ 0x06, 0x0F, 0x00, 0x00, 0x00, 0x14, 0x54, 0x73 });
    testea(mbs, "Formazos salidas digitales de la 1 a la 20 a los valores [1 0 1 1 1 0 1 0 0 1 1 0 1 0 1 0 1 0 1 0]", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x01, 0x00, 0x00, 0x00, 0x14, 0x3D, 0xB2 });
    Mensaje resp({ 0x06, 0x01, 0x03, 0x5D, 0x56, 0x05, 0x53, 0x88 });
    testea(mbs, "Leemos las salidas digitales de la 1 a la 20", peti, resp);
    //Salidas digitales valen ahora [1 0 1 1 1 0 1 0 0 1 1 0 1 0 1 0 1 0 1 0]
  }
  {
    Mensaje peti({ 0x06, 0x0F, 0x00, 0x00, 0x00, 0x09, 0x02, 0xA2, 0x01, 0x7B, 0xEC });
    Mensaje resp({ 0x06, 0x0F, 0x00, 0x00, 0x00, 0x09, 0x94, 0x7A });
    testea(mbs, "Formazos salidas digitales de la 1 a la 9 a los valores [0 1 0 0 0 1 0 1 1]", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x01, 0x00, 0x00, 0x00, 0x14, 0x3D, 0xB2 });
    Mensaje resp({ 0x06, 0x01, 0x03, 0xA2, 0x57, 0x05, 0x62, 0x28 });
    testea(mbs, "Leemos las salidas digitales de la 1 a la 20", peti, resp);
    //Salidas digitales valen ahora [0 1 0 0 0 1 0 1 1 1 1 0 1 0 1 0 1 0 1 0]
  }
  {
    Mensaje peti({ 0x06, 0x0F, 0x00, 0x00, 0x00, 0x14, 0x03, 0xFF, 0xFF, 0x0F, 0xD0, 0x6B });
    Mensaje resp({ 0x06, 0x0F, 0x00, 0x00, 0x00, 0x14, 0x54, 0x73 });
    testea(mbs, "Formazos salidas digitales de la 1 a la 20 a los valores [1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1]", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x01, 0x00, 0x00, 0x00, 0x14, 0x3D, 0xB2 });
    Mensaje resp({ 0x06, 0x01, 0x03, 0xFF, 0xFF, 0x0F, 0x0C, 0x3D });
    testea(mbs, "Leemos las salidas digitales de la 1 a la 20", peti, resp);
    //Salidas digitales valen ahora [1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1]
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
