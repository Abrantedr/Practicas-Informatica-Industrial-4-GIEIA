//Ejemplo servidor eco TCP.
//Espera varios clientes sucesivamente.
//Autor: Alberto Hamilton Castro
// 2019-10-23

#include <iostream>     // std::cout, std::endl
#include <arpa/inet.h>  // htons(), htonl(), etc.
#include <sys/socket.h> // socket(), bind(), etc.
#include <unistd.h>     // close()
#include <cstring>      // std::strerror()

#define BUFLEN 256  // Longitud del buffer
#define PORT 8888   // Puerto en que recibir datos

int main(void) {

  // creamos socket TCP para escuchar
  int sfdl = socket(AF_INET, SOCK_STREAM, 0);
  if (sfdl == -1) {
    std::cerr << "Error al abrir el socket: " << std::strerror(errno)
        << std::endl;
    return 1;
  }

  struct sockaddr_in si_mio;
  si_mio.sin_family = AF_INET;
  si_mio.sin_port = htons(PORT);
  //cualquier dirección IP
  si_mio.sin_addr.s_addr = htonl(INADDR_ANY);
  unsigned slen = sizeof(struct sockaddr_in);

  // unimos socket a puerto elegido
  if(bind(sfdl, (struct sockaddr*)&si_mio, slen) == -1) {
    std::cerr << "Error al intentar bind(): " << std::strerror(errno)
        << std::endl;
    return 2;
  }

  //activamos la escucha para 1 cliente
  if(listen(sfdl, 1) == -1) {
    std::cerr << "Error al intentar listen(): " << std::strerror(errno)
        << std::endl;
    return 3;
  }

  int numEcos = 0;
  //esperamos que acceda un cliente
  while (1) {
    std::cout << "\n\nEsperando cliente" << std::endl;

    struct sockaddr_in si_otro;
    int sfd = accept(sfdl,
        (struct sockaddr *) &si_otro, &slen);

    if (sfd == -1) {
      std::cerr << "Error en el accept: " << std::strerror(errno) << std::endl;
      return 4;
    }
    std::cout << "Conectado cliente desde "
        << inet_ntoa(si_otro.sin_addr)
        << ":" << ntohs(si_otro.sin_port) << std::endl;

    // bucle de espera de datos
    while(1) {
      std::cout << "Esperando datos ..." << std::flush;

      char buf[BUFLEN];
      int recv_len = recv(sfd, buf, BUFLEN, 0);
      if (recv_len == -1) {
        std::cerr << "Error al recibir datos: " << std::strerror(errno)
            << std::endl;
        return 5;
      } else if (recv_len == 0) {
        std::cout << "Cliente cerró comunicación" << std::endl;
        break; // esperamos por siguiente cliente
      }

      std::cout << "Recibidos " << recv_len
          << " bytes" << std::endl;
      std::cout << "Datos: " << buf << std::endl;

      numEcos++;
      std::string respuesta = "(Eco " + std::to_string(numEcos) + ") " + buf;

      // reenviamos respuesta al cliente
      int send_len = send(sfd, respuesta.c_str(), respuesta.size() + 1, 0);
      if (send_len == -1) {
        std::string mensaje = "Error enviando respuesta: ";
        mensaje += std::strerror(errno);
        std::cerr << mensaje << std::endl;
        return 5;
      }
    }
    int cl = close(sfd); // cerramos comunicación con cliente
    if (cl == -1) {
      std::cerr << "Error al cerrar con cliente: " << std::strerror(errno)
          << std::endl;
    }
  }
  close(sfdl);
  return 0;
}
