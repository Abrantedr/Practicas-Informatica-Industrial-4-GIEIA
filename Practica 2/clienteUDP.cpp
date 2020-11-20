//Ejemplo cliente eco UDP
//Autor: Alberto Hamilton Castro
// 2019-10-23

#include <iostream>     // std::cout, std::endl
#include <arpa/inet.h>  // htons(), htonl(), etc.
#include <sys/socket.h> // socket(), bind(), etc.
#include <unistd.h>     // close()

#define SERVER "127.0.0.1"
#define BUFLEN 256  // Longitud del buffer
#define PORT 8888   // Puerto al que enviar datos


int main(void)
{

  int s;
  if ( (s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
    std::cerr << "Error al abrir el socket: " << std::strerror(errno)
        << std::endl;
    return 1;
  }

  struct sockaddr_in si_otro;
  unsigned int slen = sizeof(si_otro);

  si_otro.sin_family = AF_INET;
  si_otro.sin_port = htons(PORT);

  if (inet_aton(SERVER , &si_otro.sin_addr) == 0)
  {
    std::cerr << "inet_aton() failed" << std::endl;
    return 2;
  }

  while(1) {
    std::string mensaje;
    std::cout << "\nIntroduce mensaje : ";
    std::getline(std::cin, mensaje);

    // enviamos mensaje
    if (sendto(s, mensaje.c_str(), mensaje.size()+1 , 0 ,
        (struct sockaddr *) &si_otro, slen) == -1) {
      std::string menError = "Error enviando mensaje: ";
      menError += std::strerror(errno);
      std::cerr << menError << std::endl;
      return 3;
    }

    // recibimos respuesta
    char buf[BUFLEN];
    int recv_len;
    if ((recv_len = recvfrom(s, buf, BUFLEN, 0,
        (struct sockaddr *) &si_otro, &slen)) == -1) {
      std::string menError = "Error recibiendo respuesta: ";
      menError += std::strerror(errno);
      std::cerr << menError << std::endl;
      return 4;
    }

    std::cout << "-> Recibido respuesta de " << recv_len << " bytes desde "
        << inet_ntoa(si_otro.sin_addr)
        << ":" << ntohs(si_otro.sin_port) << std::endl;
    std::cout << "-> Datos: " << buf << std::endl;
  }

  close(s);
  return 0;
}
