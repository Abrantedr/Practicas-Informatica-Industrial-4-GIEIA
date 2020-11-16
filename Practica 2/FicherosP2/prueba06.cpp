// prueba de la clase mensaje

#define TITULO "Mensaje: probando crcOK()"

#include "Mensaje.hpp"
#include <iostream>
#include <iomanip>

int pruebas = 0;
int fallos = 0;

void miraCorrecto(Mensaje men, bool dev, bool corr) {
  pruebas++;
  if (dev == corr)
    std::cout << "\nCORRECTO crcOK() de " << men.toString() << " devuelve "
        << dev << std::endl;
  else
    std::cout << "\nFALLO " << ++fallos << ": crcOK() de " << men.toString()
        << " devuelve " << dev  << " cuando se esperaba "<< corr << std::endl;
}

int main() {

  std::cout << "\n***  " << TITULO << "  ***\n" << std::endl;


  {
    Mensaje men({ 0x35 });
    miraCorrecto(men, men.crcOK(), false);
  }

  {
    Mensaje men({ 0x5C, 0x08, 0x39, 0x76 });
    miraCorrecto(men, men.crcOK(), true);
  }

  {
    Mensaje men({ 0xCE, 0x4C, 0x19, 0xE5, 0x35 });
    miraCorrecto(men, men.crcOK(), true);
  }

  {
    Mensaje men({ 0x10, 0x96, 0x5A, 0x80, 0x12, 0x56, 0x1B, 0x98, 0x7A, 0x01, 0x65, 0x33, 0xB0, 0xEA, 0x01 });
    miraCorrecto(men, men.crcOK(), true);
  }

  {
    Mensaje men({ 0xFD, 0x4D, 0x78, 0x44, 0xC4, 0xDA, 0xB2, 0x16 });
    miraCorrecto(men, men.crcOK(), true);
  }

  {
    Mensaje men({ 0xC2, 0x3D, 0xB9, 0x40, 0xDE, 0x30 });
    miraCorrecto(men, men.crcOK(), true);
  }

  {
    Mensaje men({ 0x4F, 0x72, 0x22, 0x5D, 0x29, 0x5C, 0xBC, 0xA1, 0x85, 0xF6, 0x2F, 0x4B });
    miraCorrecto(men, men.crcOK(), false);
  }

  {
    Mensaje men({ 0x29, 0xF1, 0xB7, 0xCE, 0x70, 0x37, 0x6A, 0x64, 0x87, 0x46, 0xBB, 0x39, 0x03, 0x25, 0x28, 0x95, 0xE0 });
    miraCorrecto(men, men.crcOK(), false);
  }

  {
    Mensaje men({ 0xEC, 0x45, 0xB3, 0xC5, 0xAA });
    miraCorrecto(men, men.crcOK(), false);
  }

  {
    Mensaje men({ 0x50, 0xAF, 0x29, 0x36, 0x11, 0xF0, 0x64, 0x55, 0x3A, 0x30 });
    miraCorrecto(men, men.crcOK(), true);
  }

  {
    Mensaje men({ 0x48, 0x86, 0x70, 0x72, 0xFD, 0x09, 0xBC });
    miraCorrecto(men, men.crcOK(), true);
  }

  {
    Mensaje men({ 0x2D, 0xC9, 0xA6, 0x40, 0x7F, 0x5E, 0x9A, 0x51, 0x5E, 0xF4, 0xB4, 0x1E, 0x4C, 0xD1, 0xA8, 0x18, 0x35 });
    miraCorrecto(men, men.crcOK(), false);
  }

  {
    Mensaje men;
    miraCorrecto(men, men.crcOK(), false);
  }


  // Resumen final
  if (fallos == 0)
    std::cout << "\n :-) Todas las " << pruebas << " pruebas correctas\n"
        << std::endl;
  else
    std::cout << "\n :-( Se han producido " << fallos << " FALLOs de "
        << pruebas << " pruebas (" << std::fixed <<  std::setprecision(0)
        << fallos*100.0/pruebas << "%)\n"
        << std::endl;
  return fallos;
}
