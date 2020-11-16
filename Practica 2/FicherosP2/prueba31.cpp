// Programa de prueba de la clase ModbusServe

#define TITULO "ModbusRTU: contadores analógicos (0x04) y digitales (0x02)"

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

// Registros de entrada analógicos y digitales

  {
    Mensaje peti({ 0x36, 0x04, 0x00, 0x63, 0x00, 0x03, 0x44, 0x52 });
    Mensaje resp({ 0x36, 0x04, 0x06, 0x00, 0x01, 0x00, 0x06, 0x00, 0x00, 0xCF, 0x63 });
    testea(mbs, "Lectura contador peticiones 1 y bytes", peti, resp);
  }
  {
    Mensaje peti({ 0x36, 0x02, 0x00, 0x00, 0x00, 0x03, 0x3C, 0x4C });
    Mensaje resp({ 0x36, 0x02, 0x01, 0x04, 0xAE, 0x3F });
    testea(mbs, "Lectura reg digitales de entrada 2 y bytes", peti, resp);
  }
  {
    Mensaje peti({ 0x36, 0x04, 0x00, 0x63, 0x00, 0x03, 0x44, 0x52 });
    Mensaje resp({ 0x36, 0x04, 0x06, 0x00, 0x03, 0x00, 0x12, 0x00, 0x0D, 0x37, 0x62 });
    testea(mbs, "Lectura contador peticiones 3 y bytes", peti, resp);
  }
  {
    Mensaje peti({ 0x36, 0x02, 0x00, 0x00, 0x00, 0x03, 0x3C, 0x4C });
    Mensaje resp({ 0x36, 0x02, 0x01, 0x00, 0xAF, 0xFC });
    testea(mbs, "Lectura reg digitales de entrada 4 y bytes", peti, resp);
  }
  {
    Mensaje peti({ 0x36, 0x04, 0x00, 0x63, 0x00, 0x03, 0x44, 0x52 });
    Mensaje resp({ 0x36, 0x04, 0x06, 0x00, 0x05, 0x00, 0x1E, 0x00, 0x1A, 0x3F, 0x6F });
    testea(mbs, "Lectura contador peticiones 5 y bytes", peti, resp);
  }
  {
    Mensaje peti({ 0x36, 0x02, 0x00, 0x00, 0x00, 0x03, 0x3C, 0x4C });
    Mensaje resp({ 0x36, 0x02, 0x01, 0x04, 0xAE, 0x3F });
    testea(mbs, "Lectura reg digitales de entrada 6 y bytes", peti, resp);
  }
  {
    Mensaje peti({ 0x36, 0x04, 0x00, 0x63, 0x00, 0x03, 0x44, 0x52 });
    Mensaje resp({ 0x36, 0x04, 0x06, 0x00, 0x07, 0x00, 0x2A, 0x00, 0x27, 0xC6, 0xB0 });
    testea(mbs, "Lectura contador peticiones 7 y bytes", peti, resp);
  }
  {
    Mensaje peti({ 0x36, 0x02, 0x00, 0x00, 0x00, 0x03, 0x3C, 0x4C });
    Mensaje resp({ 0x36, 0x02, 0x01, 0x00, 0xAF, 0xFC });
    testea(mbs, "Lectura reg digitales de entrada 8 y bytes", peti, resp);
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
