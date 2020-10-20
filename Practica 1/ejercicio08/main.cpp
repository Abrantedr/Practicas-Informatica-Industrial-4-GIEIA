// Ejercicio 08
// Autor: Rubén Abrante Delgado
// Fecha: 19/10/2020

#include <iostream>

// Devuelve la cantidad de bits a uno que tiene un número entero
int cuantosBitsUno(int registro) {
  int cuantos = 0;
  while (registro) {
    if (registro & 1)
      cuantos++;
    registro >>= 1;
  }
  return cuantos;
}

int main() {
  int num = 0;
  std::cin >> num;
  if (num < 0)
    return 2;
  while (std::cin.good()) {
    if (num == 0)
      return 0;
    else {
      std::cout << cuantosBitsUno(num) << " 0x" << std::hex
          << std::uppercase << num << std::endl;
      std::cin >> num;
      if (num < 0)
        return 2;
    }
  }
  return 1;
}
