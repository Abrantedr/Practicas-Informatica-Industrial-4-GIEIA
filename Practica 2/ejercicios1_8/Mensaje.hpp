//
// Created by Rubén Abrante Delgado on 16/11/2020.
//

#ifndef _MENSAJE_
#define _MENSAJE_

#include <vector>
#include <cstdint>
#include <string>


// Representa un mensaje Modbus como una secuencia de bytes (uint8_t)
class Mensaje {

private:
  std::vector<uint8_t> _bytes;

  /*! Calcula el código CRC-16 de los primeros 'len' bytes
  * \param mensaje std::vector que contiene los bytes de trabajo
  * \param len numero de bytes considerados
  * \return los dos bytes del CRC en el orden "correcto"
  */
  uint16_t crc16(std::vector<uint8_t> mensaje, unsigned len);

  /*! Calcula el código CRC-16 de TODOS los bytes del std::vector
  * \param mensaje std::vector de bytes de trabajo
  * \return los dos bytes del CRC en el orden "correcto"
  */
  uint16_t crc16(std::vector<uint8_t> mensaje);

public:
  // Constructor por defecto crea un mensaje vacío.
  Mensaje();

  // Constructor que crea un mensaje con los bytes del vector pasado.
  Mensaje(std::vector<uint8_t> bytes);

  /* Devuelve representación del mensaje indicando el número de bytes y
  el valor de sus bytes en hexadecimal. Por ejemplo si el mensaje contiene
  los bytes 199, 106 y 112 el string devuelto debe ser '[(3) c7 6a 70 ]' */
  std::string toString() const;

  // Devuelve el número de bytes que tienen el mensaje en ese momento.
  unsigned size() const;

  // Devuelve el iterador al principio del mensaje
  std::vector<uint8_t>::const_iterator begin() const;

  // Devuelve el iterador al final del mensaje
  std::vector<uint8_t>::const_iterator end() const;

  // Borra el mensaje desde el principio hasta una posición dada
  void erase(std::vector<uint8_t>::const_iterator begin,
             std::vector<uint8_t>::const_iterator end);

  // Inserta parte de un mensaje en otro
  void insert(std::vector<uint8_t>::const_iterator beginRTU,
              std::vector<uint8_t>::const_iterator beginOriginal,
              std::vector<uint8_t>::const_iterator endOriginal);

  // Devuelve el byte que se encuentra en la posición ind del mensaje.
  uint8_t getByteAt(unsigned ind) const;

  // Coloca el byte dato en la posición ind.
  void setByteAt(unsigned ind, uint8_t dato);

  // Añade un doble byte dato al final del mensaje.
  void pushByte_back(uint8_t dato);

  // Indica si el CRC del mensaje es correcto
  bool crcOK();

  // añade el CRC al mensaje
  void aniadeCRC();

  // Devuelve el doble byte situado a partir de la posición ind del mensaje.
  uint16_t getWordAt(unsigned ind) const;

  // coloca el doble byte dato a partir de la posición ind.
  void setWordAt(unsigned ind, uint16_t dato);

  // Añade un doble byte dato al final del mensaje.
  void pushWord_back(uint16_t dato);
};

std::ostream& operator<<(std::ostream& os, Mensaje& mensaje);

#endif //_MENSAJE_
