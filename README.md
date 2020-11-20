# Practicas-Informatica-Industrial-4-GIEIA
## Práctica 2
1. Copia las clases `Mensaje`, `ModbusRTU`, `ModbusTCP`, `ModbusTCP2` y `ModbusTCP2Multiple` en el directorio `FicherosP2`
2. Abre un terminal y escribe `make fail`
3. Resultado:

==== prueba01 CORRECTA   "Mensaje: probando toString()"\
==== prueba02 CORRECTA   "Mensaje: Probando getByteAt() setByteAt() pushByte_back()"\
==== prueba03 CORRECTA   "Mensaje: Probando getWordAt()"\
==== prueba04 CORRECTA   "Mensaje: probando setWordAt()"\
==== prueba05 CORRECTA   "Mensaje: probando pushWord_back()"\
==== prueba06 CORRECTA   "Mensaje: probando crcOK()"\
==== prueba07 CORRECTA   "Mensaje: probando aniadeCRC()"\
==== prueba08 CORRECTA   "Mensaje: probando sobrecarga <<"\
==== prueba09 CORRECTA   "Mensaje: probando out_of_range"\
==== prueba11 CORRECTA   "ModbusRTU: lectura salidas analógicas (función 0x03)"\
==== prueba12 CORRECTA   "ModbusRTU: lectura de salidas digitales (función 0x01)"\
==== prueba13 CORRECTA   "ModbusRTU: Escritura digital salida única (función 0x05)"\
==== prueba14 CORRECTA   "ModbusRTU: Escrituras de salidas digitales (función 0x0F)"\
==== prueba15 CORRECTA   "ModbusRTU: Escritura salidas analógica única (función 0x06)"\
==== prueba16 CORRECTA   "ModbusRTU: Escritura salidas analógicas múltiples (función 0x10)"\
==== prueba21 CORRECTA   "ModbusRTU: error de ID y CRC"\
==== prueba22 CORRECTA   "ModbusRTU: error función inválida"\
==== prueba23 CORRECTA   "ModbusRTU: error fuera de rango"\
==== prueba24 CORRECTA   "ModbusRTU: error mal formado"\
==== prueba31 CORRECTA   "ModbusRTU: contadores analógicos (0x04) y digitales (0x02)"\
==== prueba32 CORRECTA   "ModbusRTU: entradas analógicas y digitales del 14 al 20"\
==== prueba33 CORRECTA   "ModbusRTU: entradas analógicas y digitales del 10 al 13"\
==== prueba34 CORRECTA   "ModbusRTU: entradas analógicas y digitales del 4 al 9"\
==== prueba35 CORRECTA   "ModbusRTU: entradas digitales del 4 al 20"\
==== prueba36 CORRECTA   "ModbusRTU: errores entradas digitales y analógicas"\
==== prueba41 CORRECTA   "ModbusTCP: lectura salidas (funciones 0x03 y 0x01)"\
==== prueba42 CORRECTA   "ModbusTCP: Escribimos digitales únicas y multiples (0x05 y 0x0F)"\
==== prueba43 CORRECTA   "ModbusTCP: Escribimos analógicas únicas y multiples (0x06 y 0x10)"\
==== prueba44 CORRECTA   "ModbusTCP: Errores registros de salida."\
==== prueba45 CORRECTA   "ModbusTCP: Lectura registros de entrada (funciones 02 y 04)"\
==== prueba46 CORRECTA   "ModbusTCP: Errores lectura registros de entrada"\
==== prueba47 CORRECTA   "ModbusTCP: Excepciones puerto restringido"\
==== prueba48 CORRECTA   "ModbusTCP: Paquete muy grande."\
==== prueba49 CORRECTA   "ModbusTCP: Numero de clientes."\
==== prueba51 CORRECTA   "ModbusTCP2: Escribimos digitales únicas y multiples (0x05 y 0x0F)"\
==== prueba61 CORRECTA   "ModbusTCP2Multiple: Escribimos digitales únicas y multiples (0x05 y 0x0F)"\
==== prueba62 CORRECTA   "ModbusTCP2Multiple: Escribimos digitales únicas y multiples (0x05 y 0x0F)"

4. Realiza una sola prueba escribiendo `make exe<número de la prueba>` (Ej. `make exe42`)
