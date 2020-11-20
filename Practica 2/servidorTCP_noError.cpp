//Ejemplo servidor eco TCP. Sin control de errores
//Autor: Alberto Hamilton Castro
// 2019-10-23

#include <iostream>     // std::cout, std::endl
#include <arpa/inet.h>  // htons(), htonl(), etc.
#include <sys/socket.h> // socket(), bind(), etc.
#include <unistd.h>     // close()

#define BUFLEN 256  // Longitud del buffer
#define PORT 8888   // Puerto en que recibir datos

int main(void) {

  // creamos socket TCP para escuchar
  int sfdl = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in si_mio;
  si_mio.sin_family = AF_INET;
  si_mio.sin_port = htons(PORT);
  //cualquier dirección IP
  si_mio.sin_addr.s_addr = htonl(INADDR_ANY);
  unsigned slen = sizeof(struct sockaddr_in);

  // unimos socket a puerto elegido
  bind(sfdl, (struct sockaddr*)&si_mio, slen);

  //activamos la escucha para 1 cliente
  listen(sfdl, 1);
  //esperamos que acceda un cliente
  std::cout << "Esperando cliente" << std::endl;
  struct sockaddr_in si_otro;
  int sfd = accept(sfdl,
      (struct sockaddr *) &si_otro, &slen);
  std::cout << "Conectado cliente desde "
      << inet_ntoa(si_otro.sin_addr)
      << ":" << ntohs(si_otro.sin_port) << std::endl;

  // bucle de espera de datos
  while(1) {
    std::cout << "Esperando datos ..." << std::flush;

    char buf[BUFLEN];
    int recv_len;
    recv_len = recv(sfd, buf, BUFLEN, 0);

    std::cout << "Recibidos " << recv_len
        << " bytes" << std::endl;
    std::cout << "Datos: " << buf << std::endl;

    // reenviamos al cliente los mismos datos
    send(sfd, buf, recv_len, 0);
  }

  close(sfd);
  return 0;
}
