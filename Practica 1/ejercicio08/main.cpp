// Ejercicio 08 - All tests passed!
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

// Devuelve el formato de salida '<bits a uno> 0x<hexadecimal>'
void formatoSalida(int num) {
  int bits = cuantosBitsUno(num);
  std::cout << std::dec << bits << " 0x" << std::hex << num
      << std::endl;
}

int main() {
  int num = 0;
  std::cin >> num;
  if (num < 0)
    return 2;
  while (std::cin.good()) {
    if (num == 0) {
      formatoSalida(num);
      return 0;
    } else {
      formatoSalida(num);
      std::cin >> num;
      if (num < 0)
        return 2;
    }
  }
  return 1;
}
