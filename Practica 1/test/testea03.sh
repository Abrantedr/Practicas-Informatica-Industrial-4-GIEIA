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
Resultado: 37, 34, 34, 25,
_EOF_
ejecutaOK ${FICHDAT} ${PIVOTE}

FICHDAT=datosInt2.dat
cian "\n* Fichero  '${FICHDAT}',   y pivote ${PIVOTE}"
cat > $TMPOK <<_EOF_
Pivote: 20
Resultado: 59, 88, 57, 50, 68, 71, 53, 29, 21,
_EOF_
ejecutaOK ${FICHDAT} ${PIVOTE}

FICHDAT=datosInteg3.dat
cian "\n* Fichero  '${FICHDAT}',   y pivote ${PIVOTE}"
cat > $TMPOK <<_EOF_
Pivote: 20
Resultado: 27, 29, 54, 49, 45, 32, 29, 24, 23, 25, 22, 48,
_EOF_
ejecutaOK ${FICHDAT} ${PIVOTE}


PIVOTE=-20
FICHDAT=datosEnteros1.dat
cian "\n* Fichero  '${FICHDAT}',  y pivote ${PIVOTE}"
cat > $TMPOK <<_EOF_
Pivote: -20
Resultado: -19, 37, 34, -17, 34, 10, 25, 2, 18, 10, -10,
_EOF_
ejecutaOK ${FICHDAT} ${PIVOTE}

FICHDAT=datosInt2.dat
cian "\n* Fichero  '${FICHDAT}',   y pivote ${PIVOTE}"
cat > $TMPOK <<_EOF_
Pivote: -20
Resultado: 59, 88, 57, 6, 3, 50, 68, 16, -13, 71, 53, 1, 29, 21, 8,
_EOF_
ejecutaOK ${FICHDAT} ${PIVOTE}

FICHDAT=datosInteg3.dat
cian "\n* Fichero  '${FICHDAT}',   y pivote ${PIVOTE}"
cat > $TMPOK <<_EOF_
Pivote: -20
Resultado: -17, 27, 19, 7, 15, -3, 29, -10, -15, 3, 54, 49, 45, 32, 29, 3, 24, 23, 25, 13, 22, 48, 4,
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
Resultado: 59, 88, 57, 68, 71, 53,
_EOF_
ejecutaOK ${FICHDAT} ${PIVOTE}

FICHDAT=datosInteg3.dat
cian "\n* Fichero  '${FICHDAT}',   y pivote ${PIVOTE}"
cat > $TMPOK <<_EOF_
Pivote: 50
Resultado: 54,
_EOF_
ejecutaOK ${FICHDAT} ${PIVOTE}


# ## Pivote por defecto ##############################
FICHDAT=datosEnteros1.dat
cian "\n* Fichero  '${FICHDAT}',  y pivote por defecto"
cat > $TMPOK <<_EOF_
Pivote: 5
Resultado: 37, 34, 34, 10, 25, 18, 10,
_EOF_
ejecutaOK ${FICHDAT}

FICHDAT=datosInt2.dat
cian "\n* Fichero  '${FICHDAT}',   y pivote  por defecto"
cat > $TMPOK <<_EOF_
Pivote: 5
Resultado: 59, 88, 57, 6, 50, 68, 16, 71, 53, 29, 21, 8,
_EOF_
ejecutaOK ${FICHDAT}

FICHDAT=datosInteg3.dat
cian "\n* Fichero  '${FICHDAT}',   y pivote  por defecto"
cat > $TMPOK <<_EOF_
Pivote: 5
Resultado: 27, 19, 7, 15, 29, 54, 49, 45, 32, 29, 24, 23, 25, 13, 22, 48,
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
