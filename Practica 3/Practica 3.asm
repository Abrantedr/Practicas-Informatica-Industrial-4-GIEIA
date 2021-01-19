.include "8515def.inc" ; Incluye el archivo donde están todos los parámetros

; Vectores de interrupción
.org $000 		rjmp RESET 		; Reset Handler
.org INT0addr 	rjmp EXT_INT0 	; IRQ0 Handler
reti 							; IRQ1 Handler
reti 							; Timer1 Capture Handler
reti 							; Timer1 Compare A Handler
reti							; Timer1 Compare B Handler
reti		 					; Timer1 Overflow Handler
reti		 					; Timer0 Overflow Handler
reti		 					; SPI Transfer Complete Handler
reti		 					; USART RX Complete Handler
reti							; UDR0 Empty Handler
reti		 					; USART TX Complete Handler
reti							; Analog Comparator Handler
reti		 					; IRQ2 Handler
reti		 					; Timer0 Compare Handler
reti							; EEPROM Ready Handler
reti							; Store Program memory Ready Handler

.cseg

RESET:									; Comienzo del programa principal
	; Inicializamos la pila
	ldi r16,high(RAMEND)
	out SPH,r16
	ldi r16,low(RAMEND)						; En la posición más alejada en memoria
	out SPL,r16
																				
	; Definimos las entradas				
	clr r16
	out DDRA, r16
	out DDRB, r16							; PORTA y PORTB son entradas
	cbi DDRD, DDD2							; PD2 es una entrada

	; Definimos las salidas
	sbi DDRD, DDD5							; PD5 es una salida

	; Configuración del pulsador
	in r16, GIMSK
	sbr r16, (1 << INT0)
	out GIMSK, r16							; Habilitamos INT0

	in r16, MCUCR
	sbr r16, (1 << ISC00) + (1 << ISC01)				; Configuramos INT0 para que dispare una interrupción
	out MCUCR, r16							; en el flanco de subida

	; Configuración de la PWM
	clr r16
	out OCR1AH, r16							; Ciclo de trabajo al 0%
	out OCR1AL, r16

	ldi r16, 0b1000_0011						; PWM no invertida, 10 bits de resolución
	out TCCR1A, r16

	in r16, TCCR1B
	sbr r16, (1 << CS10)
	out TCCR1B, r16							; Ponemos en marcha la PWM a una frecuencia de CK
									; (CK = 8 MHz)
	clr r16
	out TCNT1H, r16							; Reset del counter
	out TCNT1L, r16

	; Interrupciones		
	sei 								; Habilitamos las interrupciones

LOOP: 									; Definimos un bucle infinito
	in r16, PIND
	out PORTC, r16							; Obtenemos la PWM en PORTC
	rjmp LOOP




EXT_INT0:								; Interrupción del pulsador
	cli								; Deshabilitamos las interrupciones

	; Contador de bits
	in r16, PINA							; Leemos las entradas del puerto A
	clr r17
	clr r0
	clc
	COUNT_PINA_BITS:
		lsr r16							; Desplaza el registro a la derecha
		breq TERMINATE_PINA
		adc r17, r0						; Suma el bit de acarreo si era un 1
		rjmp COUNT_PINA_BITS
	TERMINATE_PINA:
		adc r17, r0


	in r16, PINB							; Leemos las entradas del puerto B
	clr r0								; Hacemos lo mismo que en el caso anterior
	clc
	COUNT_PINB_BITS:
		lsr r16
		breq TERMINATE_PINB
		adc r17, r0
		rjmp COUNT_PINB_BITS
	TERMINATE_PINB:
		adc r17, r0						; r17 contiene el número de switches a uno


	; Calculamos la nueva PWM					; Mediante desplazamiento de registros
	clr r16
	out OCR1AH, r16							; Desde $0000 (BOTTOM) a $03FF (TOP)

	cpi r17, $00
	breq SET_PWM
									; Empezamos por el valor más bajo que puede tomar OCR1A
	ldi r16, $02							; Ponemos a 1 el bit 1 de r16

	cpi r17, $01
	breq SET_PWM
	
	dec r17															
	SHIFT_LOW:
		lsl r16							; Lo multiplicamos por 2 tantas veces como switches a uno
		breq OVF						; Overflow cuando hay 8 bits (128 << 1 = 256 -> $00)
		dec r17
		brne SHIFT_LOW
	
	SET_PWM:							; Si r17 <= 8 switches
		out OCR1AL, r16						
		sei
		reti

	OVF:								; si r17 > 8 switches
		out OCR1AL, r16
		
		ldi r16, $01
		SHIFT_HIGH:
			lsl r16
			dec r17
			brne SHIFT_HIGH
		lsr r16
		cpi r16, $04						; Nos hemos pasado del TOP
		breq SET_MAX_PWM

		out OCR1AH, r16
		sei									
		reti										
		
		SET_MAX_PWM:
			ldi r20, $03
			ldi r21, $FF
			out OCR1AL, r21					; Ciclo de trabajo al 100%
			out OCR1AH, r20
			sei
			reti
