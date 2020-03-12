# PORTA EQU 00H
# PORTB EQU 08H
# PORTC EQU 04H
# CNTRL EQU 0CH

# ORG 0000H
JMP MAIN

# ORG 0034H
JMP ISR6_5

# ORG 003CH
JMP ISR7_5

# ORG 0500H
MAIN:	;THIS CODE INITIALIZES LCD AND 8255
	EI
	LXI SP, B000;
	MVI A, 09H;
	SIM;	

	MVI A, 80H; ALL PORTS IN OUTPUT MODE
	OUT CNTRL;
	MVI A, 00H; EN = LOW (PC0 = EN = 0)
	OUT CNTRL;

INIT:	MVI A, 30H; PRECAUTIONARY CODE
	MOV B,A; SAVE THE CODE
	CALL OUTPUT; DB7 CANT BE CHECKED WRITE NOW
	CALL CMDOUT;
	CALL CMDOUT;
	;(Actual initialisation) SETTING CONTROL REG
	MVI A, 38H;
	CALL CMDOUT;
	MVI A, 08H; Display off
	CALL CMDOUT;
	MVI A, 01H; CODE FOR CLEAR DISPLAY
	CALL CMDOUT;
	MVI A, 07H; ENTRY MODE SET; SHIFTING ON
	CALL CMDOUT;
	MVI A, 0EH; TURN ON CURSOR, DISPLAY, BLINK
	CALL CMDOUT;

