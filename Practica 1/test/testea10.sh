#!/bin/bash

# Scritp para la prueba del ejercicio10 de la Práctica 1 de Inf. Ind.
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
    ./$EJECUTA $@ >$TMPF 2>/dev/null
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

ejecutaNoOK() {
    let PRUEBAS++
    ./$EJECUTA $@ 2>/dev/null
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
}

cian "\n* Faltan argumentos"
ejecutaNoOK

cian "\n* Fichero no existe"
ejecutaNoOK NoExito.NO

# #########################################
cian "\n* Argumento por defecto datos 1"
cat > $TMPOK <<_EOF_
435091 0x6a393 0b1101010001110010011
_EOF_
ejecutaOK datosBool1

# Los 40 bits no caben en los 32 de un int
# cian "\n* Argumento por defecto datos 2"
# cat > $TMPOK <<_EOF_
# 552359706 0x20ec571a 0b100000111011000101011100011010
# _EOF_
# ejecutaOK datosBooleanos2

cian "\n* Del 0 al 3 datos 1"
cat > $TMPOK <<_EOF_
3 0x3 0b11
_EOF_
ejecutaOK datosBool1 0 3

cian "\n* Del 0 al 3 datos 2"
cat > $TMPOK <<_EOF_
10 0xa 0b1010
_EOF_
ejecutaOK datosBooleanos2 0 3

cian "\n* Del 1 al 1 datos 1"
cat > $TMPOK <<_EOF_
1 0x1 0b1
_EOF_
ejecutaOK datosBool1 1 1

cian "\n* Del 1 al 1 datos 2"
cat > $TMPOK <<_EOF_
1 0x1 0b1
_EOF_
ejecutaOK datosBooleanos2 1 1

cian "\n* Del 2 al 7 datos 1"
cat > $TMPOK <<_EOF_
36 0x24 0b100100
_EOF_
ejecutaOK datosBool1 2 7

cian "\n* Del 2 al 7 datos 2"
cat > $TMPOK <<_EOF_
6 0x6 0b110
_EOF_
ejecutaOK datosBooleanos2 2  7

cian "\n* Del 5 al 20 datos 1"
cat > $TMPOK <<_EOF_
13596 0x351c 0b11010100011100
_EOF_
ejecutaOK datosBool1 5 20

cian "\n* Del 5 al 20 datos 2"
cat > $TMPOK <<_EOF_
25272 0x62b8 0b110001010111000
_EOF_
ejecutaOK datosBooleanos2 5  20


cian "\n* De 7 sin límite datos 1"
cat > $TMPOK <<_EOF_
3399 0xd47 0b110101000111
_EOF_
ejecutaOK datosBool1 7

cian "\n* Del 7 sin límite datos 2"
cat > $TMPOK <<_EOF_
1380047022 0x5241d8ae 0b1010010010000011101100010101110
_EOF_
ejecutaOK datosBooleanos2 7

cian "\n* De 9 sin límite datos 1"
cat > $TMPOK <<_EOF_
849 0x351 0b1101010001
_EOF_
ejecutaOK datosBool1 9

cian "\n* Del 9 sin límite datos 2"
cat > $TMPOK <<_EOF_
345011755 0x1490762b 0b10100100100000111011000101011
_EOF_
ejecutaOK datosBooleanos2 9

# #########################################

cian "\n* Argumento inicio incorrecto"
ejecutaNoOK datosBool1 xx 10

cian "\n* Argumento fin incorrecto"
ejecutaNoOK datosBool1 0 xx


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
