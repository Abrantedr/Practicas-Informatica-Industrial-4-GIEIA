// prueba de la clase mensaje

#define TITULO "Mensaje: probando aniadeCRC()"

#include "Mensaje.hpp"
#include <iostream>
#include <iomanip>

int pruebas = 0;
int fallos = 0;

void miraCorrecto(std::string tostring, std::string esperado) {
  pruebas++;
  if (tostring == esperado)
    std::cout << "\nCORRECTO toString() devuelve \n'" << tostring << "'"
        << std::endl;
  else
    std::cout << "\nFALLO " << ++fallos << ": se esperaba \n'"<< esperado
        << "' pero toString() devolvió \n'" << tostring << "'" << std::endl;
}

int main() {

  std::cout << "\n***  " << TITULO << "  ***\n" << std::endl;

  std::vector <std::string> totEsperado {
    "[(14) f0 57 75 20 4a 08 39 e6 d0 ca ee 66 63 c8 ]",
    "[(5) 19 67 ef cb bb ]",
    "[(12) 7b e9 98 c0 84 4e ea 2c d8 3c b1 11 ]",
    "[(7) 12 48 4a 77 02 0c 44 ]",
    "[(17) 25 fd 2a 97 70 1f 1c c0 1b 15 ff 53 f0 10 61 67 c2 ]",
    "[(15) d6 7a 55 31 03 c5 57 e1 35 b8 06 c5 7d 1f 13 ]",
    "[(14) 76 45 f5 d0 8d 95 5e f4 11 3c 03 cf d4 a1 ]",
    "[(13) 6d 42 b9 1b e2 81 d0 94 8c 96 5d 21 30 ]",
    "[(19) cc a0 ee 97 5a 0e 05 45 8b 36 49 f4 7c 91 15 43 fb 17 90 ]",
    "[(6) 99 d2 6f a5 63 0a ]",
    "[(2) ff ff ]",
  };

  std::size_t ea = 0;

  {
    Mensaje men({ 0xF0, 0x57, 0x75, 0x20, 0x4A, 0x08, 0x39, 0xE6, 0xD0, 0xCA, 0xEE, 0x66 });
    men.aniadeCRC();
    miraCorrecto(men.toString(), totEsperado[ea++]);
  }

  {
    Mensaje men({ 0x19, 0x67, 0xEF });
    men.aniadeCRC();
    miraCorrecto(men.toString(), totEsperado[ea++]);
  }

  {
    Mensaje men({ 0x7B, 0xE9, 0x98, 0xC0, 0x84, 0x4E, 0xEA, 0x2C, 0xD8, 0x3C });
    men.aniadeCRC();
    miraCorrecto(men.toString(), totEsperado[ea++]);
  }

  {
    Mensaje men({ 0x12, 0x48, 0x4A, 0x77, 0x02 });
    men.aniadeCRC();
    miraCorrecto(men.toString(), totEsperado[ea++]);
  }

  {
    Mensaje men({ 0x25, 0xFD, 0x2A, 0x97, 0x70, 0x1F, 0x1C, 0xC0, 0x1B, 0x15, 0xFF, 0x53, 0xF0, 0x10, 0x61 });
    men.aniadeCRC();
    miraCorrecto(men.toString(), totEsperado[ea++]);
  }

  {
    Mensaje men({ 0xD6, 0x7A, 0x55, 0x31, 0x03, 0xC5, 0x57, 0xE1, 0x35, 0xB8, 0x06, 0xC5, 0x7D });
    men.aniadeCRC();
    miraCorrecto(men.toString(), totEsperado[ea++]);
  }

  {
    Mensaje men({ 0x76, 0x45, 0xF5, 0xD0, 0x8D, 0x95, 0x5E, 0xF4, 0x11, 0x3C, 0x03, 0xCF });
    men.aniadeCRC();
    miraCorrecto(men.toString(), totEsperado[ea++]);
  }

  {
    Mensaje men({ 0x6D, 0x42, 0xB9, 0x1B, 0xE2, 0x81, 0xD0, 0x94, 0x8C, 0x96, 0x5D });
    men.aniadeCRC();
    miraCorrecto(men.toString(), totEsperado[ea++]);
  }

  {
    Mensaje men({ 0xCC, 0xA0, 0xEE, 0x97, 0x5A, 0x0E, 0x05, 0x45, 0x8B, 0x36, 0x49, 0xF4, 0x7C, 0x91, 0x15, 0x43, 0xFB });
    men.aniadeCRC();
    miraCorrecto(men.toString(), totEsperado[ea++]);
  }

  {
    Mensaje men({ 0x99, 0xD2, 0x6F, 0xA5 });
    men.aniadeCRC();
    miraCorrecto(men.toString(), totEsperado[ea++]);
  }

  {
    Mensaje men;
    men.aniadeCRC();
    miraCorrecto(men.toString(), totEsperado[ea++]);
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
