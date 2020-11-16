//
// Created by Rubén Abrante Delgado on 16/11/2020.
//

#include "Mensaje.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>

Mensaje::Mensaje() = default;

Mensaje::Mensaje(std::vector<uint8_t> bytes) : _bytes(bytes) {
  // Pasa por valor (std::move() no es requerido)
}

std::string Mensaje::toString() {
  std::ostringstream os;
  os << "[(" << _bytes.size() << ") ";
  for (unsigned _byte : _bytes) {
    os << std::hex << std::setw(2) << std::setfill('0')
        << _byte << " ";
  }
  os << "]";
  return os.str();
}

unsigned Mensaje::size() {
  return _bytes.size();
}

uint8_t Mensaje::getByteAt(unsigned ind) {
  // Aunque ind sea negativo o haga overflow a unsigned int
  // siempre es capturado como std::out_of_range
  uint8_t byteAt;
  try {
    byteAt = _bytes.at(ind);
  } catch (std::out_of_range& e) {
    std::cerr << "Indice fuera de rango. Motivo: " << e.what()
        << std::endl;
  }
  return byteAt;
}

void Mensaje::setByteAt(unsigned ind, uint8_t dato) {
  // Nada previene al dato hacer underflow u overflow a uint8_t
  try {
    _bytes.at(ind) = dato;
  } catch (std::out_of_range& e) {
    std::cerr << "Indice fuera de rango. Motivo: " << e.what()
        << std::endl;
  }
}

void Mensaje::pushByte_back(uint8_t dato) {
  // Nada previene al dato hacer underflow u overflow a uint8_t
  _bytes.push_back(dato);
}

bool Mensaje::crcOK() {
  return false;
}

void Mensaje::aniadeCRC() {

}

uint16_t Mensaje::getWordAt(unsigned ind) {
  return (uint16_t)getByteAt(ind) << 8 | getByteAt(ind + 1);
}

void Mensaje::setWordAt(unsigned ind, uint16_t dato) {
  // Big-Endian
  setByteAt(ind, dato >> 8); // msb
  setByteAt(ind + 1, dato & 0xFF); // lsb
}

void Mensaje::pushWord_back(uint16_t dato) {
  // Big-Endian
  pushByte_back(dato >> 8); // msb
  pushByte_back(dato & 0xFF); // lsb
}
