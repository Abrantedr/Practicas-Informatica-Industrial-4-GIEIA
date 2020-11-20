//
// Created by Rubén Abrante Delgado on 20/11/2020.
// Works only in GNU/Linux Systems

#ifdef __unix__

#include <unistd.h>     // close()
#include <arpa/inet.h>  // htons(), htonl(), etc.
#include <sys/socket.h> // socket(), bind(), etc.


#elif defined(_WIN64) || defined(WIN64)

#define OS_Windows
// Placeholder
#endif

#define BUFLEN 512  // Longitud del buffer

#include <iostream> // std::cout, std::endl
#include <cstring> // std::strerror()
#include <string>
#include <thread>
#include <chrono>
#include "ModbusTCP2Multiple.hpp"


ModbusTCP2Multiple::ModbusTCP2Multiple(uint16_t puerto, uint8_t uId1, uint8_t uId2)
    : _puerto(puerto), _mbusRTU1(uId1), _mbusRTU2(uId2), _uId1(uId1),
    _uId2(uId2) {}


#ifdef __unix__
void ModbusTCP2::atiende(unsigned numClientes) {

  // creamos socket TCP para escuchar
  int sfdl = socket(AF_INET, SOCK_STREAM, 0);
  if (sfdl == -1) {
    std::string mensaje = "Error abriendo socket: ";
    mensaje += std::strerror(errno);
    std::cerr << mensaje << std::endl;
    throw std::runtime_error(mensaje);
  }

  struct sockaddr_in si_mio;
  si_mio.sin_family = AF_INET;
  si_mio.sin_port = htons(_puerto);
  //cualquier dirección IP
  si_mio.sin_addr.s_addr = htonl(INADDR_ANY);
  unsigned slen = sizeof(struct sockaddr_in);

  // unimos socket a puerto elegido
  if(bind(sfdl, (struct sockaddr*)&si_mio, slen) == -1) {
    std::string mensaje = "Error al intentar bind(): ";
    mensaje += std::strerror(errno);
    std::cerr << mensaje << std::endl;
    throw std::runtime_error(mensaje);
  }

  //activamos la escucha para 1 cliente
  if(listen(sfdl, numClientes) == -1) {
    std::string mensaje = "Error al intentar listen(): ";
    mensaje += std::strerror(errno);
    std::cerr << mensaje << std::endl;
    throw std::runtime_error(mensaje);
  }

  unsigned clienteActual = 0;
  //esperamos que acceda un cliente

  std::vector<std::thread*> vecThPt;

  while (clienteActual < numClientes) {
    std::cout << "\n\nEsperando cliente" << std::endl;

    struct sockaddr_in si_otro;
    int sfd = accept(sfdl,
                     (struct sockaddr *) &si_otro, &slen);

    if (sfd == -1) {
      std::string mensaje = "Error en el accept: ";
      mensaje += std::strerror(errno);
      std::cerr << mensaje << std::endl;
      throw std::runtime_error(mensaje);
    }

    std::thread* ptTh = new std::thread(&ModbusTCP2Multiple::atiendeCliente,
                                        this, newsockfd, cli_addr);
    vecThPt.push_back(ptTh);

    clienteActual++;
  }
  std::cerr << "Atendidos los " << numClientes << " clientes." << std::endl;

  for (auto thAct: vecThPt) {
    thAct->join();
    delete thAct;
  }

  std::cerr << "Terminaron todos los hilos." << std::endl;

  close(sfdl);
}


