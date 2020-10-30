// Autor: Rubén Abrante Delgado. Fecha: 30/10/2020
 
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

template <class T> void muestraVector(const std::vector<T>& vector) {
  std::cerr << "El vector contiene: ";
  for (const T& valor: vector)
    std::cerr << valor << ", ";
  std::cerr << std::endl;
}

std::vector<bool> valorBitMesimo(const std::vector<int>& vector,
                                 int n, int m) {
  std::vector<bool> vectorBool;

  for (std::size_t i = 0; i < vector.size(); i += n) {
      if (vector.at(i) & (1 << m))
        vectorBool.push_back(true);
      else
        vectorBool.push_back(false);
  }
  return vectorBool;
}

/**
* Comprueba si un archivo posee una determinada extensión.
*
* @param archivo Dirección del archivo que queremos comprobar.
* @param sufijo Extensión del archivo. (Ej. '.dat')
*
* @return Verdadero si contiene el sufijo, falso si no lo contiene.
*/
bool tieneSufijo(const std::string& archivo,
                 const std::string& sufijo) {

  return archivo.size() >= sufijo.size() && archivo.compare(
      archivo.size() - sufijo.size(), sufijo.size(), sufijo) == 0;
  }

/**
* Comprueba si un archivo posee un prefijo específico.
*
* @param archivo Dirección del archivo que queremos comprobar.
* @param prefijo Prefijo. (Ej. 'prueba')
*
* @return Verdadero si contiene el prefijo, falso si no lo contiene.
*/
bool tienePrefijo(const std::string& archivo,
                  const std::string& prefijo) {

  return archivo.size() >= prefijo.size() && archivo.compare(
      0, prefijo.size(), prefijo) == 0;
  }


  int main(int argc,char* argv[]) {
 
  if (argc < 2) {
    std::cerr <<  "No se ha especificado el nombre del archivo"
        << std::endl;
    return 1;
  }
 
  std::string archivo = argv[1];
  std::cerr << "El archivo pasado por parametros se llama '" << archivo
      << "'." << std::endl;
     
  std::string extension = ".dat";
  std::string prefijo = "datos";
 
  if (!tieneSufijo(archivo, extension)) {
    std::cerr << "El archivo no tiene extension '" << extension << "'"
        << std::endl;
    archivo.append(extension);
  }
 
  if (!tienePrefijo(archivo, prefijo)) {
    std::cerr << "El archivo no tiene prefijo '" << prefijo << "'"
        << std::endl;
    archivo = prefijo + archivo;
  }
 
  std::cerr << "Archivo final: '" << archivo << "'" << std::endl;

  int m = 0;
  int n = 2;

  if (argc >= 3) {
    try {
      m = std::stoi(argv[2]);
      if (argc > 3)
        n = std::stoi(argv[3]);
    } catch (std::invalid_argument& e) {
      std::cerr << "No se pudo convertir '" << argv[2] << "' o '"
          << argv[3] << "' a entero." << " Motivo: " << e.what()
          << std::endl;
      return 2;
    }
  }

  // Validamos los valores de m y n
  if (m < 0 || n <= 0) {
    std::cerr << "Los valores de 'm' y 'n' no pueden ser negativos."
        << " 'n' no puede ser 0" << std::endl;
    return 3;
  }

  std::cerr << "El parametro m vale '" << m
      << "' y el parametro n vale '" << n << "'." << std::endl;

  // Intentamos leer el archivo
  std::ifstream file(archivo);

  if (!file.good()) {
    std::cerr << "No se ha podido abrir el archivo" << std::endl;
    return 4;
  }

  // Leemos el archivo para pasarlo a un vector de enteros
  int dato;
  std::vector<int> vectorInt;

  file >> dato;
  while (file.good()) {
    vectorInt.push_back(dato);
    file >> dato;
  }
  muestraVector<int>(vectorInt);

  // Pasamos la función del programa
  std::vector<bool> vectorBool = valorBitMesimo(vectorInt, n, m);
  muestraVector<bool>(vectorBool);

  //Mostramos el resultado por std::cout
  for (bool b: vectorBool) {
    if (b)
      std::cout << "V, ";
    else
      std::cout << "F, ";
  }
  std::cout << std::endl;

  return 0;
}
 