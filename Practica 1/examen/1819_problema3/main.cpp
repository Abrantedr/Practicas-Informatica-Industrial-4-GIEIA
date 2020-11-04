// Autor: Rubén Abrante Delgado. Fecha: 04/11/2020

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

std::vector<bool> bitMesimo(const std::vector<int>& vector,
                            int n, int m) {
  std::vector<bool> resultado;
  for (std::size_t i = 0; i < vector.size(); i += n) {
    if (vector.at(i) & (1 << m))
      resultado.push_back(true);
    else
      resultado.push_back(false);
  }
  return resultado;
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

bool tieneSufijo(const std::string& archivo,
                 const std::string& sufijo) {

  return archivo.size() >= sufijo.size() &&
         archivo.compare(archivo.size() - sufijo.size(),
                         sufijo.size(), sufijo) == 0;
}

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

  int m = 0;
  // Por defecto, n vale 2
  int n = 2;
  if (argc >= 3) {
    try {
      m = std::stoi(argv[2]);
      if (argc >= 4)
        n = std::stoi(argv[3]);
    } catch (std::invalid_argument& e1) {
      std::cerr << "El valor de m o el valor de n no pudo convertirse a"
          << " entero. Motivo: " << e1.what() << std::endl;
      return 2;
    } catch (std::out_of_range& e2) {
      std::cerr << "El valor de m o el valor de n es demasiado grande."
          << " Motivo: " << e2.what() << std::endl;
      return 3;
    }
  }

  std::cerr << "El valor de m es '" << m << "' y el valor de n es '"
      << n << "'." << std::endl;

  // Validamos m y n
  if (m < 0 || n < 1) {
    std::cerr << "m no puede ser negativo y n no puede valer 0 o menos"
        << std::endl;
    return 4;
  }

  // Abrimos el archivo
  std::ifstream file(archivo);

  if (file.fail()) {
    std::cerr << "Se ha producido un error al abrir el archivo"
        << std::endl;
    return 5;
  }

  int dato;
  std::vector<int> vector;

  file >> dato;
  while (file.good()) {
    vector.push_back(dato);
    file >> dato;
  }
  muestraVector<int>(vector);

  // Llamamos a la función del programa
  std::vector<bool> res = bitMesimo(vector, n, m);
  muestraVector(res);

  // Creamos la ruta del fichero de salida
  archivo = archivo.substr(0, archivo.size() - extension.size())
      + "_" + std::to_string(m) + "_" + std::to_string(n) + extension;

  std::cerr << "El archivo de salida se llama '" << archivo << "'."
      << std::endl;

  // Abrimos el archivo de salida
  std::ofstream outFile(archivo);

  if (outFile.fail()) {
    std::cerr << "No se ha podido crear el archivo de salida"
        << std::endl;
    return 6;
  }

  // Volcamos el contenido del vector bool al archivo
  for (const auto& k: res) {
    if (k) {
      outFile << "Verdad" << std::endl;
    } else {
      outFile << "Falso" << std::endl;
    }
    if (outFile.fail()) {
      std::cerr << "Se ha producido un error al escribir el archivo"
          << std::endl;
      return 7;
    }
  }

  std::cerr << "Se ha generado el archivo '"
      << archivo << "' satisfactoriamente" << std::endl;

  return 0;
}
