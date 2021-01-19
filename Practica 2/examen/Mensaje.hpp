// Autor: Rubén Abrante Delgado. Fecha: 02/12/2020

//
// Clase que implementa un mensaje con conjunto de bytes
//

#ifndef _MENSAJE_
#define _MENSAJE_

#include <cstdint>
#include <vector>
#include <string>

class Mensaje {

private:
  std::vector<uint8_t> _bytes;
  
  /*! Calcula el código CRC-16 de los primeros 'len' bytes
  * \param mensaje std::vector que contiene los bytes de trabajo
  * \param len numero de bytes considerados
  * \return los dos bytes del CRC en el orden "correcto" */
  uint16_t crc16(std::vector<uint8_t> mensaje, unsigned len);
  
  /*! Calcula el código CRC-16 de TODOS los bytes del std::vector
  * \param mensaje std::vector de bytes de trabajo
  * \return los dos bytes del CRC en el orden "correcto" */
  uint16_t crc16(std::vector<uint8_t> mensaje);

public:
  // Constructor por defecto crea un mensaje vacío
  Mensaje();

  // Constructor que crea un mensaje con los bytes del vector pasado
  Mensaje(std::vector<uint8_t> bytes);

  // Devuelve el número de bytes que tienen el mensaje en ese momento
  unsigned size();

  // Devuelve el byte que se encuentra en la posición ind del mensaje
  uint8_t getByteAt(unsigned ind) const;

  // Coloca el byte dato en la posición ind
  void setByteAt(unsigned ind, uint8_t dato);

  // Añade un doble byte dato al final del mensaje
  void pushByte_back(uint8_t dato);

  // Devuelve el doble byte situado a partir de la posición ind del mensaje
  uint16_t getWordAt(unsigned ind) const;

  // Coloca el doble byte dato a partir de la posición ind
  void setWordAt(unsigned ind, uint16_t dato);

  // Añade un doble byte dato al final del mensaje
  void pushWord_back(uint16_t dato);

  // Devuelve representación del mensaje:
  // número de bytes y el valor de sus bytes en hexadecimal
  std::string toString() const;

  // Indica si el CRC del mensaje es correcto
  bool crcOK();

  // Añade el CRC al mensaje
  void aniadeCRC();
  
  // Devuelve el id del mensaje
  uint8_t getId() const;
  
  // Devuelve la función del mensaje
  uint8_t getFuncion() const;
  
  // Devuelve el offset del mensaje
  uint16_t getOffset() const;
  
  // Devuelve un iterador al principio del mensaje
  std::vector<uint8_t>::const_iterator begin();
  
  // Devuelve un iterador al final del mensaje
  std::vector<uint8_t>::const_iterator end();
  
  // Inserta parte de un mensaje en otro
  void insert(std::vector<uint8_t>::const_iterator begin,
      std::vector<uint8_t>::const_iterator beginOther,
      std::vector<uint8_t>::const_iterator endOther);
  
  // Borra parte de un mensaje a partir de una posición dada
  void erase(std::vector<uint8_t>::const_iterator begin,
      std::vector<uint8_t>::const_iterator end);
};

// Vuelca por std::ostream lo mismo que toString()
std::ostream& operator<<(std::ostream& os, Mensaje &mensaje);

#endif
