// Ejercicio 02 - All tests passed!
// Autor: Rubén Abrante Delgado
// Fecha: 13/10/2020

#include <fstream>
#include <iostream>
#include <vector>


/**
 * Recorre un vector de enteros y devuelve otro con
 * los elementos del vector mayores que el pivote.
 *
 * @param vector Vector de enteros a recorrer.
 * @param pivote Pivote entero de referencia.
 *
 * @return Vector de enteros con los elementos mayores que el pivote.
 */
std::vector<int> vectorMayores(const std::vector<int>& vector,
                               int pivote) {
  std::vector<int> mayores;
  for (int elemento: vector) {
    if (elemento > pivote)
      mayores.push_back(elemento);
  }
  return mayores;
}


int main(int argc, char *argv[]) {

  if (argc < 2) {
    std::cerr << "No se ha especificado el nombre del archivo"
        << std::endl;
    return 1;
  }

  // Abre el archivo usando el primer argumento
  std::ifstream file(argv[1]);

  if (!file.good()) {
    std::cerr << "El archivo no ha podido abrirse o no existe"
        << std::endl;
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

  /* Si no se ha pasado el parámetro del pivote, tendrá un valor de 2.
   * En otro caso, usaremos el valor pasado por parámetros */
  int pivote = 2;

  if (argc >= 3) {
    try {
      pivote = std::stoi(argv[2]);
    } catch (std::invalid_argument& e) {
      std::cerr << "Argumento invalido (el pivote '" << argv[2]
          << "' no ha podido convertirse a entero). Motivo: "
          << e.what() << std::endl;
      return 3;
    }
  }

  std::vector<int> mayores = vectorMayores(vector, pivote);
  std::cout << "Pivote: " << pivote << std::endl;
  std::cout << "Resultado: ";
  if (mayores.empty()) {
    std::cerr << "No existe ningun elemento mayor que " << pivote
        << std::endl;
  } else {
    for (int elemento: mayores)
      std::cout << elemento << ", ";
  }

  return 0;
}
