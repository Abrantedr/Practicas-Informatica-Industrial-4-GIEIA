#!/bin/bash

# Scritp para la prueba del ejercicio01 de la Práctica 1 de Inf. Ind.
# Curso 19-20

GPP=g++

# Limitamos procesos hijos, por si se descontrolan
ulimit -v 3000000 -t 5

rojo() { echo -e "$(tput setaf 1)$@$(tput setaf 9)"; }
verde() { echo -e "$(tput setaf 2)$@$(tput setaf 9)"; }
amarillo()  { echo -e "$(tput setaf 3)$@$(tput setaf 9)"; }
azul()  { echo -e "$(tput setaf 4)$@$(tput setaf 9)"; }
magenta()  { echo -e "$(tput setaf 5)$@$(tput setaf 9)"; }
cian()  { echo -e "$(tput setaf 6)$@$(tput setaf 9)"; }



if [ $# -lt 1 ]
then
  rojo "\n$0: Falta primer argumento con fichero fuente testear\n"
  exit 1
fi

FICHERO_CPP=$1
EJECUTA=ejecutable$$
TMPF=tempfile$$
TMPOK=tempfileOK$$
TMPMA=tempfileMA$$
TMPME=tempfileME$$


if [ ! -r $FICHERO_CPP ]
then
    rojo "\n$0: No puedo acceder al fichero $FICHERO_CPP\n"
    exit 2
fi

CMD="$GPP --std=c++14 -Wall -o $EJECUTA $FICHERO_CPP"
magenta "\nPasamos a compilar en fuente ==============="

verde $CMD

$CMD
if [ $? -ne 0 ]
then
    rojo "\n$0: Falló la compilación\n"
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
            verde "CORRECTO: código devuelto ${Devuelto} es != 0"
        else
            rojo "Error: La ejecución debe ser INCORRECTA pero devuelve  0"
            let ERRORES++
        fi
    fi
}

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


# #########################################

cian "\n* Faltan argumentos"
ejecutaNoOK

FICHDAT=NoExito.NO
cian "\n* Fichero '${FICHDAT}'"
ejecutaNoOK ${FICHDAT}

PIVOTE=20
cian "\n* Fichero  '${FICHDAT}' y pivote ${PIVOTE}"
ejecutaNoOK ${FICHDAT} ${PIVOTE}

PIVOTE=20
FICHDAT=datosEnteros1.dat
cian "\n* Fichero  '${FICHDAT}',  y pivote ${PIVOTE}"
cat > $TMPOK <<_EOF_
Pivote: 20
Resultado: 37, 34, 34, 25, 34, 31, 25, 44, 43,
_EOF_
ejecutaOK ${FICHDAT} ${PIVOTE}

FICHDAT=datosInt2.dat
cian "\n* Fichero  '${FICHDAT}',   y pivote ${PIVOTE}"
cat > $TMPOK <<_EOF_
Pivote: 20
Resultado: 51, 38, 59, 88, 57, 49, 92, 23, 50, 23, 68, 78, 71, 53, 76, 29, 50, 21, 65,
_EOF_
ejecutaOK ${FICHDAT} ${PIVOTE}

FICHDAT=datosInteg3.dat
cian "\n* Fichero  '${FICHDAT}',   y pivote ${PIVOTE}"
cat > $TMPOK <<_EOF_
Pivote: 20
Resultado: 27, 45, 51, 52, 29, 54, 49, 45, 35, 30, 32, 29, 29, 24, 23, 25, 58, 22, 26, 48, 25,
_EOF_
ejecutaOK ${FICHDAT} ${PIVOTE}


PIVOTE=-20
FICHDAT=datosEnteros1.dat
cian "\n* Fichero  '${FICHDAT}',  y pivote ${PIVOTE}"
cat > $TMPOK <<_EOF_
Pivote: -20
Resultado: -17, -19, 37, 34, 34, -11, -17, 25, 34, 31, -15, 10, 25, 2, 44, 18, 43, 10, 12, -10,
_EOF_
ejecutaOK ${FICHDAT} ${PIVOTE}

