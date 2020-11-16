// Programa de prueba de la clase ModbusServe

#define TITULO "ModbusTCP: Excepciones puerto restringido"

#include <iostream>
#include <iomanip>
#include <vector>
#include <thread>
#include <chrono>
#include <cstring>      // std::strerror()

#include "Mensaje.hpp"
#include "ModbusTCP.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>     // close()


int pruebas = 0;
int fallos = 0;

#define PUERTO 1502
#define ID_DEV  33
#define SERVER "127.0.0.1"
#define BUFLEN 512  // Longitud del buffer


int main (int argc, char *argv[]) {

  std::cout << "\n***  " << TITULO << "  ***\n" << std::endl;


  try {

    ModbusTCP mbTCP(502, ID_DEV);
    std::cout << "Creado servidor ModbusTCP" << std::endl;
    mbTCP.atiende(0);

    std::cout << "FALLO "  << ++fallos << ": NO lanzó excepción: " << std::endl;

  } catch (std::runtime_error& e) {
    std::cout << "CORRECTO: lanzó excepción " << e.what() << std::endl;
  } catch (std::exception& e) {
    std::cout << "FALLO "  << ++fallos << ": NO lanzó excepción del tipo 'runtime_error' " << std::endl;
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
