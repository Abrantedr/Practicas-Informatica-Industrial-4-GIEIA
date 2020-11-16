// Programa de prueba de la clase ModbusServe

#define TITULO "ModbusRTU: entradas analógicas y digitales del 4 al 9"

#include <iostream>
#include <iomanip>
#include <vector>

#include "Mensaje.hpp"
#include "ModbusRTU.hpp"

#include <sys/types.h>
#include <unistd.h>

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

  // fecha y hora
  std::time_t now_time = std::time(nullptr);
  std::tm* now = std::localtime(&now_time);
  uint16_t anio = now->tm_year + 1900;
  uint16_t mes = now->tm_mon + 1;
  uint16_t dia = now->tm_mday;
  uint16_t hora = now->tm_hour;
  uint16_t min = now->tm_min;
  uint16_t seg = now->tm_sec;

  {
    Mensaje peti({ 0x06, 0x04, 0x00, 0x66, 0x00, 0x01, 0xD0, 0x62 });
    Mensaje resp({ 0x06, 0x04, 0x02 });
    resp.pushWord_back(anio);
    resp.aniadeCRC();
    testea(mbs, "Leemos las entradas analogicas de la 4 a la 4", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x04, 0x00, 0x67, 0x00, 0x01, 0x81, 0xA2 });
    Mensaje resp({ 0x06, 0x04, 0x02 });
    resp.pushWord_back(mes);
    resp.aniadeCRC();
    testea(mbs, "Leemos las entradas analogicas de la 5 a la 5", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x04, 0x00, 0x69, 0x00, 0x01, 0xE0, 0x61 });
    Mensaje resp({ 0x06, 0x04, 0x02 });
    resp.pushWord_back(hora);
    resp.aniadeCRC();
    testea(mbs, "Leemos las entradas analogicas de la 7 a la 7", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x04, 0x00, 0x66, 0x00, 0x06, 0x91, 0xA0 });
    Mensaje resp({ 0x06, 0x04, 0x0C });
    resp.pushWord_back(anio);
    resp.pushWord_back(mes);
    resp.pushWord_back(dia);
    resp.pushWord_back(hora);
    resp.pushWord_back(min);
    resp.pushWord_back(seg);
    resp.aniadeCRC();
    testea(mbs, "Leemos las entradas analogicas de la 4 a la 9", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x04, 0x00, 0x67, 0x00, 0x04, 0x41, 0xA1 });
    Mensaje resp({ 0x06, 0x04, 0x08 });
    resp.pushWord_back(mes);
    resp.pushWord_back(dia);
    resp.pushWord_back(hora);
    resp.pushWord_back(min);
    resp.aniadeCRC();
    testea(mbs, "Leemos las entradas analogicas de la 5 a la 8", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x04, 0x00, 0x69, 0x00, 0x03, 0x61, 0xA0 });
    Mensaje resp({ 0x06, 0x04, 0x06 });
    resp.pushWord_back(hora);
    resp.pushWord_back(min);
    resp.pushWord_back(seg);
    resp.aniadeCRC();
    testea(mbs, "Leemos las entradas analogicas de la 7 a la 9", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x02, 0x00, 0x03, 0x00, 0x01, 0x48, 0x7D });
    Mensaje resp({ 0x06, 0x02, 0x01 });
    resp.pushByte_back(anio & 1);
    resp.aniadeCRC();
    testea(mbs, "Leemos las entradas digitales de la 4 a la 4", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x02, 0x00, 0x04, 0x00, 0x01, 0xF9, 0xBC });
    Mensaje resp({ 0x06, 0x02, 0x01 });
    resp.pushByte_back(mes & 1);
    resp.aniadeCRC();
    testea(mbs, "Leemos las entradas digitales de la 5 a la 5", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x02, 0x00, 0x06, 0x00, 0x01, 0x58, 0x7C });
    Mensaje resp({ 0x06, 0x02, 0x01 });
    resp.pushByte_back(hora & 1);
    resp.aniadeCRC();
    testea(mbs, "Leemos las entradas digitales de la 7 a la 7", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x02, 0x00, 0x03, 0x00, 0x06, 0x09, 0xBF });
    Mensaje resp({ 0x06, 0x02, 0x01 });
    resp.pushByte_back((anio & 1) | ((mes & 1) << 1) | ((dia & 1) << 2)
        | ((hora & 1) << 3) | ((min & 1) << 4) | ((seg & 1) << 5));
    resp.aniadeCRC();
    testea(mbs, "Leemos las entradas digitales de la 4 a la 9", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x02, 0x00, 0x04, 0x00, 0x04, 0x39, 0xBF });
    Mensaje resp({ 0x06, 0x02, 0x01 });
    resp.pushByte_back( ((mes & 1) << 0) | ((dia & 1) << 1)
        | ((hora & 1) << 2) | ((min & 1) << 3));
    resp.aniadeCRC();
    testea(mbs, "Leemos las entradas digitales de la 5 a la 8", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x02, 0x00, 0x06, 0x00, 0x03, 0xD9, 0xBD });
    Mensaje resp({ 0x06, 0x02, 0x01 });
    resp.pushByte_back(((hora & 1) << 0) | ((min & 1) << 1) | ((seg & 1) << 2));
    resp.aniadeCRC();
    testea(mbs, "Leemos las entradas digitales de la 7 a la 9", peti, resp);
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
