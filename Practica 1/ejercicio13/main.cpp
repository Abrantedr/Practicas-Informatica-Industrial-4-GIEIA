// Ejercicio 13
// Autor: Rubén Abrante Delgado
// Fecha: 21/10/2020

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>


// Muestra un vector por std::cerr, del tipo que sea
template <class T> void muestraVector(const std::vector<T> &vector) {
  std::cerr << "El vector contiene: ";
  for (const T& k: vector) {
    std::cerr << k << ", ";
  }
  std::cerr << std::endl;
}

/**
* Comprueba si un archivo posee una determinada extensión.
*
* @param archivo Dirección del archivo que queremos comprobar.
* @param sufijo Extensión del archivo. (Ej. '.dat')
*
* @return Verdadero si contiene el sufijo, falso si no lo contiene.
*/
bool tieneSufijo(const std::string &archivo, const std::string &sufijo) {
  return archivo.size() >= sufijo.size() &&
         archivo.compare(archivo.size() - sufijo.size(),
                         sufijo.size(), sufijo) == 0;
}

// Ordena un vector de menor a mayor por el método de selección directa
void metodoSeleccionDirecta(std::vector<double> &vector) {

  for (unsigned i = 0; i < vector.size(); ++i) {
    double min = vector.at(i);
    unsigned pos = i;
    // Busca el menor en el subvector
    for (unsigned j = i; j < vector.size(); ++j) {
      if (vector.at(j) < min) {
        min = vector.at(j);
        pos = j;
      }
    }
    // Intercambia el menor del subvector por la posición i
    double temp = vector.at(pos);
    vector.at(pos) = vector.at(i);
    vector.at(i) = temp;
  }
}

int main(int argc, char *argv[]) {

  if (argc < 2) {
    std::cerr << "No se ha especificado el nombre del archivo"
              << std::endl;
    return 1;
  }

  std::string archivo = argv[1];
  std::string extension = ".dat";
  // Si el archivo no tiene extensión .dat, se la añade
  if (!tieneSufijo(archivo, extension))
    archivo.append(extension);

  // Abre el archivo usando el primer argumento
  std::ifstream file(archivo);

  if (!file.good()) {
    std::cerr << "El archivo no ha podido abrirse o no existe"
              << std::endl;
    return 2;
  }

  std::vector<double> vector;
  double dato;
  // Intenta leer el primer dato del archivo
  file >> dato;
  while (file.good()) {
    // Lee el resto del archivo y coloca su contenido en un vector
    vector.push_back(dato);
    file >> dato;
  }
  muestraVector<double>(vector);

  // Ordenamos el vector con el método de selección directa
  metodoSeleccionDirecta(vector);

  // Mostramos el vector por std::cout
  for (double &i: vector) {
    std::cout << std::fixed << std::setprecision(4) << std::setw(7) << i << std::endl;
  }


  return 0;
}
