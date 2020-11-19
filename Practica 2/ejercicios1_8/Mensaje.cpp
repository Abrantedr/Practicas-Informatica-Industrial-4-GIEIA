//
// Created by Rubén Abrante Delgado on 16/11/2020.
//

#include <iostream>
#include <iomanip>
#include "Mensaje.hpp"


Mensaje::Mensaje() = default;

Mensaje::Mensaje(std::vector<uint8_t> bytes) : _bytes(bytes) {}

std::string Mensaje::toString() const {
  std::ostringstream os;
  os << "[(" << _bytes.size() << ") ";
  for (unsigned _byte : _bytes) {
    os << std::hex << std::setw(2) << std::setfill('0')
       << _byte << " ";
  }
  os << "]";
  return os.str();
}

unsigned Mensaje::size() const {
  return _bytes.size();
}

std::vector<uint8_t>::const_iterator Mensaje::begin() const {
  return _bytes.begin();
}

std::vector<uint8_t>::const_iterator Mensaje::end() const {
  return _bytes.end();
}

void Mensaje::erase(std::vector<uint8_t>::const_iterator begin,
                    std::vector<uint8_t>::const_iterator end) {
  _bytes.erase(begin, end);
}

void Mensaje::insert(std::vector<uint8_t>::const_iterator beginRTU,
                     std::vector<uint8_t>::const_iterator beginOriginal,
                     std::vector<uint8_t>::const_iterator endOriginal) {
  _bytes.insert(beginRTU, beginOriginal, endOriginal);
}

uint8_t Mensaje::getByteAt(unsigned ind) const {
  return _bytes.at(ind);
}

void Mensaje::setByteAt(unsigned ind, uint8_t dato) {
  _bytes.at(ind) = dato;
}

void Mensaje::pushByte_back(uint8_t dato) {
  _bytes.push_back(dato);
}

bool Mensaje::crcOK() {
  if (crc16(_bytes)) // Si el crc es distinto de cero
    return false; // No contiene el crc
  else
    return true;
}

void Mensaje::aniadeCRC() {
  if (!crcOK()) { // Si no tiene crc
    uint16_t crc = crc16(_bytes);
    pushWord_back(crc);
  }
}

uint16_t Mensaje::getWordAt(unsigned ind) const {
  return (uint16_t)getByteAt(ind) << 8 | getByteAt(ind + 1);
}

void Mensaje::setWordAt(unsigned ind, uint16_t dato) {
  setByteAt(ind, dato >> 8); // msb
  setByteAt(ind + 1, dato & 0xFF); // lsb
}

void Mensaje::pushWord_back(uint16_t dato) {
  pushByte_back(dato >> 8); // msb
  pushByte_back(dato & 0xFF); // lsb
}

uint16_t Mensaje::crc16(std::vector<uint8_t> mensaje, unsigned len) {
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

uint16_t Mensaje::crc16(std::vector<uint8_t> mensaje) {
  return crc16(mensaje, mensaje.size());
}

std::ostream& operator<<(std::ostream& os, Mensaje& mensaje) {
  os << mensaje.toString();
  return os;
}
