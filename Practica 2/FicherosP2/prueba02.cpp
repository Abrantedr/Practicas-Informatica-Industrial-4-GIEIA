// prueba de la clase mensaje

#define TITULO "Mensaje: Probando getByteAt() setByteAt() pushByte_back()"

#include "Mensaje.hpp"
#include <iostream>
#include <iomanip>

int pruebas = 0;
int fallos = 0;

void miraCorrectoGet(Mensaje& men, unsigned ind, uint8_t corr) {
  pruebas++;
  uint8_t dev = men.getByteAt(ind);
  if (dev == corr)
    std::cout << "CORRECTO getByteAt(" << ind <<") de " << men.toString()
        << " devuelve " << (int)dev << std::endl;
  else
    std::cout << "FALLO " << ++fallos << ": getByteAt(" << ind <<") de "
        << men.toString()
        << " devuelve " << (int)dev  << " cuando se esperaba "<< (int)corr
        << std::endl;
}


void miraCorrectoSet(Mensaje& men, unsigned ind, uint8_t val,
    std::string& esperado) {
  pruebas++;
  std::cout << "\nCambiando byte " << ind << " con valor " << (int)val
      << "(0x" << std::hex << (int)val << std::dec << ")\n";
  men.setByteAt(ind, val);
  std::string tostring = men.toString();
  if (tostring == esperado)
    std::cout << "CORRECTO toString() devuelve '" << tostring << "'"
        << std::endl;
  else
    std::cout << "FALLO " << ++fallos << ": se esperaba '"<< esperado
        << "' pero toString() devolvió '" << tostring << "'" << std::endl;
}


void miraCorrectoPush(Mensaje& men, uint8_t val, std::string esperado) {
  pruebas++;
  std::cout << "\nAñadiendo byte " << (int)val
      << "(0x" << std::hex << (int)val << std::dec << ")\n";
  men.pushByte_back(val);
  std::string tostring = men.toString();
  if (tostring == esperado)
    std::cout << "CORRECTO toString() devuelve \n'" << tostring << "'"
        << std::endl;
  else
    std::cout << "FALLO " << ++fallos << ": se esperaba \n'"<< esperado
        << "' pero toString() devolvió \n'" << tostring << "'" << std::endl;
}



