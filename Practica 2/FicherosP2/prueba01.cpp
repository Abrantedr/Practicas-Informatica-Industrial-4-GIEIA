// prueba de la clase mensaje

#define TITULO "Mensaje: probando toString()"

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
    miraCorrecto(men.toString(), totEsperado[ea++]);
  }

  {
    Mensaje men;
    miraCorrecto(men.toString(), totEsperado[ea++]);
  }

  {
    Mensaje men({109});
    miraCorrecto(men.toString(), totEsperado[ea++]);
  }

  {
    Mensaje men({171, 25});
    miraCorrecto(men.toString(), totEsperado[ea++]);
  }

  {
    Mensaje men({199, 106, 112});
    miraCorrecto(men.toString(), totEsperado[ea++]);
  }

  {
    Mensaje men({105, 87, 255, 16});
    miraCorrecto(men.toString(), totEsperado[ea++]);
  }

  {
    Mensaje men({147, 124, 79, 32, 115});
    miraCorrecto(men.toString(), totEsperado[ea++]);
  }

  {
    Mensaje men({118, 88, 112, 98, 156, 247, 238, 73, 74, 125, 181});
    miraCorrecto(men.toString(), totEsperado[ea++]);
  }


  {
    Mensaje men({192, 180, 15, 40, 81, 169});
    miraCorrecto(men.toString(), totEsperado[ea++]);
  }


  {
    Mensaje men({221, 7, 61, 163, 235, 68, 107, 160, 210, 108, 250, 73, 243, 213, 215});
    miraCorrecto(men.toString(), totEsperado[ea++]);
  }


  {
    Mensaje men({81, 108, 47, 212, 3, 143, 229, 114, 78, 76, 77, 74, 70, 118, 2, 252});
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
