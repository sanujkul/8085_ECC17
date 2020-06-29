
;FUNCTION- SID DATA
;This function returns a bit in B register, Then main function need to store it in memory
SIDATA: 	PUSH PSW;
	
	RIM;   Read SID Line
	RAL;   Put this bit in CY flag
	JC SIDATA; if bit = 1, this means not started
	                ;if bit = 0; this means start bit	
	CALL HAFBIT; We got start bit; wait for half bit time
	
	MVI C, 09H; Counter for 9 bits

NXTBIT: 	CALL BITTIME; Wait for 1 bit period
	RIM;
	RAL;
	DCR C;
	JZ OVER;
	
	MOV A,B;
	RAR;
	MOV B,A;
	JMP NXTBIT;

OVER:	POP PSW
	RET


;FUNCTION- BIT TIME
;Calling this subroutine will cause delay of 103us (incluing call)
BITTIME:    PUSH B;
	PUSH PSW;
	MVI B, 11H;
bitTimeloop: DCR B;
	   JNZ bitTimeloop

	   MVI D, D; Replacement of NOP
 	   POP PSW;
 	   POP B;
	   RET;

;FUNCTION- HAFBIT (Half bit delay)
;Calling this subroutine will cause delay of 103us (incluing call)
HAFBIT:    PUSH B;
	PUSH PSW;
	MVI B, 06H;
hafTimeloop: DCR B;
	   JNZ bitTimeloop;

	POP PSW;
	POP B;
	RET;
