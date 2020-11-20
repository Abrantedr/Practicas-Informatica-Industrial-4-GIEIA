//Ejemplo cliente eco UDP. Sin control de errores.
//Autor: Alberto Hamilton Castro
// 2019-10-23


#include <iostream>     // std::cout, std::endl
#include <arpa/inet.h>  // htons(), htonl(), etc.
#include <sys/socket.h> // socket(), bind(), etc.
#include <unistd.h>     // close()

#define SERVER "127.0.0.1"
#define BUFLEN 256  // Longitud del buffer
#define PORT 8888   // Puerto al que enviar datos

// SIN control de errores
int main(void) {

  //creamos socket TCP
  int sfd = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in si_otro;
  unsigned slen = sizeof(struct sockaddr_in);

  si_otro.sin_family = AF_INET;
  si_otro.sin_port = htons(PORT);
  inet_aton(SERVER , &si_otro.sin_addr);

  //Conectamos al servidor
  connect(sfd, (struct sockaddr *) &si_otro, slen);
  std::cout << "Conexión establecida" << std::endl;
  while(1) {
    std::string mensaje;
    std::cout << "\nIntroduce mensaje : ";
    std::getline(std::cin, mensaje);
    // enviamos mensaje
    send(sfd, mensaje.c_str(), mensaje.size()+1 , 0);

    // recibimos respuesta
    char buf[BUFLEN];
    int recv_len = recv(sfd, buf, BUFLEN, 0);

    std::cout << "-> Recibido respuesta de "
        << recv_len << " bytes" << std::endl;
    std::cout << "-> Datos: " << buf << std::endl;
  }
  close(sfd);
  return 0;
}
