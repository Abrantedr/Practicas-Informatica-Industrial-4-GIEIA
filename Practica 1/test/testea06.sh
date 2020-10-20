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
    Nombre=$(echo $1 | sed -e's/\.dat$//')
    FICHMA="${Nombre}_mayores.dat"
    FICHME="${Nombre}_menores.dat"
    rm -f $FICHMA $FICHME

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
    let PRUEBAS++
    if [ -r  $FICHMA ]
    then
        diff  --ignore-trailing-space -q $TMPMA $FICHMA &>/dev/null
        if [ $? -eq 0 ]
        then
            verde "CORRECTO: el fichero de mayores"
        else
            rojo "ERROR: el fichero de mayores debería contener"
            cat $TMPMA
            rojo "Pero contiene"
            cat $FICHMA
            let ERRORES++
        fi
    else
        rojo "No existe el fichero de mayores: -$FICHMA-"
        let ERRORES++
    fi

    let PRUEBAS++
    if [ -r  $FICHME ]
    then
        diff  --ignore-trailing-space -q $TMPME $FICHME &>/dev/null
        if [ $? -eq 0 ]
        then
            verde "CORRECTO: el fichero de menores"
        else
            rojo "ERROR: el fichero de menores debería contener"
            cat $TMPME
            rojo "Pero contiene"
            cat $FICHME
            let ERRORES++
        fi
    else
        rojo "No existe el fichero de mayores: -$FICHME-"
        let ERRORES++
    fi
    rm -f $FICHMA $FICHME
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
cat > $TMPMA <<_EOF_
37
34
34
25
34
31
25
44
43
_EOF_
cat > $TMPME <<_EOF_
-38
-17
-19
-45
-11
-17
-35
-48
-34
-47
-40
-15
10
-46
-24
2
18
-27
-34
10
12
-10
-43
_EOF_
cat > $TMPOK <<_EOF_
Pivote: 20
Iguales: 0
_EOF_
ejecutaOK ${FICHDAT} ${PIVOTE}

FICHDAT=datosInt2.dat
cian "\n* Fichero  '${FICHDAT}',   y pivote ${PIVOTE}"
cat > $TMPMA <<_EOF_
51
38
59
88
57
49
92
23
50
23
68
78
71
53
76
29
50
21
65
_EOF_
cat > $TMPME <<_EOF_
-65
-9
-62
-34
-51
-88
-55
-82
-47
-36
8
6
-78
-96
0
3
-35
-91
-30
16
-62
-13
-66
-92
1
-51
-42
-78
-14
-87
8
-89
-13
_EOF_
cat > $TMPOK <<_EOF_
Pivote: 20
Iguales: 0
_EOF_
ejecutaOK ${FICHDAT} ${PIVOTE}

FICHDAT=datosInteg3.dat
cian "\n* Fichero  '${FICHDAT}',   y pivote ${PIVOTE}"
cat > $TMPMA <<_EOF_
27
45
51
52
29
54
49
45
35
30
32
29
29
24
23
25
58
22
26
48
25
_EOF_
cat > $TMPME <<_EOF_
-17
-41
-11
19
7
-13
15
-3
-38
-37
15
-25
-38
-26
-16
-24
-49
-10
-29
-15
-9
3
-25
-29
-24
-9
-54
-44
-22
-20
15
-36
3
-35
9
-40
-40
13
-20
-6
4
_EOF_
cat > $TMPOK <<_EOF_
Pivote: 20
Iguales: 0
_EOF_
ejecutaOK ${FICHDAT} ${PIVOTE}


PIVOTE=-20
FICHDAT=datosEnteros1
cian "\n* Fichero  '${FICHDAT}',  y pivote ${PIVOTE}"
cat > $TMPMA <<_EOF_
-17
-19
37
34
34
-11
-17
25
34
31
-15
10
25
2
44
18
43
10
12
-10
_EOF_
cat > $TMPME <<_EOF_
-38
-45
-35
-48
-34
-47
-40
-46
-24
-27
-34
-43
_EOF_
cat > $TMPOK <<_EOF_
Pivote: -20
Iguales: 0
_EOF_
ejecutaOK ${FICHDAT} ${PIVOTE}

FICHDAT=datosInt2
cian "\n* Fichero  '${FICHDAT}',   y pivote ${PIVOTE}"
cat > $TMPMA <<_EOF_
-9
51
38
59
88
57
8
6
0
3
49
92
23
50
23
68
16
-13
78
71
53
1
76
-14
29
50
21
8
65
-13
_EOF_
cat > $TMPME <<_EOF_
-65
-62
-34
-51
-88
-55
-82
-47
-36
-78
-96
-35
-91
-30
-62
-66
-92
-51
-42
-78
-87
-89
_EOF_
cat > $TMPOK <<_EOF_
Pivote: -20
Iguales: 0
_EOF_
ejecutaOK ${FICHDAT} ${PIVOTE}

