// Ejercicio 09
// Autor: Rubén Abrante Delgado
// Fecha: 19/10/2020

#include <iostream>
#include <string>
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

int main() {
  unsigned num = 0;
  std::cin >> num;
  if ((int) num < 0)
    return 2;
  while (std::cin.good()) {
    if (num == 0)
      return 0;
    else {
      std::cout << aBinario(num) << " 0x" << std::hex << num << std::endl;
      std::cin >> num;
      if ((int) num < 0)
        return 2;
    }
  }
  return 1;
}
