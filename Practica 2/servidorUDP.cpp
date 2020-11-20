//Ejemplo servidor eco UDP
//Autor: Alberto Hamilton Castro
// 2019-10-23

#include <iostream>     // std::cout, std::endl
#include <arpa/inet.h> // htons(), htonl(), etc.
#include <sys/socket.h> // socket(), bind(), etc.
#include <unistd.h>     // close()
#include <cstring>      // std::strerror()
#include <string>

#define BUFLEN 256  // Longitud del buffer
#define PORT 8888   // Puerto en que recibir datos

int main(void) {

  // creamos socket UDP
  int sfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (sfd == -1) {
    std::cerr << "Error al abrir el socket: " << std::strerror(errno)
        << std::endl;
    return 1;
  }

  struct sockaddr_in si_mio;
  si_mio.sin_family = AF_INET;
  si_mio.sin_port = htons(PORT);
  si_mio.sin_addr.s_addr = htonl(INADDR_ANY);  //cualquier dirección IP
  unsigned slen = sizeof(struct sockaddr_in);

  // unimos socket a puerto elegido
  if(bind(sfd, (struct sockaddr*)&si_mio, slen) == -1) {
    std::cerr << "Error al intentar bind(): " << std::strerror(errno)
        << std::endl;
    return 2;
  }

  int numEcos = 0;
  // bucle de espera de datos
  while(1) {
    std::cout << "Esperando datos ..." << std::flush;

    char buf[BUFLEN];
    struct sockaddr_in si_otro;
    unsigned int slen = sizeof(si_otro);
    int recv_len = recvfrom(sfd, buf, BUFLEN, 0,
        (struct sockaddr *) &si_otro, &slen);
    if ((recv_len) == -1) {
      std::string mensaje = "Error conectando al puerto: ";
      mensaje += std::strerror(errno);
      std::cerr << mensaje << std::endl;
      return 3;
    }

    std::cout << "Recibido paquete de " << recv_len << " bytes desde "
        << inet_ntoa(si_otro.sin_addr)
        << ":" << ntohs(si_otro.sin_port) << std::endl;
    std::cout << "Datos: " << buf << std::endl;

    numEcos++;
    std::string respuesta = "(Eco " + std::to_string(numEcos) + ") " + buf;

    // reenviamos respuesta al cliente
    int send_len = sendto(sfd, respuesta.c_str(), respuesta.size() + 1, 0,
        (struct sockaddr*) &si_otro, slen);
    if (send_len == -1) {
      std::string mensaje = "Error enviando respuesta: ";
      mensaje += std::strerror(errno);
      std::cerr << mensaje << std::endl;
      return 5;
    }
  }

  close(sfd);
  return 0;
}