int main() {

  std::cout << "\n***  " << TITULO << "  ***\n" << std::endl;


  {
    Mensaje men({ 0, 0 });
    miraCorrectoGet(men, 0, 0);
    miraCorrectoGet(men, 1, 0);
  }

  {
    Mensaje men({ 0x72, 0x9E, 0x1D, 0x33, 0x34, 0x51, 0x75, 0x09, 0x14 });
    miraCorrectoGet(men, 0, 114);
    miraCorrectoGet(men, 1, 158);
    miraCorrectoGet(men, 2, 29);
    miraCorrectoGet(men, 3, 51);
    miraCorrectoGet(men, 4, 52);
    miraCorrectoGet(men, 5, 81);
    miraCorrectoGet(men, 6, 117);
    miraCorrectoGet(men, 7, 9);
    miraCorrectoGet(men, 8, 20);
  }

  {
    Mensaje men({ 0x2B, 0x63, 0x14, 0x6E, 0x6C, 0x60, 0xD7 });
    miraCorrectoGet(men, 0, 43);
    miraCorrectoGet(men, 1, 99);
    miraCorrectoGet(men, 2, 20);
    miraCorrectoGet(men, 3, 110);
    miraCorrectoGet(men, 4, 108);
    miraCorrectoGet(men, 5, 96);
    miraCorrectoGet(men, 6, 215);
  }

  {
    Mensaje men({ 0x4D, 0xC8, 0xF1, 0xC6, 0x6E, 0xA2, 0x6A, 0x4A, 0xB8, 0xF3, 0x63, 0x09, 0x5E, 0x42, 0xD0 });
    miraCorrectoGet(men, 0, 77);
    miraCorrectoGet(men, 1, 200);
    miraCorrectoGet(men, 2, 241);
    miraCorrectoGet(men, 3, 198);
    miraCorrectoGet(men, 4, 110);
    miraCorrectoGet(men, 5, 162);
    miraCorrectoGet(men, 6, 106);
    miraCorrectoGet(men, 7, 74);
    miraCorrectoGet(men, 8, 184);
    miraCorrectoGet(men, 9, 243);
    miraCorrectoGet(men, 10, 99);
    miraCorrectoGet(men, 11, 9);
    miraCorrectoGet(men, 12, 94);
    miraCorrectoGet(men, 13, 66);
    miraCorrectoGet(men, 14, 208);
  }

  std::vector <std::string> totEsperado {
    "[(12) 00 57 75 20 4a 08 39 e6 d0 ca ee 66 ]",
    "[(12) 00 0e 75 20 4a 08 39 e6 d0 ca ee 66 ]",
    "[(12) 00 0e 75 20 4a 08 39 e6 d0 ca ee aa ]",
    "[(12) 00 0e 75 20 4a 08 39 e6 b3 ca ee aa ]",
    "[(12) 00 0e 75 0e 4a 08 39 e6 b3 ca ee aa ]",
    "[(12) 00 0e 80 0e 4a 08 39 e6 b3 ca ee aa ]",
    "[(8) 08 a4 a5 a9 42 21 b6 4f ]",
    "[(8) 08 a4 a5 a9 42 fe b6 4f ]",
    "[(8) 08 a4 a5 a9 55 fe b6 4f ]",
    "[(8) 08 a4 a5 a9 55 fe b6 22 ]",
    "[(8) 08 4d a5 a9 55 fe b6 22 ]",
    "[(8) 08 4d a5 c1 55 fe b6 22 ]",

    "[(2) 4a 00 ]",
    "[(3) 4a 00 7c ]",
    "[(4) 4a 00 7c ff ]",
    "[(5) 4a 00 7c ff f0 ]",
    "[(6) 4a 00 7c ff f0 0f ]",
    "[(7) 4a 00 7c ff f0 0f 55 ]",
    "[(4) 66 b6 4f 2a ]",
    "[(5) 66 b6 4f 2a 23 ]",
    "[(6) 66 b6 4f 2a 23 e5 ]",
    "[(7) 66 b6 4f 2a 23 e5 22 ]",
    "[(8) 66 b6 4f 2a 23 e5 22 53 ]",
    "[(9) 66 b6 4f 2a 23 e5 22 53 c7 ]",
  };
  unsigned ea = 0;

  {
    Mensaje men({ 0xF0, 0x57, 0x75, 0x20, 0x4A, 0x08, 0x39, 0xE6, 0xD0, 0xCA, 0xEE, 0x66 });
    std::cout << "Mensaje original:" << men.toString() << "\n";
    miraCorrectoSet(men, 0, 0, totEsperado[ea++]);
    miraCorrectoSet(men, 1, 14, totEsperado[ea++]);
    miraCorrectoSet(men, 11, 0xAA, totEsperado[ea++]);
    miraCorrectoSet(men, 8, 0xb3, totEsperado[ea++]);
    miraCorrectoSet(men, 3, 14, totEsperado[ea++]);
    miraCorrectoSet(men, 2, 128, totEsperado[ea++]);
  }


  {
    Mensaje men({ 0x08, 0xA4, 0xA5, 0xA9, 0x42, 0x66, 0xB6, 0x4F });
    std::cout << "Mensaje original:" << men.toString() << "\n";
    miraCorrectoSet(men, 5, 33, totEsperado[ea++]);
    miraCorrectoSet(men, 5, 254, totEsperado[ea++]);
    miraCorrectoSet(men, 4, 0x55, totEsperado[ea++]);
    miraCorrectoSet(men, 7, 0x22, totEsperado[ea++]);
    miraCorrectoSet(men, 1, 77, totEsperado[ea++]);
    miraCorrectoSet(men, 3, 193, totEsperado[ea++]);
  }


  {
    Mensaje men({ 0x4A });
    std::cout << "Mensaje original:" << men.toString() << "\n";
    miraCorrectoPush(men, 0, totEsperado[ea++]);
    miraCorrectoPush(men, 124, totEsperado[ea++]);
    miraCorrectoPush(men, 0xFF, totEsperado[ea++]);
    miraCorrectoPush(men, 0xF0, totEsperado[ea++]);
    miraCorrectoPush(men, 0xF, totEsperado[ea++]);
    miraCorrectoPush(men, 0x55, totEsperado[ea++]);
  }


  {
    Mensaje men({ 0x66, 0xB6, 0x4F });
    std::cout << "Mensaje original:" << men.toString() << "\n";
    miraCorrectoPush(men, 42, totEsperado[ea++]);
    miraCorrectoPush(men, 35, totEsperado[ea++]);
    miraCorrectoPush(men, 229, totEsperado[ea++]);
    miraCorrectoPush(men, 34, totEsperado[ea++]);
    miraCorrectoPush(men, 83, totEsperado[ea++]);
    miraCorrectoPush(men, 199, totEsperado[ea++]);
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
