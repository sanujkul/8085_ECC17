JMP MAIN

# ORG 0500H
MAIN:	MVI A, C0H;
	MVI B, 80H;
main_loop:	SIM;
	CALL BITTIME;
	XRA B;
	SIM;
	CALL HAFBIT;
	XRA B;
	JMP main_loop;


;FUNCTION- BIT TIME
;Calling this subroutine will cause delay of 103us (incluing call)
BITTIME:    PUSH B;
	PUSH PSW;
	MVI B, 10H;
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
	MVI B, 05H;
hafTimeloop: DCR B;
	   JNZ bitTimeloop;

	POP PSW;
	POP B;
	RET;