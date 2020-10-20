// Ejercicio 10
// Autor: Rubén Abrante Delgado
// Fecha: 19/10/2020

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// Convierte un número entero a binario
std::string aBinario(unsigned n) {
  std::string binario;

  if (n == 0) return "0";
  while (n != 0) {
    binario += n % 2 == 0 ? "0" : "1";
    n /= 2;
  }
  std::reverse(binario.begin(), binario.end());
  return binario;
}

// Pasa de un vector de string a un vector de bool
std::vector<bool> vectorStringABool(std::vector<std::string>& vector) {
  std::vector<bool> vectorBool;
  for (auto &i: vector) {
    if (i == "verdad" || i == "si" || i == "true" || i == "1") {
      vectorBool.push_back(true);
    } else {
      vectorBool.push_back(false);
    }
  }
  return vectorBool;
}

// Convierte un vector de bool a un string de ceros y unos
unsigned long long vectorBoolABinarioString(std::vector<bool>& vector,
                                  unsigned inicialv,
                                  unsigned finalv) {
  std::string binario;
  // Conversión directa a entero.
  // Un unsigned genera demasiados problemas
  int ini = (int) inicialv;
  int fin = (int) finalv;
  for (int i = fin; i >= ini; i--) {
    binario += (vector.at(i)) ? "1" : "0";
  }
  return std::stoull(binario, nullptr, 2);

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

  std::vector<std::string> vector;
  std::string dato;

  // Intenta leer el primer dato del archivo
  file >> dato;
  while (file.good()) {
    // Lee el resto del archivo y coloca su contenido en un vector
    vector.push_back(dato);
    file >> dato;
  }

  // Convertimos el vector de string en bool
  std::vector<bool> vectorBool = vectorStringABool(vector);

  /* Si no se han pasado los valores inicial y final,
   * tendrán un valor de 0 y el tamaño del vector, respectivamente
   * En otro caso, usaremos los valores pasados por parámetros */
  unsigned inicialv = 0;
  unsigned finalv = vectorBool.size();

  if (argc >= 4) {
    try {
      inicialv = std::stoi(argv[2]);
      finalv = std::stoi(argv[3]);
    } catch (std::invalid_argument& e) {
      std::cerr << "Argumento invalido (el valor inicial '"
                << argv[2] << " o el valor final '" << argv[3]
                << "' no ha podido convertirse a entero). Motivo: "
                << e.what() << std::endl;
      return 3;
    } catch (std::out_of_range& e) {
        std::cerr << "Fuera de rango" << std::endl;
        return 4;
    }
  }

  // Nos aseguramos de que los parámetros tienen sentido
  if ((int) inicialv < 0 || (int) finalv < 0 || inicialv > finalv) {
    std::cerr << "Los parametros no son validos."
        << " Los indices no pueden ser negativos."
        << " El indice inicial no puede ser mayor al indice final"
        << std::endl;
    return 5;
  }

  unsigned long long bin = 0;
  // Intentamos conseguir el valor entero del vector de bool
  try {
    bin = vectorBoolABinarioString(vectorBool, inicialv, finalv);
  } catch (std::out_of_range& e) {
    std::cerr << "Fuera de rango (el valor inicial '" << inicialv
              << "' o el valor final '" << finalv
              << "' se encuentran fuera de rango del vector). Motivo: "
              << e.what() << std::endl;
    return 6;
  } catch (std::invalid_argument& e) {
    std::cerr << "Argumento invalido. " << "Motivo: " << e.what()
              << std::endl;
    return 7;
  }

  // Mostramos el resultado por pantalla
  std::cout << bin << " 0x" << std::hex << bin << " 0b" << aBinario(bin)
      << std::endl;

  return 0;
}
