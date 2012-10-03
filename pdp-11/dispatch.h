//
//  dispatch.h
//  pdp-11
//
//  Created by Robert Altenburg on 9/17/12.
//  Copyright (c) 2012 Robert Altenburg. All rights reserved.
//

#ifndef pdp_11_dispatch_h
#define pdp_11_dispatch_h

void dispatch (short opcode);


// Bit Masks for Opcode patterns
#define HALF_OP             0177770
#define ONE_OP              0177700
#define BRANCH_INSTRUCTION  0177400
#define ONE_AND_A_HALF_OP   0177000
#define TWO_OP              0170000



#define HALT    0000000
#define WAIT    0000001 
#define RTI     0000002 
#define BPT     0000003 
#define IOT     0000004 
#define RESET   0000005 // todo: finish
#define RTT     0000006 // todo: finish

#define JMP     0000100
#define RTS     0000200 

#define SPL     0000230 
#define NOP     0000240
#define CLC     0000241 
#define CLV     0000242 
#define CLZ     0000244 
#define CLN     0000250 
#define CCC     0000257 
#define SEC     0000261 
#define SEV     0000262 
#define SEZ     0000264 
#define SEN     0000270 
#define SCC     0000277 
#define SWAB    0000300 // todo: finish

#define BR      0000400 
#define BNE     0001000 // todo
#define BEQ     0001400 // todo
#define BGE     0002000 // todo
#define BLT     0002400 // todo
#define BGL     0003000 // todo
#define BLE     0003400 // todo

#define JSR     0004000 

#define CLR     0005000
#define COM     0005100 // todo
#define INC     0005200
#define DEC     0005300 // todo
#define NEG     0005400 // todo
#define ADC     0005500
#define SBC     0005600 // todo
#define TST     0005700 // todo

#define ROR     0006000 // todo
#define ROL     0006100 // todo
#define ASR     0006200 // todo
#define ASL     0006300 // todo
#define MARK    0006400 // todo
#define MFPI    0006500 // todo
#define MTPI    0006600 // todo
#define SXT     0006700 // todo

// Two Operand Instructions
#define MOV     0010000
#define CMP     0020000 // todo
#define BIT     0030000 // todo
#define BIC     0040000 // todo
#define BIS     0050000 // todo
#define ADD     0060000 // todo

// One and a Half Operand Instructions
#define MUL     0070000  
#define DIV     0071000 // todo
#define ASH     0072000 // todo
#define ASHC    0073000 // todo
#define XOR     0074000 // todo

#define FADD    0075000 // todo
#define FSUB    0075010 // todo
#define FMUL    0075020 // todo
#define FDIV    0075030 // todo

#define SOB     0077000 // todo

// Branch Instructions
#define BPL     01000000 // todo
#define BMI     01004000 // todo
#define BHI     01010000 // todo
#define BLOS    01014000 // todo
#define BVC     01020000 // todo
#define BVS     01024000 // todo
#define BCC     01030000 // todo       // BHIS
#define BCS     01034000 // todo       // BLO

// Byte Instructions
#define CLRB    0105000
#define COMB    0105100 // todo
#define INCB    0105200
#define DECB    0105300 // todo
#define NEGB    0105400 // todo
#define ADCB    0105500 // todo
#define SBCB    0105600 // todo
#define TSTB    0105700 // todo

#define RORB    0106000 // todo
#define ROLB    0106100 // todo
#define ASRB    0106200 // todo
#define ASLB    0106300 // todo

#define MFPD    0106500 // todo
#define MTPD    0106600 // todo

#define MOVB    0110000 // todo
#define CMPB    0120000 // todo
#define BITB    0130000 // todo
#define BICB    0140000 // todo
#define SUB     0160000 // todo


#endif