// Autor: Rubén Abrante Delgado. Fecha: 04/11/2020

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

bool tieneSufijo(const std::string& archivo,
                 const std::string& extension) {

  return archivo.size() >= extension.size() &&
         archivo.compare(archivo.size() - extension.size(),
                         extension.size(), extension) == 0;
}

bool tienePrefijo(const std::string& archivo,
                  const std::string& prefijo) {

  return archivo.size() >= prefijo.size() &&
         archivo.compare(0, prefijo.size(), prefijo) == 0;
}

template <typename T> void muestraVector(const std::vector<T>& vector) {
  if (vector.empty()) {
    std::cerr << "El vector esta vacio" << std::endl;
  } else {
    std::cerr << "El vector contiene: ";
    for (const T& k: vector) {
      std::cerr << k << ", ";
    }
    std::cerr << std::endl;
  }
}

unsigned int binStringAUint(const std::string& bin) {
  unsigned int uint = 0;
  std::size_t b = bin.size() - 1;
  for (std::size_t i = 0; i < bin.size(); ++i) {
    if (bin.at(i) == '1')
      uint |= 1 << b;
    b--;
  }
  return uint;
}

std::vector<unsigned int> menoresQuePivote(
    const std::vector<std::string>& vector,
    const unsigned int& pivote) {

  std::vector<unsigned int> resultado;
  for (const std::string& bin: vector) {
    unsigned int entero = binStringAUint(bin);
    if (entero > pivote)
      resultado.push_back(entero);
  }
  return resultado;
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

  std::cerr << "El archivo se llama '" << archivo << "'." << std::endl;

  // El pivote vale 18 por defecto
  unsigned int pivote = 18;
  if (argc >= 3) {
    try {
      pivote = std::stoi(argv[2]);
    } catch (std::invalid_argument& e1) {
      std::cerr << "El pivote '" << pivote
                << "' no ha podido convertirse" << " a entero. Motivo: "
                << e1.what() << std::endl;
      return 2;
    } catch (std::out_of_range& e2) {
      std::cerr << "El pivote es demasiado grande."
                << " Motivo: " << e2.what() << std::endl;
      return 3;
    }
  }

  std::cerr << "El pivote vale '" << pivote << "'." << std::endl;

  // Validamos que el pivote no ha sufrido overflow
  if (argc >= 3 && std::stoi(argv[2]) < 0) {
    std::cerr << "El pivote ha sufrido overflow."
              << " No introduzca pivotes negativos" << std::endl;
    return 4;
  }

  // Abrimos el archivo
  std::ifstream file(archivo);

  if (file.fail()) {
    std::cerr << "No se ha podido abrir el archivo" << std::endl;
    return 5;
  }

  // Volcamos su contenido en un vector de string
  std::string dato;
  std::vector<std::string> vector;

  file >> dato;
  while (file.good()) {
    vector.push_back(dato);
    file >> dato;
  }
  muestraVector<std::string>(vector);

  // Llamamos a nuestra función del programa
  std::vector<unsigned int> res = menoresQuePivote(vector, pivote);
  muestraVector<unsigned int>(res);

  // Mostramos el resultado por std::cout
  std::cout << "Pivote: " << pivote << std::endl;
  std::cout << "Resultado: ";
  for (const unsigned int& k: res) {
    std::cout << std::dec << k;
    std::cout << " (0x" << std::hex << std::uppercase << k << "), ";
  }
  std::cout << std::endl;

  return 0;
}
