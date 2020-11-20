//Ejemplo cliente eco UDP. Sin control de errores.
//Autor: Alberto Hamilton Castro
// 2019-10-23


#include <iostream>     // std::cout, std::endl
#include <arpa/inet.h>  // htons(), htonl(), etc.
#include <sys/socket.h> // socket(), sendto(), etc.
#include <unistd.h>     // close()

#define SERVER "127.0.0.1"
#define BUFLEN 256  // Longitud del buffer
#define PORT 8888   // Puerto al que enviar datos

//SIN control de errores
int main(void) {

  int s = socket(AF_INET, SOCK_DGRAM, 0);

  struct sockaddr_in si_otro;
  unsigned slen = sizeof(si_otro);

  si_otro.sin_family = AF_INET;
  si_otro.sin_port = htons(PORT);

  inet_aton(SERVER , &si_otro.sin_addr);

  while(1) {
    std::string mensaje;
    std::cout << "Introduce mensaje : ";
    std::getline(std::cin, mensaje);

    // enviamos mensaje
    sendto(s, mensaje.c_str(), mensaje.size()+1,
        0, (struct sockaddr *) &si_otro, slen);

    // recibimos respuesta
    char buf[BUFLEN];
    int recv_len = recvfrom(s, buf, BUFLEN, 0,
        (struct sockaddr *) &si_otro, &slen);

    std::cout << "-> Recibido respuesta de "
        << recv_len << " bytes desde "
        << inet_ntoa(si_otro.sin_addr)
        << ":" << ntohs(si_otro.sin_port) << std::endl;
    std::cout << "-> Datos: " << buf << std::endl;
  }

  close(s);
  return 0;
}
