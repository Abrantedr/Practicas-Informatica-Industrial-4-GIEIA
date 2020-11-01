// Autor: Rubén Abrante Delgado. Fecha: 01/11/2020

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

int binarioStringAEntero(std::string binario) {
  int entero = 0;
  int b = binario.size() - 1;
  for (std::size_t i = 0; i < binario.size(); i++) {
    if (binario.at(i) == '1') {
      entero |= 1 << b;
    }
    b--;
  }
  return entero;
}

std::vector<int> mayoresQuePivote(
    const std::vector<std::string>& vector,
    int pivote) {

  std::vector<int> vectorInt;
  for (const std::string& binario: vector) {
    int valor = binarioStringAEntero(binario);
    if (valor > pivote)
      vectorInt.push_back(valor);
  }
  return vectorInt;
}

// Muestra un vector de cualquier tipo por std::cerr
template <class T> void muestraVector(const std::vector<T>& vector) {
  if (vector.empty()) {
    std::cerr << "El vector esta vacio." << std::endl;
    return;
  }
  std::cerr << "El vector contiene: ";
  for (const T& valor: vector) {
    std::cerr << valor << ", ";
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
    std::cerr << "El archivo no tiene extension '" << extension
              << std::endl;
    archivo = archivo + extension;
  }

  if (!tienePrefijo(archivo, prefijo)) {
    std::cerr << "El archivo no tiene prefijo '" << prefijo << "'."
              << std::endl;
    archivo = prefijo + archivo;
  }

  std::cerr << "El archivo final se llama '" << archivo << "'."
            << std::endl;

  int pivote = 32;
  if (argc >= 3) {
    try {
      pivote = std::stoi(argv[2]);
    } catch (std::invalid_argument& e) {
      std::cerr << "El pivote no ha podido convertirse a entero."
                << " Motivo: " << e.what() << std::endl;
      return 2;
    }
  }
  std::cerr << "El pivote vale '" << pivote << "'." << std::endl;

  // Validamos el pivote
  if (pivote < 0) {
    std::cerr << "El pivote no puede ser negativo." << std::endl;
    return 3;
  }

  // Abrimos el archivo
  std::ifstream file(archivo);
  if (file.fail()) {
    std::cerr << "Se ha producido un error al abrir el archivo"
              << std::endl;
    return 4;
  }

  std::string binario;
  std::vector<std::string> vector;

  // Leemos su contenido
  file >> binario;
  while (file.good()) {
    vector.push_back(binario);
    file >> binario;
  }
  muestraVector<std::string>(vector);

  // Llamamos a nuestra función
  std::vector<int> resultado = mayoresQuePivote(vector, pivote);
  muestraVector<int>(resultado);

  // Mostramos el resultado por std::cout
  if (resultado.empty()) {
    std::cout << "No hay ningun valor mayor que el pivote '"
              << pivote << "'." << std::endl;
    return 5;
  }
  for (int valor: resultado) {
    std::cout << std::dec << valor;
    std::cout << " (0x" << std::hex << std::uppercase << valor << "), ";
  }
  std::cout << std::endl;

  return 0;
}