void ModbusTCP2Multiple::atiendeCliente(int sfd, struct sockaddr_in si_otro) {

  std::string cliente;
  cliente = cliente + inet_ntoa(cli_addr.sin_addr) + ":"
      + std::to_string(cli_addr.sin_port);

  std::cerr << "\t\t>>" << cliente << " Recibida conexion" << std::endl;

  // bucle de espera de datos
  while(1) {
    std::cout << "Esperando datos ..." << std::flush;

    char buf[BUFLEN];
    int recv_len = recv(sfd, buf, BUFLEN, MSG_PEEK);
    // Copiamos el contenido del mensaje en buf sin
    // eliminarlo de la cola de entrada

    // Tratamos de leer la cabecera del mensaje
      uint8_t msb_len = buf[4];
      uint8_t lsb_len = buf[5];
      uint16_t tcp_len = (uint16_t)msb_len << 8 | lsb_len;
      if (recv_len != (7 + tcp_len)) // cabecera tcp + tamaño resto
        // del mensaje + ' \0'
        recv_len = recv(sfd, buf, 6 + tcp_len, 0); // Lee solamente
        // la cabecera tcp + el resto del tamaño del mensaje
    if (recv_len == -1) {
      std::string mensaje = "Error al recibir datos: ";
      mensaje += std::strerror(errno);
      std::cerr << mensaje << std::endl;
      throw std::runtime_error(mensaje);
    } else if (recv_len == 0) {
      std::cout << "Cliente cerró comunicación" << std::endl;
      break; // esperamos por siguiente cliente
    }

    std::cout << "Recibidos " << recv_len << " bytes" << std::endl;

    Mensaje mensajeOriginal;
    // Rellena el mensaje original a partir del buffer
    for (int i = 0; i < recv_len; i++) {
      mensajeOriginal.pushByte_back((uint8_t)buf[i]);
    }

    std::cout << "El mensaje recibido es " << mensajeOriginal
              << std::endl;

    Mensaje mensajeRTU = mensajeOriginal; // Copiamos el original

    // Quitamos los 6 primeros caracteres
    mensajeRTU.erase(mensajeRTU.begin(), mensajeRTU.begin() + 6);
    mensajeRTU.aniadeCRC(); // CRC requerido para Modbus/RTU

    std::cout << "El mensaje RTU es " << mensajeRTU << std::endl;

    uint8_t uId = mensajeRTU.getByteAt(0);

    Mensaje respuestaRTU;
    if (uId == _uId1) {
      std::cerr << "\t\t>>" << cliente << " quiere acceder unidad 1"
          << std::endl;
      mutexU1.lock();
      std::cerr << "\t\t>>" << cliente << " accede unidad 1" << std::endl;
      respuestaRTU = _mbusRTU1.peticion(mensajeRTU);
      std::this_thread::sleep_for(std::chrono::seconds(2));
      mutexU1.unlock();
      std::cerr << "\t\t>>" << cliente << " termina unidad 1" << std::endl;
    } else if (uId == _uId2) {
      std::cerr << "\t\t>>" << cliente << " quiere acceder unidad 2"
          << std::endl;
      mutexU2.lock();
      std::cerr << "\t\t>>" << cliente << " accede unidad 2" << std::endl;
      respuestaRTU = _mbusRTU2.peticion(mensajeRTU);
      std::this_thread::sleep_for(std::chrono::seconds(2));
      mutexU2.unlock();
      std::cerr << "\t\t>>" << cliente << " termina unidad 2" << std::endl;
    }

    std::cout << "Respuesta del RTU: " << respuestaRTU << std::endl;

    respuestaRTU.insert(respuestaRTU.begin(), mensajeOriginal.begin(),
                        mensajeOriginal.begin() + 6);
    respuestaRTU.erase(respuestaRTU.end() - 2, respuestaRTU.end());
    respuestaRTU.setWordAt(4, respuestaRTU.size() - 6);

    std::cout << "Respuesta: " << respuestaRTU << std::endl;

    for (unsigned int i = 0; i < respuestaRTU.size(); ++i) {
      buf[i] = (char)respuestaRTU.getByteAt(i);
    }

    // reenviamos respuesta al cliente
    int send_len = send(sfd, buf, respuestaRTU.size(), 0);
    if (send_len == -1) {
      std::string mensaje = "Error enviando respuesta: ";
      mensaje += std::strerror(errno);
      std::cerr << mensaje << std::endl;
      throw std::runtime_error(mensaje);
    }
  }
  int cl = close(sfd); // cerramos comunicación con cliente
  if (cl == -1) {
    std::cerr << "Error al cerrar con cliente: "
        << std::strerror(errno) << std::endl;
  }
}

#endif
