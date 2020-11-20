//Ejemplo servidor eco UDP. Sin control de errores
//Autor: Alberto Hamilton Castro
// 2019-10-23

#include <iostream>     // std::cout, std::endl
#include <arpa/inet.h>  // htons(), htonl(), etc.
#include <sys/socket.h> // socket(), bind(), etc.
#include <unistd.h>     // close()

#define BUFLEN 256  // Longitud del buffer
#define PORT 8888   // Puerto en que recibir datos

int main(void) {

  // creamos socket UDP
  int sfd = socket(AF_INET, SOCK_DGRAM, 0);

  struct sockaddr_in si_mio;
  si_mio.sin_family = AF_INET;
  si_mio.sin_port = htons(PORT);
  si_mio.sin_addr.s_addr = htonl(INADDR_ANY);  //cualquier dirección IP
  unsigned int slen = sizeof(struct sockaddr_in);

  // unimos socket al puerto elegido
  bind(sfd, (struct sockaddr*)&si_mio, slen);

  // bucle de espera de datos
  while(1) {
    std::cout << "Esperando datos ..." << std::flush;

    char buf[BUFLEN];
    struct sockaddr_in si_otro;
    int recv_len = recvfrom(sfd, buf, BUFLEN, 0,
        (struct sockaddr *) &si_otro, &slen);

    //informamos de lo recibido
    std::cout << "Recibido paquete de "
        << recv_len << " bytes desde "
        << inet_ntoa(si_otro.sin_addr)
        << ":" << ntohs(si_otro.sin_port)
        << std::endl;
    std::cout << "Datos: " << buf << std::endl;

    // reenviamos al cliente los mismos datos
    sendto(sfd, buf, recv_len, 0,
        (struct sockaddr*) &si_otro, slen);
  }

  close(sfd);
  return 0;
}