DISPLAY: 	MVI A, 8FH; WRITE TO 7TH LOCATION
	CALL CMDOUT;
	MVI A, 47H;   	Print G
	CALL DTAOUT;
	MVI A, 50H;  	Print P
	CALL DTAOUT;
	MVI A, 53H;	Print S
	CALL DTAOUT;
	MVI A, 20H;	Print " "
	CALL DTAOUT;
	MVI A, 47H; `	Print G
	CALL DTAOUT;
	MVI A, 55H;	Print U
	CALL DTAOUT;
	MVI A, 49H;	Print I
	CALL DTAOUT;
	MVI A, 44H;	Print D
	CALL DTAOUT;
	MVI A, 45H;	Print E
	CALL DTAOUT;
	MVI A, 52H;	Print R
	CALL DTAOUT
	


MAIN_LOOP: ; Read sid data and print onLCD
GPSREAD:	CALL SIDATA; Data stored in B register

PRINTGPS:	MOV A,B;
	CALL DTAOUT
	
	JMP MAIN_LOOP;
	
	HLT;
;===========================================
;================LCD  FUNCTIONS===============
;===========================================
CMDOUT: 	MOV B,A; SAVE DATA BYTE
	CALL CHKDB7; CALLING CHKDB7
OUTPUT:	MVI A, 02H; Select data Reg, SETTING PC1 = RS = 0
	OUT CNTRL;
	MVI A, 04h; Enable Write, PC2 = R/W = 0;
	OUT CNTRL;
	MVI A, 01H; Set Enable HIGH
	OUT CNTRL;
	MOV A,B;    Get the data
	OUT PORTB;
	MVI A, 00H;  Set EN LOW
	OUT CNTRL;
	RET;


DTAOUT: ;	CALL SCROLLDELAY
	MOV B,A; SAVE DATA BYTE
	CALL CHKDB7; CALLING CHKDB7
	MVI A, 03H; Select data Reg, SETTING PC1 = RS = 1
	OUT CNTRL;
	MVI A, 04h; Enable Write, PC2 = R/W = 0;
	OUT CNTRL;
	MVI A, 01H; Set Enable HIGH
	OUT CNTRL;
	MOV A,B;    Get the data
	OUT PORTB;
	MVI A, 00H; Set EN LOW
	OUT CNTRL;
	RET;

CHKDB7:	MVI A, 92H; Set up portB as inputPort
	OUT CNTRL
	MVI A, 02H; Seting PC1 = RS = 0;
	OUT CNTRL;
	MVI A, 05H; PC2 = R/w = 1
	OUT CNTRL;
READ:	MVI A, 01H; SET ENABLE HIGH
	OUT CNTRL;
	IN PORTB;	 READ PORTB AND CHECK DB7
	RLC; 
	MVI A, 00H;  SETTING ENABLE LOW
	OUT CNTRL;	
	JC READ;	 IF DB7=1, GO BACK AND CHECK AGAIN
	MVI A, 80H; SET UP PORTB AS OUTPUT AGAIN
	OUT CNTRL
	RET;

SCROLLDELAY: PUSH PSW;
	PUSH D;
	LXI D,7A30; FOR 250MS delay	
SCROLLDELAYLoop:
	   DCX D
	   MOV A,D
	   ORA E
	   JNZ SCROLLDELAYLoop
	POP D
	POP PSW
	   RET

;===========================================
;============= ISR ===========================
;===========================================
ISR7_5: 	PUSH PSW;
	PUSH D;
	PUSH B;
	
	MVI A, 20H;
	CALL DTAOUT;
	MVI A, 20H;
	CALL DTAOUT;
	MVI A, 20H;
	CALL DTAOUT;
	MVI A, 20H;
	CALL DTAOUT;
	
	MVI A, 53H;
	CALL DTAOUT;
	MVI A, 41H;
	CALL DTAOUT;
	MVI A, 4EH;
	CALL DTAOUT;
	MVI A, 55H;
	CALL DTAOUT;
	MVI A, 4AH;
	CALL DTAOUT;
	
	POP B;
	POP D;
	POP PSW;
	EI;
	RET;

ISR6_5: 	PUSH PSW;
	PUSH D;
	PUSH B;
	PUSH H;
	
	LXI H, A100H;
	MVI C, 04H; COUNTER TO READ 4 BYTES
PRINTGPS2:MOV A,M;
	CALL DTAOUT;
	INX H;
	DCR C;
	JNZ PRINTGPS2
	
	POP H;
	POP B;
	POP D;
	POP PSW;
	EI;
	RET;

;===========================================
;============= GPS SID READ ===================
;===========================================
;FUNCTION- SID DATA
;This function returns a byte in B register, Then main function need to store it in memory
SIDATA: 	PUSH PSW;
	PUSH D;
	RIM;   Read SID Line
	RAL;   Put this bit in CY flag
	JC SIDATA; if bit = 1, this means not started
	                ;if bit = 0; this means start bit	
	CALL HAFBIT; We got start bit; wait for half bit time
	
	MVI D, 09H; Counter for 9 bits

NXTBIT: 	CALL BITTIME; Wait for 1 bit period
	RIM;
	RAL;
	DCR D;
	JZ OVER;
	
	MOV A,B;
	RAR;
	MOV B,A;
	JMP NXTBIT;

OVER:	POP D;
	POP PSW
	RET


;FUNCTION- BIT TIME
;Calling this subroutine will cause delay of 103us (incluing call)
BITTIME:    PUSH H;
	PUSH PSW;
	MVI H, 0EH;
bitTimeloop: DCR H;
	   JNZ bitTimeloop

	   MVI D, D; Replacement of NOP
 	   POP PSW;
 	   POP H;
	   RET;

;FUNCTION- HAFBIT (Half bit delay)
;Calling this subroutine will cause delay of 103us (incluing call)
HAFBIT:    PUSH H;
	PUSH PSW;
	MVI H, 05H;
hafTimeloop: DCR H;
	   JNZ bitTimeloop;

	POP PSW;
	POP H;
	RET;

;===========================================
;============= DELAY ========================
;===========================================  
DELAY500:
	   LXI B,FFFF
DELAY500Loop:
	   DCX B
	   MOV A,B
	   ORA C
	   JNZ DELAY500Loop
	   RET
