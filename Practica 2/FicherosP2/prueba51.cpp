// Programa de prueba de la clase ModbusServe

#define TITULO "ModbusTCP2: Escribimos digitales únicas y multiples (0x05 y 0x0F)"

#include <iostream>
#include <iomanip>
#include <vector>
#include <thread>
#include <chrono>
#include <cstring>      // std::strerror()

#include "Mensaje.hpp"
#include "ModbusTCP2.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>     // close()


int pruebas = 0;
int fallos = 0;

#define PUERTO 1502
#define ID1  33
#define ID2  150
#define SERVER "127.0.0.1"
#define BUFLEN 512  // Longitud del buffer

bool servFin = false;

void ejecutaTCP(unsigned numClientes) {

  ModbusTCP2 mbTCP(PUERTO, ID1, ID2);
  std::cout << "Creado servidor ModbusTCP" << std::endl;

  try {

    mbTCP.atiende(numClientes);

  } catch (std::runtime_error& e) {
    std::cerr << "Lanzada exception: " << e.what() << std::endl;
  }
  servFin = true;
}

void testea(int sfd, std::string titulo, Mensaje peticion,
    Mensaje respuestaOK) {

  pruebas++;

  std::cout << "\n\nTest '" << titulo << "' enviamos " << peticion.toString() << "\n";

  //metemos peticion en buffer
  uint8_t bufEnv[BUFLEN];
  for(unsigned int i=0; i<peticion.size(); i++)
    bufEnv[i] = peticion.getByteAt(i);

  int bsend = send(sfd, bufEnv, peticion.size(), 0);
  if(bsend == -1) {
    std::cerr << "Error al intentar send(): " << std::strerror(errno)
        << std::endl;
    return;
  }

  // recibimos respuesta
  uint8_t bufRec[BUFLEN];
  int recv_len;
  recv_len = recv(sfd, bufRec, BUFLEN, 0);
  if (recv_len == -1) {
    if((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
      std::cout << "FALLO "  << ++fallos << ": Timeout esperando por la respuesta"
          << std::endl;
    } else {
      std::cerr << "Error al recibir datos: " << std::strerror(errno)
          << std::endl;
    }
    return;
  } else if (recv_len == 0) {
    std::cout << "Servidor cerró comunicaión" << std::endl;
    return;
  }

  //std::cout << "-> Recibido respuesta de "
      //<< recv_len << " bytes" << std::endl;
  Mensaje respuesta;
  for(int i = 0; i < recv_len; i++)
    respuesta.pushByte_back(bufRec[i]);
  //std::cout << "-> Datos: " << respuesta.toString() << std::endl;

  if(respuesta.toString() == respuestaOK.toString()) {
    std::cout << "CORRECTO: la respuesta fue " << respuesta.toString() << std::endl;
  } else {
    std::cout << "FALLO "  << ++fallos << ": la respuesta debería ser "
        << respuestaOK.toString() << " pero es " << respuesta.toString() << std::endl;
  }
}

int main (int argc, char *argv[]) {

  std::cout << "\n***  " << TITULO << "  ***\n" << std::endl;

  std::thread hiloModbusTCP(ejecutaTCP, 1);

  std::this_thread::sleep_for(std::chrono::seconds(1));

  //creamos socket TCP
  int sfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sfd == -1) {
    std::cerr << "Error al abrir el socket: " << std::strerror(errno)
        << std::endl;
    return 200;
  }

  // Ponemos timeout de receptción
  struct timeval tv;
  tv.tv_sec = 1;
  tv.tv_usec = 0;
  setsockopt(sfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

  struct sockaddr_in si_otro;
  unsigned slen = sizeof(struct sockaddr_in);

  si_otro.sin_family = AF_INET;
  si_otro.sin_port = htons(PUERTO);
  inet_aton(SERVER , &si_otro.sin_addr);

  //Conectamos al servidor
  int conn = connect(sfd, (struct sockaddr *) &si_otro, slen);
  if( conn == -1 ) {
    std::cerr << "Error al conectar con servidor: " << std::strerror(errno)
        << std::endl;
    close(sfd);
    hiloModbusTCP.join();
    return 201;
  }

  {
    Mensaje peti({ 0x00, 0x0C, 0x00, 0x00, 0x00, 0x06, ID1, 0x05, 0x00, 0x00, 0x00, 0x00 });
    Mensaje resp({ 0x00, 0x0C, 0x00, 0x00, 0x00, 0x06, ID1, 0x05, 0x00, 0x00, 0x00, 0x00 });
    testea(sfd, "Forzamos salida digital unica 1 a valor 0", peti, resp);
  }
  {
    Mensaje peti({ 0x00, 0x0C, 0x00, 0x00, 0x00, 0x06, ID2, 0x05, 0x00, 0x00, 0x00, 0x00 });
    Mensaje resp({ 0x00, 0x0C, 0x00, 0x00, 0x00, 0x06, ID2, 0x05, 0x00, 0x00, 0x00, 0x00 });
    testea(sfd, "Forzamos salida digital unica 1 a valor 0", peti, resp);
  }
  {
    Mensaje peti({ 0x00, 0x0D, 0x00, 0x00, 0x00, 0x06, ID1, 0x01, 0x00, 0x00, 0x00, 0x14 });
    Mensaje resp({ 0x00, 0x0D, 0x00, 0x00, 0x00, 0x06, ID1, 0x01, 0x03, 0x54, 0x55, 0x05 });
    testea(sfd, "Leemos las salidas digitales de la 1 a la 20", peti, resp);
    //Salidas digitales valen ahora [0 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0]
  }
  {
    Mensaje peti({ 0x00, 0x0C, 0x00, 0x00, 0x00, 0x06, ID2, 0x05, 0x00, 0x00, 0x00, 0x00 });
    Mensaje resp({ 0x00, 0x0C, 0x00, 0x00, 0x00, 0x06, ID2, 0x05, 0x00, 0x00, 0x00, 0x00 });
    testea(sfd, "Forzamos salida digital unica 1 a valor 0", peti, resp);
  }
  {
    Mensaje peti({ 0x00, 0x0E, 0x00, 0x00, 0x00, 0x06, ID1, 0x05, 0x00, 0x01, 0xFF, 0x00 });
    Mensaje resp({ 0x00, 0x0E, 0x00, 0x00, 0x00, 0x06, ID1, 0x05, 0x00, 0x01, 0xFF, 0x00 });
    testea(sfd, "Forzamos salida digital unica 2 a valor 1", peti, resp);
  }
  {
    Mensaje peti({ 0x00, 0x0C, 0x00, 0x00, 0x00, 0x06, ID2, 0x05, 0x00, 0x00, 0x00, 0x00 });
    Mensaje resp({ 0x00, 0x0C, 0x00, 0x00, 0x00, 0x06, ID2, 0x05, 0x00, 0x00, 0x00, 0x00 });
    testea(sfd, "Forzamos salida digital unica 1 a valor 0", peti, resp);
  }
  {
    Mensaje peti({ 0x00, 0x0F, 0x00, 0x00, 0x00, 0x06, ID1, 0x01, 0x00, 0x00, 0x00, 0x14 });
    Mensaje resp({ 0x00, 0x0F, 0x00, 0x00, 0x00, 0x06, ID1, 0x01, 0x03, 0x56, 0x55, 0x05 });
    testea(sfd, "Leemos las salidas digitales de la 1 a la 20", peti, resp);
    //Salidas digitales valen ahora [0 1 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0]
  }
  {
    Mensaje peti({ 0x00, 0x0E, 0x00, 0x00, 0x00, 0x06, ID2, 0x05, 0x00, 0x01, 0xFF, 0x00 });
    Mensaje resp({ 0x00, 0x0E, 0x00, 0x00, 0x00, 0x06, ID2, 0x05, 0x00, 0x01, 0xFF, 0x00 });
    testea(sfd, "Forzamos salida digital unica 2 a valor 1", peti, resp);
  }
  {
    Mensaje peti({ 0x00, 0x10, 0x00, 0x00, 0x00, 0x06, ID1, 0x05, 0x00, 0x09, 0xFF, 0x00 });
    Mensaje resp({ 0x00, 0x10, 0x00, 0x00, 0x00, 0x06, ID1, 0x05, 0x00, 0x09, 0xFF, 0x00 });
    testea(sfd, "Forzamos salida digital unica 10 a valor 1", peti, resp);
  }
  {
    Mensaje peti({ 0x00, 0x11, 0x00, 0x00, 0x00, 0x06, ID1, 0x01, 0x00, 0x00, 0x00, 0x14 });
    Mensaje resp({ 0x00, 0x11, 0x00, 0x00, 0x00, 0x06, ID1, 0x01, 0x03, 0x56, 0x57, 0x05 });
    testea(sfd, "Leemos las salidas digitales de la 1 a la 20", peti, resp);
    //Salidas digitales valen ahora [0 1 1 0 1 0 1 0 1 1 1 0 1 0 1 0 1 0 1 0]
  }
  {
    Mensaje peti({ 0x00, 0x0F, 0x00, 0x00, 0x00, 0x06, ID2, 0x01, 0x00, 0x00, 0x00, 0x14 });
    Mensaje resp({ 0x00, 0x0F, 0x00, 0x00, 0x00, 0x06, ID2, 0x01, 0x03, 0x56, 0x55, 0x05 });
    testea(sfd, "Leemos las salidas digitales de la 1 a la 20", peti, resp);
    //Salidas digitales valen ahora [0 1 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0]
  }
  {
    Mensaje peti({ 0x00, 0x10, 0x00, 0x00, 0x00, 0x06, ID2, 0x05, 0x00, 0x09, 0xFF, 0x00 });
    Mensaje resp({ 0x00, 0x10, 0x00, 0x00, 0x00, 0x06, ID2, 0x05, 0x00, 0x09, 0xFF, 0x00 });
    testea(sfd, "Forzamos salida digital unica 10 a valor 1", peti, resp);
  }
  {
    Mensaje peti({ 0x00, 0x11, 0x00, 0x00, 0x00, 0x06, ID2, 0x01, 0x00, 0x00, 0x00, 0x14 });
    Mensaje resp({ 0x00, 0x11, 0x00, 0x00, 0x00, 0x06, ID2, 0x01, 0x03, 0x56, 0x57, 0x05 });
    testea(sfd, "Leemos las salidas digitales de la 1 a la 20", peti, resp);
    //Salidas digitales valen ahora [0 1 1 0 1 0 1 0 1 1 1 0 1 0 1 0 1 0 1 0]
  }
  {
    Mensaje peti({ 0x00, 0x12, 0x00, 0x00, 0x00, 0x06, ID1, 0x05, 0x00, 0x04, 0x00, 0x00 });
    Mensaje resp({ 0x00, 0x12, 0x00, 0x00, 0x00, 0x06, ID1, 0x05, 0x00, 0x04, 0x00, 0x00 });
    testea(sfd, "Forzamos salida digital unica 5 a valor 0", peti, resp);
  }
  {
    Mensaje peti({ 0x00, 0x13, 0x00, 0x00, 0x00, 0x06, ID1, 0x01, 0x00, 0x00, 0x00, 0x14 });
    Mensaje resp({ 0x00, 0x13, 0x00, 0x00, 0x00, 0x06, ID1, 0x01, 0x03, 0x46, 0x57, 0x05 });
    testea(sfd, "Leemos las salidas digitales de la 1 a la 20", peti, resp);
    //Salidas digitales valen ahora [0 1 1 0 0 0 1 0 1 1 1 0 1 0 1 0 1 0 1 0]
  }
  {
    Mensaje peti({ 0x00, 0x12, 0x00, 0x00, 0x00, 0x06, ID2, 0x05, 0x00, 0x04, 0x00, 0x00 });
    Mensaje resp({ 0x00, 0x12, 0x00, 0x00, 0x00, 0x06, ID2, 0x05, 0x00, 0x04, 0x00, 0x00 });
    testea(sfd, "Forzamos salida digital unica 5 a valor 0", peti, resp);
  }
  {
    Mensaje peti({ 0x00, 0x14, 0x00, 0x00, 0x00, 0x06, ID1, 0x05, 0x00, 0x13, 0xFF, 0x00 });
    Mensaje resp({ 0x00, 0x14, 0x00, 0x00, 0x00, 0x06, ID1, 0x05, 0x00, 0x13, 0xFF, 0x00 });
    testea(sfd, "Forzamos salida digital unica 20 a valor 1", peti, resp);
  }
  {
    Mensaje peti({ 0x00, 0x13, 0x00, 0x00, 0x00, 0x06, ID2, 0x01, 0x00, 0x00, 0x00, 0x14 });
    Mensaje resp({ 0x00, 0x13, 0x00, 0x00, 0x00, 0x06, ID2, 0x01, 0x03, 0x46, 0x57, 0x05 });
    testea(sfd, "Leemos las salidas digitales de la 1 a la 20", peti, resp);
    //Salidas digitales valen ahora [0 1 1 0 0 0 1 0 1 1 1 0 1 0 1 0 1 0 1 0]
  }
  {
    Mensaje peti({ 0x00, 0x15, 0x00, 0x00, 0x00, 0x06, ID1, 0x01, 0x00, 0x00, 0x00, 0x14 });
    Mensaje resp({ 0x00, 0x15, 0x00, 0x00, 0x00, 0x06, ID1, 0x01, 0x03, 0x46, 0x57, 0x0D });
    testea(sfd, "Leemos las salidas digitales de la 1 a la 20", peti, resp);
    //Salidas digitales valen ahora [0 1 1 0 0 0 1 0 1 1 1 0 1 0 1 0 1 0 1 1]
  }
  {
    Mensaje peti({ 0x00, 0x14, 0x00, 0x00, 0x00, 0x06, ID2, 0x05, 0x00, 0x13, 0xFF, 0x00 });
    Mensaje resp({ 0x00, 0x14, 0x00, 0x00, 0x00, 0x06, ID2, 0x05, 0x00, 0x13, 0xFF, 0x00 });
    testea(sfd, "Forzamos salida digital unica 20 a valor 1", peti, resp);
  }
  {
    Mensaje peti({ 0x00, 0x16, 0x00, 0x00, 0x00, 0x06, ID1, 0x05, 0x00, 0x01, 0xFF, 0x00 });
    Mensaje resp({ 0x00, 0x16, 0x00, 0x00, 0x00, 0x06, ID1, 0x05, 0x00, 0x01, 0xFF, 0x00 });
    testea(sfd, "Dejamos salida digital unica 2 a valor 1", peti, resp);
  }
  {
    Mensaje peti({ 0x00, 0x17, 0x00, 0x00, 0x00, 0x06, ID1, 0x01, 0x00, 0x00, 0x00, 0x14 });
    Mensaje resp({ 0x00, 0x17, 0x00, 0x00, 0x00, 0x06, ID1, 0x01, 0x03, 0x46, 0x57, 0x0D });
    testea(sfd, "Leemos las salidas digitales de la 1 a la 20", peti, resp);
    //Salidas digitales valen ahora [0 1 1 0 0 0 1 0 1 1 1 0 1 0 1 0 1 0 1 1]
  }
  {
    Mensaje peti({ 0x00, 0x18, 0x00, 0x00, 0x00, 0x06, ID1, 0x05, 0x00, 0x04, 0x00, 0x00 });
    Mensaje resp({ 0x00, 0x18, 0x00, 0x00, 0x00, 0x06, ID1, 0x05, 0x00, 0x04, 0x00, 0x00 });
    testea(sfd, "Dejamos salida digital unica 5 a valor 0", peti, resp);
  }
  {
    Mensaje peti({ 0x00, 0x19, 0x00, 0x00, 0x00, 0x06, ID1, 0x01, 0x00, 0x00, 0x00, 0x14 });
    Mensaje resp({ 0x00, 0x19, 0x00, 0x00, 0x00, 0x06, ID1, 0x01, 0x03, 0x46, 0x57, 0x0D });
    testea(sfd, "Leemos las salidas digitales de la 1 a la 20", peti, resp);
    //Salidas digitales valen ahora [0 1 1 0 0 0 1 0 1 1 1 0 1 0 1 0 1 0 1 1]
  }
  {
    Mensaje peti({ 0x00, 0x1A, 0x00, 0x00, 0x00, 0x06, ID1, 0x05, 0x00, 0x13, 0xFF, 0x00 });
    Mensaje resp({ 0x00, 0x1A, 0x00, 0x00, 0x00, 0x06, ID1, 0x05, 0x00, 0x13, 0xFF, 0x00 });
    testea(sfd, "Dejamos salida digital unica 20 a valor 1", peti, resp);
  }

  close(sfd);
  std::this_thread::sleep_for(std::chrono::seconds(1));
  if (servFin)  hiloModbusTCP.join();
  else {
    std::cout << "FALLO "  << ++fallos << ": el servidor no terminó tras 1 cliente"
        << std::endl;
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
