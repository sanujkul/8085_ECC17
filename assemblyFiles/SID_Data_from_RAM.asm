LXI H, A100;
MVI M, C0H;
INX H;
MVI M, 40H;

START: 	MOV A,M;
	SIM;
	DCX H;
	MOV A,M;
	SIM;
	INX H;
	JMP START;