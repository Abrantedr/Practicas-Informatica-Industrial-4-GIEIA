// Autor: Rubén Abrante Delgado. Fecha: 02/12/2020

#include <iostream>     // std::cout, std::endl
#include <arpa/inet.h>  // htons(), htonl(), etc.
#include <sys/socket.h> // socket(), bind(), etc.
#include <unistd.h>     // close()
#include "ModbusTCP.hpp"

#define BUFLEN 512  // Longitud del buffer
#define PORT 8888   // Puerto en que recibir datos

ModbusTCP::ModbusTCP(uint16_t puerto, uint8_t unitId)
    : _puerto(puerto), _mbusRTU(unitId) {}


void ModbusTCP::atiende() {

  // creamos socket TCP para escuchar
  int sfdl = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in si_mio;
  si_mio.sin_family = AF_INET;
  si_mio.sin_port = htons(_puerto);
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
    
    if (recv_len == 0) {
      std::cout << "Cliente cerró comunicación" << std::endl;
      break; // esperamos por siguiente cliente
    }

    std::cout << "Recibidos " << recv_len << " bytes" << std::endl;
    
    Mensaje mensajeTCP;
    for (unsigned i = 0; i < recv_len; i++) {
      mensajeTCP.pushByte_back((uint8_t)buf[i]);
    }
    
    std::cerr << "El mensaje TCP es " << mensajeTCP << std::endl;
    // Hacemos una copia del mensaje TCP
    Mensaje mensajeRTU = mensajeTCP;
    // Eliminamos la cabecera del TCP
    mensajeRTU.erase(mensajeRTU.begin(), mensajeRTU.begin() + 6);
    mensajeRTU.aniadeCRC(); // Añadimos el CRC
    
    std::cerr << "El mensaje RTU es " << mensajeRTU << std::endl;
    
    Mensaje respuestaRTU = _mbusRTU.peticion(mensajeRTU);
    
    std::cerr << "La respuesta RTU es " << respuestaRTU << std::endl;
    
    // Le quitamos el CRC a la respuesta
    respuestaRTU.erase(respuestaRTU.end() - 2, respuestaRTU.end());
    // Insertamos la cabecera
    respuestaRTU.insert(respuestaRTU.begin(), mensajeTCP.begin(),
        mensajeTCP.begin() + 6);
    // Colocamos el tamaño correcto
    respuestaRTU.setWordAt(4, respuestaRTU.size() - 6);
    
    std::cerr << "La respuesta TCP es " << respuestaRTU << std::endl;
    
    for (unsigned i = 0; i < respuestaRTU.size(); i++) {
      buf[i] = (char)respuestaRTU.getByteAt(i);
    }
    
    // Reenviamos al cliente los datos
    send(sfd, buf, respuestaRTU.size(), 0);
  }
  close(sfd);
}
