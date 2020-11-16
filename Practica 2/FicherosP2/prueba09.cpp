// prueba de la clase mensaje

#define TITULO "Mensaje: probando out_of_range"

#include "Mensaje.hpp"
#include <iostream>
#include <iomanip>

int pruebas = 0;
int fallos = 0;

void miraCorrecto(Mensaje& men, unsigned size) {
  std::vector<unsigned> sizes { size, size + 1, size + 10};
  std::cout << "Considerando mensaje " << men.toString() << std::endl;
  for (unsigned sz : sizes) {
    try {
      pruebas++;
      men.getByteAt(sz);
      std::cout << "\nFALLO " << ++fallos << ": no se generó excepción"
          << " getByteAt( " << sz << ")" << std::endl;
    } catch (std::out_of_range) {
      std::cout << "\nCORRECTO se generó excepción"
          << " getByteAt( " << sz << ")" << std::endl;
    }
    try {
      pruebas++;
      men.setByteAt(sz, 0xFF);
      std::cout << "\nFALLO " << ++fallos << ": no se generó excepción"
          << " setByteAt(" << sz << ")" << std::endl;
    } catch (std::out_of_range) {
      std::cout << "\nCORRECTO se generó excepción"
          << " setByteAt(" << sz << ")" << std::endl;
    }
    try {
      pruebas++;
      men.getWordAt(sz - 1);
      std::cout << "\nFALLO " << ++fallos << ": no se generó excepción"
          << " getWordAt(" << sz -1 << ")" << std::endl;
    } catch (std::out_of_range) {
      std::cout << "\nCORRECTO se generó excepción"
          << " getWordAt(" << sz -1 << ")" << std::endl;
    }
    try {
      pruebas++;
      men.setWordAt(sz - 1, 0xFF);
      std::cout << "\nFALLO " << ++fallos << ": no se generó excepción"
          << " setWordAt(" << sz -1 << ")" << std::endl;
    } catch (std::out_of_range) {
      std::cout << "\nCORRECTO se generó excepción"
          << " setWordAt(" << sz -1 << ")" << std::endl;
    }
  }
}

int main() {

  std::cout << "\n***  " << TITULO << "  ***\n" << std::endl;

  std::vector <std::string> totEsperado {
    "[(4) 01 02 03 04 ]",
    "[(0) ]",
    "[(1) 6d ]",
    "[(2) ab 19 ]",
    "[(3) c7 6a 70 ]",
    "[(4) 69 57 ff 10 ]",
    "[(5) 93 7c 4f 20 73 ]",
    "[(11) 76 58 70 62 9c f7 ee 49 4a 7d b5 ]",
    "[(6) c0 b4 0f 28 51 a9 ]",
    "[(15) dd 07 3d a3 eb 44 6b a0 d2 6c fa 49 f3 d5 d7 ]",
    "[(16) 51 6c 2f d4 03 8f e5 72 4e 4c 4d 4a 46 76 02 fc ]",
  };
  std::size_t ea = 0;

  {
    Mensaje men({1, 2, 3, 4});
    miraCorrecto(men, 4);
  }

  {
    Mensaje men({109});
    miraCorrecto(men, 1);
  }

  {
    Mensaje men({171, 25});
    miraCorrecto(men, 2);
  }

  {
    Mensaje men({147, 124, 79, 32, 115});
    miraCorrecto(men, 5);
  }

  {
    Mensaje men({81, 108, 47, 212, 3, 143, 229, 114, 78, 76, 77, 74, 70, 118, 2, 252});
    miraCorrecto(men, 16);
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
