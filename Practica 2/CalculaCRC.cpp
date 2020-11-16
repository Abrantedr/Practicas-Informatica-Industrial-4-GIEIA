/**
 * Programa que calcula CRC de cadenas de bytes
 * pasados como parámetro de entrada.
 * Se consideran bytes escritos en hexadecimal.
 *
 * \author Alberto Hamilton Castro
 *
 * Ejemplos de invocación:
 *    ./CalculaCRC 4 1 0 b 0 5
 *    ./CalculaCRC 04 01 00 0b 00 05
*/

#include <vector>
#include <iostream>
#include <iomanip>

/*! Calcula el código CRC-16 de los primeros 'len' bytes
 * \param mensaje std::vector que contiene los bytes de trabajo
 * \param len numero de bytes considerados
 * \return los dos bytes del CRC en el orden "correcto"
*/
uint16_t crc16(std::vector<uint8_t> mensaje, unsigned len) {
  uint16_t crc = 0xFFFF;

  for(unsigned ba = 0; ba < len; ba++) {
    crc ^= mensaje.at(ba);
    for(unsigned i = 0; i < 8; i++) {
      if(crc & 0x0001) {
        crc = (crc >> 1) ^ 0xA001;
      } else {
        crc >>= 1;
      }
    }
  }
  // pasamos a big-endian
  return (crc >> 8) | ((crc & 0xff) << 8);
}

/*! Calcula el código CRC-16 de TODOS los bytes del std::vector
 * \param mensaje std::vector de bytes de trabajo
 * \return los dos bytes del CRC en el orden "correcto"
 */
uint16_t crc16(std::vector<uint8_t> mensaje) {
  return crc16(mensaje, mensaje.size());
}


int main (int argc, char *argv[]) {

  //parseamos la linea de comandos
  int numBytes = argc - 1; //el primer argumento es el nombre del programa

  if(numBytes < 1) {
    std::cerr << "Necesario al menos un parámetro de entrada" <<std::endl;
    return 1;
  }

  std::vector<uint8_t> bytes;
  char* p;
  for(int pa = 0; pa < numBytes; pa++) {
    unsigned long int val = std::strtoul(argv[pa+1], &p, 16);
    if(*p != 0) {
      std::cout << "Argunmento '" << argv[pa+1]
          << "' no parece número hexadecimal" <<std::endl;
      return 2;
    }
    if (val > 255) {
      std::cout << "Argunmento '" << argv[pa+1]
          << "' no cabe en un uint8_t" <<std::endl;
      return 3;
    }
    bytes.push_back((uint8_t)val);
  }

  //invocamos la función
  uint16_t crc = crc16(bytes);

  std::cout << "El CRC para " << std::setfill('0') << std::hex;
  for (int ba = 0; ba < numBytes; ba++)
      std::cout << std::setw(2) << (int)bytes[ba] << ' ';
  std::cout << "es " << std::setw(4)  << crc
      << std::dec <<std::endl;

  return 0;
}
