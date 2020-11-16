// Programa de prueba de la clase ModbusServe

#define TITULO "ModbusRTU: entradas analógicas y digitales del 10 al 13"

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

  // Datos proceso
  uint16_t uid = getuid();
  uint16_t gid = getgid();
  uint16_t pid = getpid();
  uint16_t ppid = getppid();

  // Registros de entrada analógicos y digitales del 10 al 13 (ID 6)

  {
    Mensaje peti({ 0x06, 0x04, 0x00, 0x6C, 0x00, 0x01, 0xF0, 0x60 });
    Mensaje resp({ 0x06, 0x04, 0x02 });
    resp.pushWord_back(uid);
    resp.aniadeCRC();
    testea(mbs, "Leemos las entradas analogicas de la 10 a la 10", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x04, 0x00, 0x6D, 0x00, 0x01, 0xA1, 0xA0 });
    Mensaje resp({ 0x06, 0x04, 0x02 });
    resp.pushWord_back(gid);
    resp.aniadeCRC();
    testea(mbs, "Leemos las entradas analogicas de la 11 a la 11", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x04, 0x00, 0x6F, 0x00, 0x01, 0x00, 0x60 });
    Mensaje resp({ 0x06, 0x04, 0x02 });
    resp.pushWord_back(ppid);
    resp.aniadeCRC();
    testea(mbs, "Leemos las entradas analogicas de la 13 a la 13", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x04, 0x00, 0x6C, 0x00, 0x04, 0x30, 0x63 });
    Mensaje resp({ 0x06, 0x04, 0x08 });
    resp.pushWord_back(uid);
    resp.pushWord_back(gid);
    resp.pushWord_back(pid);
    resp.pushWord_back(ppid);
    resp.aniadeCRC();
    testea(mbs, "Leemos las entradas analogicas de la 10 a la 13", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x04, 0x00, 0x6E, 0x00, 0x02, 0x11, 0xA1 });
    Mensaje resp({ 0x06, 0x04, 0x04 });
    resp.pushWord_back(pid);
    resp.pushWord_back(ppid);
    resp.aniadeCRC();
    testea(mbs, "Leemos las entradas analogicas de la 12 a la 13", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x02, 0x00, 0x09, 0x00, 0x01, 0x68, 0x7F });
    Mensaje resp({ 0x06, 0x02, 0x01 });
    resp.pushByte_back(uid & 1);
    resp.aniadeCRC();
    testea(mbs, "Leemos las entradas digitales de la 10 a la 10", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x02, 0x00, 0x0A, 0x00, 0x01, 0x98, 0x7F });
    Mensaje resp({ 0x06, 0x02, 0x01 });
    resp.pushByte_back(gid & 1);
    resp.aniadeCRC();
    testea(mbs, "Leemos las entradas digitales de la 11 a la 11", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x02, 0x00, 0x0C, 0x00, 0x01, 0x78, 0x7E });
    Mensaje resp({ 0x06, 0x02, 0x01 });
    resp.pushByte_back(ppid & 1);
    resp.aniadeCRC();
    testea(mbs, "Leemos las entradas digitales de la 13 a la 13", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x02, 0x00, 0x09, 0x00, 0x04, 0xA8, 0x7C });
    Mensaje resp({ 0x06, 0x02, 0x01 });
    resp.pushByte_back((uid & 1) | ((gid & 1) << 1) | ((pid & 1) << 2) | ((ppid & 1) << 3));
    resp.aniadeCRC();
    testea(mbs, "Leemos las entradas digitales de la 10 a la 13", peti, resp);
  }
  {
    Mensaje peti({ 0x06, 0x02, 0x00, 0x0B, 0x00, 0x02, 0x89, 0xBE });
    Mensaje resp({ 0x06, 0x02, 0x01 });
    resp.pushByte_back((pid & 1) | ((ppid & 1) << 1));
    resp.aniadeCRC();
    testea(mbs, "Leemos las entradas digitales de la 12 a la 13", peti, resp);
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