FICHDAT=datosInt2.dat
cian "\n* Fichero  '${FICHDAT}',   y pivote ${PIVOTE}"
cat > $TMPOK <<_EOF_
Pivote: -20
Resultado: -9, 51, 38, 59, 88, 57, 8, 6, 0, 3, 49, 92, 23, 50, 23, 68, 16, -13, 78, 71, 53, 1, 76, -14, 29, 50, 21, 8, 65, -13,
_EOF_
ejecutaOK ${FICHDAT} ${PIVOTE}

FICHDAT=datosInteg3.dat
cian "\n* Fichero  '${FICHDAT}',   y pivote ${PIVOTE}"
cat > $TMPOK <<_EOF_
Pivote: -20
Resultado: -17, 27, -11, 19, 45, 7, -13, 15, 51, -3, 15, 52, 29, -16, -10, -15, -9, 3, 54, -9, 49, 45, 35, 30, 32, 15, 29, 3, 29, 24, 23, 9, 25, 58, 13, 22, 26, 48, -6, 4, 25,
_EOF_
ejecutaOK ${FICHDAT} ${PIVOTE}



PIVOTE=50
FICHDAT=datosEnteros1.dat
cian "\n* Fichero  '${FICHDAT}',  y pivote ${PIVOTE}"
cat > $TMPOK <<_EOF_
Pivote: 50
Resultado:
_EOF_
ejecutaOK ${FICHDAT} ${PIVOTE}

FICHDAT=datosInt2.dat
cian "\n* Fichero  '${FICHDAT}',   y pivote ${PIVOTE}"
cat > $TMPOK <<_EOF_
Pivote: 50
Resultado: 51, 59, 88, 57, 92, 68, 78, 71, 53, 76, 65,
_EOF_
ejecutaOK ${FICHDAT} ${PIVOTE}

FICHDAT=datosInteg3.dat
cian "\n* Fichero  '${FICHDAT}',   y pivote ${PIVOTE}"
cat > $TMPOK <<_EOF_
Pivote: 50
Resultado: 51, 52, 54, 58,
_EOF_
ejecutaOK ${FICHDAT} ${PIVOTE}


# ## Pivote por defecto ##############################
FICHDAT=datosEnteros1.dat
cian "\n* Fichero  '${FICHDAT}',  y pivote por defecto"
cat > $TMPOK <<_EOF_
Pivote: 2
Resultado: 37, 34, 34, 25, 34, 31, 10, 25, 44, 18, 43, 10, 12,
_EOF_
ejecutaOK ${FICHDAT}

FICHDAT=datosInt2.dat
cian "\n* Fichero  '${FICHDAT}',   y pivote  por defecto"
cat > $TMPOK <<_EOF_
Pivote: 2
Resultado: 51, 38, 59, 88, 57, 8, 6, 3, 49, 92, 23, 50, 23, 68, 16, 78, 71, 53, 76, 29, 50, 21, 8, 65,
_EOF_
ejecutaOK ${FICHDAT}

FICHDAT=datosInteg3.dat
cian "\n* Fichero  '${FICHDAT}',   y pivote  por defecto"
cat > $TMPOK <<_EOF_
Pivote: 2
Resultado: 27, 19, 45, 7, 15, 51, 15, 52, 29, 3, 54, 49, 45, 35, 30, 32, 15, 29, 3, 29, 24, 23, 9, 25, 58, 13, 22, 26, 48, 4, 25,
_EOF_
ejecutaOK ${FICHDAT}


# ## Pivotes incorrectos #######################################

FICHDAT=datosEnteros1.dat
PIVOTE=XX
cian "\n* Fichero  '${FICHDAT}' y pivote ${PIVOTE}"
ejecutaNoOK  ${FICHDAT} ${PIVOTE}

PIVOTE=Hola
cian "\n* Fichero  '${FICHDAT}' y pivote ${PIVOTE}"
ejecutaNoOK  ${FICHDAT} ${PIVOTE}


# #########################################

magenta "\nPruebas terminadas ============\n"
if [ $ERRORES -gt 0 ]
then
  rojo "Realizadas $PRUEBAS pruebas con $ERRORES errores\n"
else
  verde "Realizadas $PRUEBAS pruebas con $ERRORES errores :-)\n"
fi

rm $EJECUTA
rm $TMPF
rm $TMPOK
