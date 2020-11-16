// prueba de la clase mensaje

#define TITULO "Mensaje: Probando getWordAt()"

#include "Mensaje.hpp"
#include <iostream>
#include <iomanip>

int pruebas = 0;
int fallos = 0;

void miraCorrecto(Mensaje men, unsigned ind, uint16_t dev, uint16_t corr) {
  pruebas++;
  if (dev == corr)
    std::cout << "\nCORRECTO getWordAt(" << ind <<") de " << men.toString()
        << " devuelve " << dev << std::endl;
  else
    std::cout << "\nFALLO " << ++fallos << ": getWordAt(" << ind <<") de "
        << men.toString()
        << " devuelve " << dev  << " cuando se esperaba "<< corr << std::endl;
}

int main() {

  std::cout << "\n***  " << TITULO << "  ***\n" << std::endl;


  {
    Mensaje men({ 0, 0 });
    miraCorrecto(men, 0, men.getWordAt(0), 0);
  }

  {
    Mensaje men({ 0x72, 0x9E, 0x1D, 0x33, 0x34, 0x51, 0x75, 0x09, 0x14 });
    miraCorrecto(men, 0, men.getWordAt(0), 29342);
    miraCorrecto(men, 1, men.getWordAt(1), 40477);
    miraCorrecto(men, 2, men.getWordAt(2), 7475);
    miraCorrecto(men, 3, men.getWordAt(3), 13108);
    miraCorrecto(men, 4, men.getWordAt(4), 13393);
    miraCorrecto(men, 5, men.getWordAt(5), 20853);
    miraCorrecto(men, 6, men.getWordAt(6), 29961);
    miraCorrecto(men, 7, men.getWordAt(7), 2324);
  }

  {
    Mensaje men({ 0x2B, 0x63, 0x14, 0x6E, 0x6C, 0x60, 0xD7 });
    miraCorrecto(men, 0, men.getWordAt(0), 11107);
    miraCorrecto(men, 1, men.getWordAt(1), 25364);
    miraCorrecto(men, 2, men.getWordAt(2), 5230);
    miraCorrecto(men, 3, men.getWordAt(3), 28268);
    miraCorrecto(men, 4, men.getWordAt(4), 27744);
    miraCorrecto(men, 5, men.getWordAt(5), 24791);
  }

  {
    Mensaje men({ 0x4D, 0xC8, 0xF1, 0xC6, 0x6E, 0xA2, 0x6A, 0x4A, 0xB8, 0xF3, 0x63, 0x09, 0x5E, 0x42, 0xD0 });
    miraCorrecto(men, 0, men.getWordAt(0), 19912);
    miraCorrecto(men, 1, men.getWordAt(1), 51441);
    miraCorrecto(men, 2, men.getWordAt(2), 61894);
    miraCorrecto(men, 3, men.getWordAt(3), 50798);
    miraCorrecto(men, 4, men.getWordAt(4), 28322);
    miraCorrecto(men, 5, men.getWordAt(5), 41578);
    miraCorrecto(men, 6, men.getWordAt(6), 27210);
    miraCorrecto(men, 7, men.getWordAt(7), 19128);
    miraCorrecto(men, 8, men.getWordAt(8), 47347);
    miraCorrecto(men, 9, men.getWordAt(9), 62307);
    miraCorrecto(men, 10, men.getWordAt(10), 25353);
    miraCorrecto(men, 11, men.getWordAt(11), 2398);
    miraCorrecto(men, 12, men.getWordAt(12), 24130);
    miraCorrecto(men, 13, men.getWordAt(13), 17104);
  }

  {
    Mensaje men({ 0xDE, 0x9C, 0x57, 0x04, 0x7F, 0x0A, 0x4A });
    miraCorrecto(men, 0, men.getWordAt(0), 56988);
    miraCorrecto(men, 1, men.getWordAt(1), 40023);
    miraCorrecto(men, 2, men.getWordAt(2), 22276);
    miraCorrecto(men, 3, men.getWordAt(3), 1151);
    miraCorrecto(men, 4, men.getWordAt(4), 32522);
    miraCorrecto(men, 5, men.getWordAt(5), 2634);
  }

  {
    Mensaje men({ 0x08, 0x02, 0x02, 0x00, 0x01, 0x0A, 0x0B, 0x01, 0x00, 0x00, 0x03, 0x0D, 0x02, 0x08, 0x09 });
    miraCorrecto(men, 0, men.getWordAt(0), 2050);
    miraCorrecto(men, 1, men.getWordAt(1), 514);
    miraCorrecto(men, 2, men.getWordAt(2), 512);
    miraCorrecto(men, 3, men.getWordAt(3), 1);
    miraCorrecto(men, 4, men.getWordAt(4), 266);
    miraCorrecto(men, 5, men.getWordAt(5), 2571);
    miraCorrecto(men, 6, men.getWordAt(6), 2817);
    miraCorrecto(men, 7, men.getWordAt(7), 256);
    miraCorrecto(men, 8, men.getWordAt(8), 0);
    miraCorrecto(men, 9, men.getWordAt(9), 3);
    miraCorrecto(men, 10, men.getWordAt(10), 781);
    miraCorrecto(men, 11, men.getWordAt(11), 3330);
    miraCorrecto(men, 12, men.getWordAt(12), 520);
    miraCorrecto(men, 13, men.getWordAt(13), 2057);
  }

  {
    Mensaje men({ 0x07, 0x0A, 0x00, 0x0C, 0x04, 0x00, 0x0B, 0x00, 0x05 });
    miraCorrecto(men, 0, men.getWordAt(0), 1802);
    miraCorrecto(men, 1, men.getWordAt(1), 2560);
    miraCorrecto(men, 2, men.getWordAt(2), 12);
    miraCorrecto(men, 3, men.getWordAt(3), 3076);
    miraCorrecto(men, 4, men.getWordAt(4), 1024);
    miraCorrecto(men, 5, men.getWordAt(5), 11);
    miraCorrecto(men, 6, men.getWordAt(6), 2816);
    miraCorrecto(men, 7, men.getWordAt(7), 5);
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
