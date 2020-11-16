// prueba de la clase mensaje

#define TITULO "Mensaje: probando setWordAt()"

#include "Mensaje.hpp"
#include <iostream>
#include <iomanip>

int pruebas = 0;
int fallos = 0;

void miraCorrecto(Mensaje& men, unsigned ind, uint16_t val, std::string esperado) {
  pruebas++;
  std::cout << "\nCambiando word " << ind << " con valor " << val
      << "(0x" << std::hex << val << std::dec << ")\n";
  men.setWordAt(ind, val);
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
    "[(12) 00 00 75 20 4a 08 39 e6 d0 ca ee 66 ]",
    "[(12) 00 04 00 20 4a 08 39 e6 d0 ca ee 66 ]",
    "[(12) 00 04 00 20 4a 08 39 e6 d0 ca 00 ff ]",
    "[(12) 00 04 00 20 4a 08 39 e6 ff 00 00 ff ]",
    "[(12) 00 04 00 0f ff 08 39 e6 ff 00 00 ff ]",
    "[(12) 00 04 55 55 ff 08 39 e6 ff 00 00 ff ]",
    "[(8) 08 a4 a5 a9 42 a9 a3 4f ]",
    "[(8) 08 a4 a5 a9 42 2b 1b 4f ]",
    "[(8) 08 a4 a5 a9 59 d7 1b 4f ]",
    "[(8) 08 a4 a5 a9 59 d7 cd 36 ]",
    "[(8) 08 e6 03 a9 59 d7 cd 36 ]",
    "[(8) 08 e6 03 4e 0f d7 cd 36 ]",
  };
  std::size_t ea = 0;

  {
    Mensaje men({ 0xF0, 0x57, 0x75, 0x20, 0x4A, 0x08, 0x39, 0xE6, 0xD0, 0xCA, 0xEE, 0x66 });
    std::cout << "Mensaje original:" << men.toString() << "\n";
    miraCorrecto(men, 0, 0, totEsperado[ea++]);
    miraCorrecto(men, 1, 1024, totEsperado[ea++]);
    miraCorrecto(men, 10, 0xFF, totEsperado[ea++]);
    miraCorrecto(men, 8, 0xFF00, totEsperado[ea++]);
    miraCorrecto(men, 3, 0xFFF, totEsperado[ea++]);
    miraCorrecto(men, 2, 0x5555, totEsperado[ea++]);
  }


  {
    Mensaje men({ 0x08, 0xA4, 0xA5, 0xA9, 0x42, 0x66, 0xB6, 0x4F });
    std::cout << "Mensaje original:" << men.toString() << "\n";
    miraCorrecto(men, 5, 43427, totEsperado[ea++]);
    miraCorrecto(men, 5, 11035, totEsperado[ea++]);
    miraCorrecto(men, 4, 22999, totEsperado[ea++]);
    miraCorrecto(men, 6, 52534, totEsperado[ea++]);
    miraCorrecto(men, 1, 58883, totEsperado[ea++]);
    miraCorrecto(men, 3, 19983, totEsperado[ea++]);
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
