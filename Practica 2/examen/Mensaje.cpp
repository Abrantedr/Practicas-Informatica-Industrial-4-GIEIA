// Autor: Rubén Abrante Delgado. Fecha: 02/12/2020

#include <iostream>
#include <iomanip>
#include "Mensaje.hpp"

#define DEFAULT_WIDTH (2)
#define DEFAULT_FILL ('0')


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


Mensaje::Mensaje() = default;


Mensaje::Mensaje(std::vector<uint8_t> bytes) : _bytes(bytes) {}


unsigned Mensaje::size() {
  return _bytes.size();
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


uint16_t Mensaje::getWordAt(unsigned ind) const {
  return (uint16_t)getByteAt(ind) << 8 | getByteAt(ind + 1);
}


void Mensaje::setWordAt(unsigned ind, uint16_t dato) {
  setByteAt(ind, dato >> 8);
  setByteAt(ind + 1, dato & 0xFF);
}


void Mensaje::pushWord_back(uint16_t dato) {
  pushByte_back(dato >> 8);
  pushByte_back(dato & 0xFF);
}


std::string Mensaje::toString() const {
  std::ostringstream os;
  os << "[(" << _bytes.size() << ") ";
  for (const unsigned &_byte : _bytes) {
    os << std::hex << std::setw(DEFAULT_WIDTH) << std::setfill(DEFAULT_FILL)
        << _byte << " ";
  }
  os << "]";
  return os.str();
}


bool Mensaje::crcOK() {
  if (crc16(_bytes)) // Si el CRC es distinto de cero
    return false;
  else
    return true;
}


void Mensaje::aniadeCRC() {
  if (!crcOK())
    pushWord_back(crc16(_bytes));
}


uint8_t Mensaje::getId() const {
  return getByteAt(0);
}


uint8_t Mensaje::getFuncion() const {
  return getByteAt(1);
}


uint16_t Mensaje::getOffset() const {
  return getWordAt(2);
}


std::ostream& operator<<(std::ostream& os, Mensaje &mensaje) {
  os << mensaje.toString();
  return os;
}


std::vector<uint8_t>::const_iterator Mensaje::begin() {
  return _bytes.begin();
}


std::vector<uint8_t>::const_iterator Mensaje::end() {
  return _bytes.end();
}


void Mensaje::insert(std::vector<uint8_t>::const_iterator begin,
    std::vector<uint8_t>::const_iterator beginOther,
    std::vector<uint8_t>::const_iterator endOther) {
  _bytes.insert(begin, beginOther, endOther);
}


void Mensaje::erase(std::vector<uint8_t>::const_iterator begin,
    std::vector<uint8_t>::const_iterator end) {
  _bytes.erase(begin, end);
}
