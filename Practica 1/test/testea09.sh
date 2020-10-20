#!/bin/bash

# Scritp para la prueba del ejercicio01 de la Práctica 1 de Inf. Ind.
# Curso 18-19

GPP=g++

# Limitamos procesos hijos, por si se descontrolan
ulimit -v 3000000 -t 5

if [ $# -lt 1 ]
then
  echo "$0: Falta primer argumento con fichero fuente testear"
  exit 1
fi

FICHERO_CPP=$1
EJECUTA=ejecutable$$
TMPF=tempfile$$
TMPOK=tempfileOK$$
TMPIN=tempfileIN$$


rojo() { echo -e "$(tput setaf 1)$@$(tput setaf 9)"; }
verde() { echo -e "$(tput setaf 2)$@$(tput setaf 9)"; }
amarillo()  { echo -e "$(tput setaf 3)$@$(tput setaf 9)"; }
azul()  { echo -e "$(tput setaf 4)$@$(tput setaf 9)"; }
magenta()  { echo -e "$(tput setaf 5)$@$(tput setaf 9)"; }
cian()  { echo -e "$(tput setaf 6)$@$(tput setaf 9)"; }

if [ ! -r $FICHERO_CPP ]
then
    echo "$0: No puedo acceder al fichero $FICHERO_CPP"
    exit 2
fi

CMD="$GPP --std=c++14 -Wall -o $EJECUTA $FICHERO_CPP"
magenta "\nPasamos a compilar en fuente ==============="

echo $CMD

$CMD
if [ $? -ne 0 ]
then
    echo "$0: Falló la compilación"
    exit 2
fi

$GPP --std=c++14 -Wall -Werror  -o $EJECUTA $FICHERO_CPP &>/dev/null
if [ $? -ne 0 ]
then
    amarillo "Compilación con Warnings"
fi

magenta "\nComenzamos las pruebas ==========="
PRUEBAS=0
ERRORES=0

ejecutaOK() {

    let PRUEBAS++
    ./$EJECUTA <$TMPIN >$TMPF 2>/dev/null
    if [ $? -eq 0 ]
    then
        verde "CORRECTO: código devuelto es 0"
    else
        rojo "Error: La ejecución debe ser CORRECTO pero devuelve !=0"
        let ERRORES++
    fi

    let PRUEBAS++
    diff  --ignore-trailing-space -q $TMPOK $TMPF &>/dev/null
    if [ $? -eq 0 ]
    then
        verde "CORRECTO: lo volcado por salida estándar es correcto"
    else
        rojo "ERROR: la salida estándar debería ser"
        cat $TMPOK
        rojo "Pero lo volcado es"
        cat $TMPF
        let ERRORES++
    fi
}


# #########################################
cian "\n* Entrada 0 directamente"
cat > $TMPIN <<_EOF_
0
_EOF_
cat > $TMPOK <<_EOF_
0 0x0
_EOF_
ejecutaOK

cian "\n* Números potencia de 2"
cat > $TMPIN <<_EOF_
1
2
4
8
16
32
1024
0
_EOF_
cat > $TMPOK <<_EOF_
1 0x1
10 0x2
100 0x4
1000 0x8
10000 0x10
100000 0x20
10000000000 0x400
0 0x0
_EOF_
ejecutaOK

cian "\n* Números de 2 bits"
cat > $TMPIN <<_EOF_
3
6
12
5
10
9
18
33
1032
0
_EOF_
cat > $TMPOK <<_EOF_
11 0x3
110 0x6
1100 0xc
101 0x5
1010 0xa
1001 0x9
10010 0x12
100001 0x21
10000001000 0x408
0 0x0
_EOF_
ejecutaOK

cian "\n* Números de 3 bits"
cat > $TMPIN <<_EOF_
7
14
11
13
273
0
_EOF_
cat > $TMPOK <<_EOF_
111 0x7
1110 0xe
1011 0xb
1101 0xd
100010001 0x111
0 0x0
_EOF_
ejecutaOK

cian "\n* Números de muchos bits"
cat > $TMPIN <<_EOF_
61
65535
437
0
_EOF_
cat > $TMPOK <<_EOF_
111101 0x3d
1111111111111111 0xffff
110110101 0x1b5
0 0x0
_EOF_
ejecutaOK


# #########################################

cian "\n* Entrada incorrecta"
let PRUEBAS++
./$EJECUTA &>/dev/null <<_EOF_
4
xx
_EOF_
Devuelto=$?
if [ $Devuelto -eq 134 ]
then
    rojo "ERROR: el programa fue abortado"
    let ERRORES++
else
    if [ $Devuelto -ne 0 ]
    then
        verde "CORRECTO: código devuelto es != 0"
    else
        rojo "Error: La ejecución debe ser INCORRECTA pero devuelve  0"
        let ERRORES++
    fi
fi

# #########################################

magenta "\nPruebas terminadas ============"
if [ $ERRORES -gt 0 ]
then
  rojo "Realizadas $PRUEBAS pruebas con $ERRORES errores\n"
else
  verde "Realizadas $PRUEBAS pruebas con $ERRORES errores :-)\n"
fi

rm $EJECUTA
rm $TMPF
rm $TMPOK
rm $TMPIN
