//Ejemplo cliente eco TCP. Con control de errores.
//Autor: Alberto Hamilton Castro
// 2019-10-23


#include <iostream>     // std::cout, std::endl
#include <arpa/inet.h>  // htons(), htonl(), etc.
#include <sys/socket.h> // socket(), bind(), etc.
#include <unistd.h>     // close()
#include <cstring>      // std::strerror()

#define SERVER "127.0.0.1"
#define BUFLEN 256  // Longitud del buffer
#define PORT 8888   // Puerto al que enviar datos

int main(void) {

  //creamos socket TCP
  int sfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sfd == -1) {
    std::cerr << "Error al abrir el socket: " << std::strerror(errno)
        << std::endl;
    return 1;
  }

  struct sockaddr_in si_otro;
  unsigned slen = sizeof(struct sockaddr_in);

  si_otro.sin_family = AF_INET;
  si_otro.sin_port = htons(PORT);
  inet_aton(SERVER , &si_otro.sin_addr);

  //Conectamos al servidor
  int conn = connect(sfd, (struct sockaddr *) &si_otro, slen);
  if( conn == -1 ) {
    std::cerr << "Error al conectar con servidor: " << std::strerror(errno)
        << std::endl;
    return 2;
  }

  std::cout << "Conexión establecida" << std::endl;
  while(1) {
    std::string mensaje;
    std::cout << "\nIntroduce mensaje: ";
    std::getline(std::cin, mensaje);
    if (!std::cin.good()) break;  // Si se pulsa <ctrl>+d

    // enviamos mensaje
    int bsend = send(sfd, mensaje.c_str(), mensaje.size()+1 , 0);
    if(bsend == -1) {
      std::cerr << "Error al intentar send(): " << std::strerror(errno)
          << std::endl;
      break;
    }
    std::cout << "Se enviaro " << bsend << " bytes" << std::endl;

    // recibimos respuesta
    char buf[BUFLEN];
    int recv_len;
    recv_len = recv(sfd, buf, BUFLEN, 0);
    if (recv_len == -1) {
      std::cerr << "Error al recibir datos: " << std::strerror(errno)
          << std::endl;
      return 3;
    } else if (recv_len == 0) {
      std::cout << "Servidor cerró comunicaión" << std::endl;
      break;
    }

    std::cout << "-> Recibido respuesta de "
        << recv_len << " bytes" << std::endl;
    std::cout << "-> Datos: " << buf << std::endl;
  }
  std::cout << "\nCerramos conexión y salimos\n";
  close(sfd);
  return 0;
}
