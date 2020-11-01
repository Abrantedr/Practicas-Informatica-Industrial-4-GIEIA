#include <iostream>
#include <string>
#include <vector>
#include <fstream>

// Muestra el contenido de un vector por std::cerr, del tipo que sea
template <typename T> void muestraVector(const std::vector<T>& vector) {
  if (vector.empty()) {
    std::cerr << "El vector esta vacio" << std::endl;
    return;
  }
  std::cerr << "El vector contiene: ";
  for (const T& k: vector) {
    std::cerr << k << ", ";
  }
  std::cerr << std::endl;
}

// Indica si un archivo tiene una determinada extensión
bool tieneSufijo(const std::string& archivo,
                 const std::string& sufijo) {

  return archivo.size() >= sufijo.size() &&
      archivo.compare(archivo.size() - sufijo.size(),
                      sufijo.size(), sufijo) == 0;

}

// Indica si un archivo tiene un prefijo determinado
bool tienePrefijo(const std::string& archivo,
                  const std::string& prefijo) {

  return archivo.size() >= prefijo.size() &&
      archivo.compare(0, prefijo.size(), prefijo) == 0;
}

std::vector<int> mayoresQuePivoteBit(std::vector<int>& vector,
                                     int m, int pivote) {
  std::vector<int> vectorInt;
  for (const int& k: vector) {
    if ((k & (1 << m)) && (k > pivote)) {
      vectorInt.push_back(k);
    }
  }
  return vectorInt;
}

int main(int argc, char* argv[]) {

  if (argc < 2) {
    std::cerr << "No se ha especificado el nombre del archivo"
        << std::endl;
    return 1;
  }

  std::string archivo = argv[1];
  std::string extension = ".dat";
  std::string prefijo = "datos";

  if (!tieneSufijo(archivo, extension)) {
    std::cerr << "El archivo no tiene extension '" << extension << "'."
        << std::endl;
    archivo = archivo + extension;
  }

  if (!tienePrefijo(archivo, prefijo)) {
    std::cerr << "El archivo no empieza por '" << prefijo << "'."
        << std::endl;
    archivo = prefijo + archivo;
  }

  std::cerr << "El archivo final se llama '" << archivo << "'."
      << std::endl;

  int m = 0;
  int pivote = 0;
  if (argc >= 3) {
    try {
      m = std::stoi(argv[2]);
      if (argc > 3)
        pivote = std::stoi(argv[3]);
    } catch (std::invalid_argument& e) {
      std::cerr << "El valor de m o el valor del "
          << "pivote no pudo convertirse a entero. Motivo: " << e.what()
          << std::endl;
      return 2;
    }
  }
  std::cerr << "El valor de m es '" << m
      << "' y el valor del pivote es '" << pivote << "'." << std::endl;

  // Validamos m y el pivote
  if (m < 0 || pivote < 0) {
    std::cerr << "El valor de m o el pivote no puede ser negativo"
        << std::endl;
    return 3;
  }

  // Abrimos el archivo
  std::ifstream file(archivo);

  if (file.fail()) {
    std::cerr << "Se ha producido un error al abrir el archivo"
        << std::endl;
    return 4;
  }

  int dato;
  std::vector<int> vector;
  // Leemos el contenido
  file >> dato;
  while (file.good()) {
    vector.push_back(dato);
    file >> dato;
  }
  muestraVector<int>(vector);

  // Llamamos a la función del programa
  std::vector<int> resultado = mayoresQuePivoteBit(vector, m, pivote);
  muestraVector<int>(resultado);

  if (resultado.empty()) {
    std::cerr << "No es necesario generar un archivo" << std::endl;
    return 5;
  }

  // Escribimos el vector resultado en el archivo "..._m_mayores.dat"
  archivo = archivo.substr(0, archivo.size() - extension.size())
      + "_" + std::to_string(m) + "_mayores" + extension;

  std::ofstream outputFile(archivo);

  if (outputFile.fail()) {
    std::cerr << "No se ha podido escribir el archivo de salida"
        << std::endl;
    return 6;
  }

  // Escribimos el contenido
  for (int k: resultado) {
    outputFile << k << std::endl;
    if (outputFile.fail()) {
      std::cerr << "Se ha producido un error mientras se escribia "
                   " el archivo de salida" << std::endl;
      return 7;
    }
  }
  std::cerr << "Se ha escrito el archivo '" << archivo
            << "' satisfactoriamente" << std::endl;

  return 0;
}
