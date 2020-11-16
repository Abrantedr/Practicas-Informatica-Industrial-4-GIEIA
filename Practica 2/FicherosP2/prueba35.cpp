// Programa de prueba de la clase ModbusServe

#define TITULO "ModbusRTU: entradas digitales del 4 al 20"

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

  // Datos proceso
  uint16_t uid = getuid();
  uint16_t gid = getgid();
  uint16_t pid = getpid();
  uint16_t ppid = getppid();

  {
    Mensaje peti({ 0x06, 0x02, 0x00, 0x03, 0x00, 0x08, 0x88, 0x7B });
    Mensaje resp({ 0x06, 0x02, 0x01 });
    resp.pushByte_back((anio & 1) | ((mes & 1) << 1) | ((dia & 1) << 2)
        | ((hora & 1) << 3) | ((min & 1) << 4) | ((seg & 1) << 5)
        | ((uid & 1) << 6) | ((gid & 1) << 7));
    resp.aniadeCRC();
    testea(mbs, "Leemos las entradas digitales de la 4 a la 11", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x02, 0x00, 0x03, 0x00, 0x0A, 0x09, 0xBA });
    Mensaje resp({ 0x06, 0x02, 0x02 });
    resp.pushByte_back((anio & 1) | ((mes & 1) << 1) | ((dia & 1) << 2)
        | ((hora & 1) << 3) | ((min & 1) << 4) | ((seg & 1) << 5)
        | ((uid & 1) << 6) | ((gid & 1) << 7));
    resp.pushByte_back((pid & 1) | ((ppid & 1) << 1));
    resp.aniadeCRC();
    testea(mbs, "Leemos las entradas digitales de la 4 a la 13", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x02, 0x00, 0x05, 0x00, 0x08, 0x68, 0x7A });
    Mensaje resp({ 0x06, 0x02, 0x01 });
    resp.pushByte_back((dia & 1) | ((hora & 1) << 1) | ((min & 1) << 2)
        | ((seg & 1) << 3) | ((uid & 1) << 4) | ((gid & 1) << 5)
        | ((pid & 1) << 6) | ((ppid & 1) << 7));
    resp.aniadeCRC();
    testea(mbs, "Leemos las entradas digitales de la 6 a la 13", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x02, 0x00, 0x09, 0x00, 0x09, 0x69, 0xB9 });
    Mensaje resp({ 0x06, 0x02, 0x02});
    resp.pushByte_back((uid & 1) | ((gid & 1) << 1) | ((pid & 1) << 2) | ((ppid & 1) << 3)
        | (1 << 5));
    resp.pushByte_back(0);
    resp.aniadeCRC();
    testea(mbs, "Leemos las entradas digitales de la 10 a la 18", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x02, 0x00, 0x09, 0x00, 0x0A, 0x29, 0xB8 });
    Mensaje resp({ 0x06, 0x02, 0x02});
    resp.pushByte_back((uid & 1) | ((gid & 1) << 1) | ((pid & 1) << 2) | ((ppid & 1) << 3)
        | (1 << 5));
    resp.pushByte_back(0);
    resp.aniadeCRC();
    testea(mbs, "Leemos las entradas digitales de la 10 a la 19", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x02, 0x00, 0x04, 0x00, 0x10, 0x39, 0xB0 });
    Mensaje resp({ 0x06, 0x02, 0x02 });
    resp.pushByte_back((mes & 1) | ((dia & 1) << 1) | ((hora & 1) << 2)
        | ((min & 1) << 3) | ((seg & 1) << 4) | ((uid & 1) << 5)
        | ((gid & 1) << 6) | ((pid & 1) << 7));
    resp.pushByte_back((ppid & 1) | (1 << 2));
    resp.aniadeCRC();
    testea(mbs, "Leemos las entradas digitales de la 5 a la 20", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x02, 0x00, 0x03, 0x00, 0x11, 0x49, 0xB1 });
    Mensaje resp({ 0x06, 0x02, 0x03 });
    resp.pushByte_back((anio & 1) | ((mes & 1) << 1) | ((dia & 1) << 2)
        | ((hora & 1) << 3) | ((min & 1) << 4) | ((seg & 1) << 5)
        | ((uid & 1) << 6) | ((gid & 1) << 7));
    resp.pushByte_back((pid & 1) | ((ppid & 1) << 1) | (1 << 3));
    resp.pushByte_back(0);
    resp.aniadeCRC();
    testea(mbs, "Leemos las entradas digitales de la 4 a la 20", peti, resp);
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
