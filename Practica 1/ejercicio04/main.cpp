// Ejercicio 04
// Autor: Rubén Abrante Delgado
// Fecha: 19/10/2020

#include <fstream>
#include <iostream>
#include <vector>


/**
 * Recorre un vector de enteros y devuelve otros dos con
 * los elementos del vector mayores y menores que el pivote.
 * Devuelve el número de elementos iguales al pivote por
 * parámetro de salida.
 *
 * @param vector Vector de enteros a recorrer.
 * @param mayores Vector que almacena los elementos mayores que el pivote.
 * @param menores Vector que almacena los elementos menores que el pivote
 * @param pivote Pivote entero de referencia.
 *
 * @return El número de elementos que son iguales al pivote.
 */
int mayoresMenoresIguales(const std::vector<int>& vector,
                          std::vector<int>& mayores,
                          std::vector<int>& menores,
                          int pivote) {
  int iguales = 0;

  for (int elemento: vector) {
    if (elemento > pivote)
      mayores.push_back(elemento);
    else if (elemento < pivote)
      menores.push_back(elemento);
    else
      iguales++;
  }
  return iguales;
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

  /* Si no se ha pasado el parámetro del pivote, tendrá un valor de 10.
   * En otro caso, usaremos el valor pasado por parámetros */
  int pivote = 10;

  if (argc >= 3) {
    try {
      pivote = std::stoi(argv[2]);
    } catch (std::invalid_argument& e) {
      std::cerr << "Argumento invalido (el pivote '"
          << argv[2] << "' no ha podido convertirse a entero). Motivo: "
          << e.what() << std::endl;
      return 3;
    }
  }

  std::vector<int> mayores, menores;
  int iguales = mayoresMenoresIguales(vector, mayores, menores, pivote);

  std::cout << "Pivote: " << pivote << std::endl;
  std::cout << "Mayores: ";

  if (mayores.empty()) {
    std::cerr << "No existe ningun elemento mayor que " << pivote << std::endl;
  } else {
    std::cout << std::endl;
    for (int elemento: mayores)
      std::cout << elemento << std::endl;
  }

  std::cout << "Menores: ";

  if (menores.empty()) {
    std::cerr << "No existe ningun elemento menor que " << pivote << std::endl;
  } else {
    std::cout << std::endl;
    for (int elemento: menores)
      std::cout << elemento << std::endl;
  }

  std::cout << "Iguales: " << iguales << std::endl;

  return 0;
}
