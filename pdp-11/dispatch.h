//
//  dispatch.h
//  pdp-11
//
//  Created by Robert Altenburg on 9/17/12.
//  Copyright (c) 2012 Robert Altenburg. All rights reserved.
//

#ifndef pdp_11_dispatch_h
#define pdp_11_dispatch_h

#include <stdlib.h>

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
#define SWAB    0000300 

#define JSR     0004000

#define CLR     0005000
#define COM     0005100
#define INC     0005200
#define DEC     0005300 
#define NEG     0005400 
#define ADC     0005500
#define SBC     0005600 
#define TST     0005700 

#define ROR     0006000 
#define ROL     0006100 
#define ASR     0006200 
#define ASL     0006300 
#define MARK    0006400 // todo
#define MFPI    0006500 // todo
#define MTPI    0006600 // todo
#define SXT     0006700 

// Two Operand Instructions
#define MOV     0010000
#define CMP     0020000 
#define BIT     0030000 // todo
#define BIC     0040000 // todo
#define BIS     0050000 // todo
#define ADD     0060000 // todo
#define SUB     0160000 // todo

// One and a Half Operand Instructions
#define MUL     0070000  
#define DIV     0071000 
#define ASH     0072000 
#define ASHC    0073000 
#define XOR     0074000 // todo

#define FADD    0075000 // todo
#define FSUB    0075010 // todo
#define FMUL    0075020 // todo
#define FDIV    0075030 // todo

#define SOB     0077000 // todo

// Branch Instructions
#define BR      0000400
#define BNE     0001000
#define BEQ     0001400
#define BGE     0002000
#define BLT     0002400
#define BGT     0003000
#define BLE     0003400
#define BPL     0100000
#define BMI     0100400
#define BHI     0101000
#define BLOS    0101400
#define BVC     0102000
#define BVS     0102400
#define BCC     0103000
#define BHIS    0103000
#define BCS     0103400
#define BLO     0103400

// Byte Instructions
#define CLRB    0105000
#define COMB    0105100 
#define INCB    0105200
#define DECB    0105300 
#define NEGB    0105400 
#define ADCB    0105500 
#define SBCB    0105600 
#define TSTB    0105700 

#define RORB    0106000 
#define ROLB    0106100 
#define ASRB    0106200 
#define ASLB    0106300 

#define MFPD    0106500 // todo
#define MTPD    0106600 // todo

#define MOVB    0110000 
#define CMPB    0120000 
#define BITB    0130000 // todo
#define BICB    0140000 // todo



#endif