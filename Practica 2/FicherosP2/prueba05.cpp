// prueba de la clase mensaje

#define TITULO "Mensaje: probando pushWord_back()"

#include "Mensaje.hpp"
#include <iostream>
#include <iomanip>

int pruebas = 0;
int fallos = 0;

void miraCorrecto(Mensaje& men, uint16_t val, std::string esperado) {
  pruebas++;
  std::cout << "\nAñadiendo word " << val
      << "(0x" << std::hex << val << std::dec << ")\n";
  men.pushWord_back(val);
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

  std::vector <std::string> totEsperado {
    "[(3) 4a 00 00 ]",
    "[(5) 4a 00 00 04 00 ]",
    "[(7) 4a 00 00 04 00 00 ff ]",
    "[(9) 4a 00 00 04 00 00 ff ff 00 ]",
    "[(11) 4a 00 00 04 00 00 ff ff 00 0f ff ]",
    "[(13) 4a 00 00 04 00 00 ff ff 00 0f ff 55 55 ]",
    "[(5) 66 b6 4f a9 a3 ]",
    "[(7) 66 b6 4f a9 a3 2b 1b ]",
    "[(9) 66 b6 4f a9 a3 2b 1b 59 d7 ]",
    "[(11) 66 b6 4f a9 a3 2b 1b 59 d7 cd 36 ]",
    "[(13) 66 b6 4f a9 a3 2b 1b 59 d7 cd 36 e6 03 ]",
    "[(15) 66 b6 4f a9 a3 2b 1b 59 d7 cd 36 e6 03 4e 0f ]",
  };
  std::size_t ea = 0;

  {
    Mensaje men({ 0x4A });
    std::cout << "Mensaje original:" << men.toString() << "\n";
    miraCorrecto(men, 0, totEsperado[ea++]);
    miraCorrecto(men, 1024, totEsperado[ea++]);
    miraCorrecto(men, 0xFF, totEsperado[ea++]);
    miraCorrecto(men, 0xFF00, totEsperado[ea++]);
    miraCorrecto(men, 0xFFF, totEsperado[ea++]);
    miraCorrecto(men, 0x5555, totEsperado[ea++]);
  }


  {
    Mensaje men({ 0x66, 0xB6, 0x4F });
    std::cout << "Mensaje original:" << men.toString() << "\n";
    miraCorrecto(men, 43427, totEsperado[ea++]);
    miraCorrecto(men, 11035, totEsperado[ea++]);
    miraCorrecto(men, 22999, totEsperado[ea++]);
    miraCorrecto(men, 52534, totEsperado[ea++]);
    miraCorrecto(men, 58883, totEsperado[ea++]);
    miraCorrecto(men, 19983, totEsperado[ea++]);
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