FICHDAT=datosInteg3
cian "\n* Fichero  '${FICHDAT}',   y pivote ${PIVOTE}"
cat > $TMPMA <<_EOF_
-17
27
-11
19
45
7
-13
15
51
-3
15
52
29
-16
-10
-15
-9
3
54
-9
49
45
35
30
32
15
29
3
29
24
23
9
25
58
13
22
26
48
-6
4
25
_EOF_
cat > $TMPME <<_EOF_
-41
-38
-37
-25
-38
-26
-24
-49
-29
-25
-29
-24
-54
-44
-22
-36
-35
-40
-40
_EOF_
cat > $TMPOK <<_EOF_
Pivote: -20
Iguales: 2
_EOF_
ejecutaOK ${FICHDAT} ${PIVOTE}



PIVOTE=50
FICHDAT=datosEnteros1
cian "\n* Fichero  '${FICHDAT}',  y pivote ${PIVOTE}"
cat > $TMPMA <<_EOF_
_EOF_
cat > $TMPME <<_EOF_
-38
-17
-19
-45
37
34
34
-11
-17
-35
-48
25
-34
-47
34
31
-40
-15
10
-46
25
-24
2
44
18
-27
-34
43
10
12
-10
-43
_EOF_
cat > $TMPOK <<_EOF_
Pivote: 50
Iguales: 0
_EOF_
ejecutaOK ${FICHDAT} ${PIVOTE}

FICHDAT=datosInt2
cian "\n* Fichero  '${FICHDAT}',   y pivote ${PIVOTE}"
cat > $TMPMA <<_EOF_
51
59
88
57
92
68
78
71
53
76
65
_EOF_
cat > $TMPME <<_EOF_
-65
-9
-62
-34
38
-51
-88
-55
-82
-47
-36
8
6
-78
-96
0
3
49
-35
-91
23
23
-30
16
-62
-13
-66
-92
1
-51
-42
-78
-14
29
21
-87
8
-89
-13
_EOF_
cat > $TMPOK <<_EOF_
Pivote: 50
Iguales: 2
_EOF_
ejecutaOK ${FICHDAT} ${PIVOTE}

FICHDAT=datosInteg3
cian "\n* Fichero  '${FICHDAT}',   y pivote ${PIVOTE}"
cat > $TMPMA <<_EOF_
51
52
54
58
_EOF_
cat > $TMPME <<_EOF_
-17
-41
27
-11
19
45
7
-13
15
-3
-38
-37
15
-25
-38
-26
29
-16
-24
-49
-10
-29
-15
-9
3
-25
-29
-24
-9
-54
-44
49
-22
45
35
-20
30
32
15
29
-36
3
29
24
-35
23
9
-40
-40
25
13
-20
22
26
48
-6
4
25
_EOF_
cat > $TMPOK <<_EOF_
Pivote: 50
Iguales: 0
_EOF_
ejecutaOK ${FICHDAT} ${PIVOTE}


# ## Pivote por defecto ##############################
FICHDAT=datosEnteros1
cian "\n* Fichero  '${FICHDAT}',  y pivote por defecto"
cat > $TMPMA <<_EOF_
37
34
34
25
34
31
25
44
18
43
12
_EOF_
cat > $TMPME <<_EOF_
-38
-17
-19
-45
-11
-17
-35
-48
-34
-47
-40
-15
-46
-24
2
-27
-34
-10
-43
_EOF_
cat > $TMPOK <<_EOF_
Pivote: 10
Iguales: 2
_EOF_
ejecutaOK ${FICHDAT}

FICHDAT=datosInt2
cian "\n* Fichero  '${FICHDAT}',   y pivote  por defecto"
cat > $TMPMA <<_EOF_
51
38
59
88
57
49
92
23
50
23
68
16
78
71
53
76
29
50
21
65
_EOF_
cat > $TMPME <<_EOF_
-65
-9
-62
-34
-51
-88
-55
-82
-47
-36
8
6
-78
-96
0
3
-35
-91
-30
-62
-13
-66
-92
1
-51
-42
-78
-14
-87
8
-89
-13
_EOF_
cat > $TMPOK <<_EOF_
Pivote: 10
Iguales: 0
_EOF_
ejecutaOK ${FICHDAT}

FICHDAT=datosInteg3
cian "\n* Fichero  '${FICHDAT}',   y pivote  por defecto"
cat > $TMPMA <<_EOF_
27
19
45
15
51
15
52
29
54
49
45
35
30
32
15
29
29
24
23
25
58
13
22
26
48
25
_EOF_
cat > $TMPME <<_EOF_
-17
-41
-11
7
-13
-3
-38
-37
-25
-38
-26
-16
-24
-49
-10
-29
-15
-9
3
-25
-29
-24
-9
-54
-44
-22
-20
-36
3
-35
9
-40
-40
-20
-6
4
_EOF_
cat > $TMPOK <<_EOF_
Pivote: 10
Iguales: 0
_EOF_
ejecutaOK ${FICHDAT}


# ## Pivotes incorrectos #######################################

FICHDAT=datosEnteros1.dat
PIVOTE=XX
cian "\n* Fichero  '${FICHDAT}' y pivote ${PIVOTE}"
ejecutaNoOK  ${FICHDAT} ${PIVOTE}

FICHDAT=datosEnteros1
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
