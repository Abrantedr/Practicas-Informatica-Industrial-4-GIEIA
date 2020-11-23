// Autor: Rubén Abrante Delgado. Fecha: 04/11/2020

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

// Convierte un numero binario de tipo string a unsigned int
unsigned int binStrAUint(const std::string& bin) {
  unsigned int sol = 0;
  std::size_t b = bin.size() - 1;
  for (std::size_t i = 0; i < bin.size(); i++) {
    if (bin.at(i) == '1')
      sol |= 1 << b;
    b--;
  }
  return sol;
}

// Devuelve un vector de u_int con los valores que son menores que el pivote
std::vector<unsigned int> menoresQuePivote(
    const std::vector<std::string>& vector,
    unsigned int pivote) {
  
  std::vector<unsigned int> res;
  unsigned int entero;
  for (const std::string& bin: vector) {
    entero = binStrAUint(bin);
    if (entero < pivote)
      res.push_back(entero);
  }
  return res;
}

// Muestra el contenido de un vector por std::cerr, del tipo que sea
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

// Comprueba si un archivo posee una extension determinada
bool tieneSufijo(const std::string& archivo, const std::string& sufijo) {
  
  return archivo.size() >= sufijo.size() &&
      archivo.compare(archivo.size() - sufijo.size(),
          sufijo.size(), sufijo) == 0;
}

// Comprueba si un archivo empieza por un prefijo determinado
bool tienePrefijo(const std::string& archivo, const std::string& prefijo) {
  
  return archivo.size() >= prefijo.size() &&
      archivo.compare(0, prefijo.size(), prefijo) == 0;
}


int main(int argc, char* argv[]) {
  
  if (argc < 2) {
    std::cerr << "No se ha especificado el archivo" << std::endl;
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
    std::cerr << "El archivo no empieza por '" << prefijo << "'." << std::endl;
    archivo = prefijo + archivo;
  }
  
  std::cerr << "El archivo se llama '" << archivo << "'." << std::endl;
  
  // Por defecto, el pivote vale 20
  unsigned int pivote = 20;
  
  if (argc >= 3) {
    try {
      pivote = std::stoi(argv[2]);
    } catch (std::invalid_argument& e1) {
      std::cerr << "El pivote '" << argv[2] << "' no ha podido convertirse "
          << "a entero. Motivo: " << e1.what() << std::endl;
      return 2;
    } catch (std::out_of_range& e2) {
      std::cerr << "El pivote es demasiado grande. Motivo: " << e2.what()
          << std::endl;
      return 3;
    }
  }
  
  std::cerr << "El pivote vale '" << pivote << "'." << std::endl;
  
  // Validamos el pivote
  if (argc >= 3 && std::stoi(argv[2]) < 0) {
    std::cerr << "El pivote ha sufrido overflow."
        << " No introduzca pivotes negativos" << std::endl;
    return 4;
  }
  
  // Abrimos el fichero
  std::ifstream file(archivo);
  
  if (file.fail()) {
    std::cerr << "Se ha producido un error al abrir el archivo" << std::endl;
    return 5;
  }
  
  std::string dato;
  std::vector<std::string> vector;
  // Leemos el contenido del fichero
  file >> dato;
  while (file.good()) {
    vector.push_back(dato);
    file >> dato;
  }
  muestraVector(vector);
  

  // Le pasamos la funcion a nuestro programa
  std::vector<unsigned int> res = menoresQuePivote(vector, pivote);
  muestraVector(res);

  // Mostramos el resultado por std::cout
  std::cout << "Pivote: " << pivote << std::endl;
  std::cout << "Resultado: ";
  for (const unsigned int& k: res) {
    std::cout << std::dec << k;
    std::cout << "(0x" << std::hex << k << "), ";
  }
  std::cout << std::endl;

  return 0;
}









