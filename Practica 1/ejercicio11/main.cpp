// Ejercicio 11 - All tests passed!
// Autor: Rubén Abrante Delgado
// Fecha: 20/10/2020

#include <iostream>
#include <vector>
#include <algorithm>


std::vector<bool> enteroAVectorBool(int entero) {
  std::vector<bool> vectorBool;
  while (entero) {
    vectorBool.push_back(entero & 1);
    entero >>= 1;
  }
  std::reverse(vectorBool.begin(), vectorBool.end());
  return vectorBool;
}

void muestraVectorBool(const std::vector<bool>& vectorBool) {
  for (bool i : vectorBool) {
    i ? std::cout << "V" : std::cout << "F";
    std::cout << ", ";
  }
  std::cout << std::endl;
}

int main() {
  std::vector<bool> vector;
  int num = 0;
  std::cin >> num;
  if (num < 0)
    return 1;
  while (std::cin.good()) {
    if (!num) {
      std::cout << "F, ";
      return 0;
    }
    vector = enteroAVectorBool(num);
    muestraVectorBool(vector);
    std::cin >> num;
    if (num < 0)
      return 1;
  }
  return 2;
}
