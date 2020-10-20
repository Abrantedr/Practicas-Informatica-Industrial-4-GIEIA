// Ejercicio 01
// Autor: Rubén Abrante Delgado
// Fecha: 13/10/2020

#include <fstream>
#include <iostream>
#include <vector>


/**
 * Recorre un vector de enteros y cuenta el número de veces que
 * los elementos del vector son mayores que el pivote.
 *
 * @param vector Vector de enteros a recorrer.
 * @param pivote Pivote entero de referencia.
 *
 * @return El número de veces que se cumple la condición.
 */
int cuantosMayores(const std::vector<int>& vector, int pivote) {
  int cuantos = 0;

  for (int elemento: vector) {
    if (elemento > pivote)
      cuantos++;
  }
  return cuantos;
}


int main(int argc, char *argv[]) {

  if (argc < 2) {
    std::cerr << "No se ha especificado el nombre del archivo" << std::endl;
    return 1;
  }

  // Abre el archivo usando el primer argumento
  std::ifstream file(argv[1]);

  if (!file.good()) {
    std::cerr << "El archivo no ha podido abrirse o no existe" << std::endl;
    return 2;
  }

  std::vector<int> vector;
  int dato;

  // Intenta leer el primer dato del archivo
  file >> dato;
  while (file.good()) {
    // Lee el resto del archivo y coloca su contenido en un vector
    vector.push_back(dato);
    file >> dato;
  }

  /* Si no se ha pasado el parámetro del pivote, tendrá un valor de 11.
   * En otro caso, usaremos el valor pasado por parámetros */
  int pivote = 11;

  if (argc >= 3) {
    try {
      pivote = std::stoi(argv[2]);
    } catch (std::invalid_argument& e) {
      std::cerr << "Argumento invalido (el pivote '" << argv[2]
          << "' no ha podido convertirse a entero). Motivo: " << e.what()
          << std::endl;
      return 3;
    }
  }

  int resultado = cuantosMayores(vector, pivote);
  std::cout << "Pivote: " << pivote << std::endl;
  std::cout << "Resultado: " << resultado << std::endl;

  return 0;
}
