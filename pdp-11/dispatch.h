//
//  dispatch.h
//  pdp-11
//
//  Created by Robert Altenburg on 9/17/12.
//  Copyright (c) 2012 Robert Altenburg. All rights reserved.
//

#ifndef pdp_11_dispatch_h
#define pdp_11_dispatch_h

short dispatch (short opcode);


// Bit Masks for Opcode patterns
#define ONE_OP              0177700
#define ONE_AND_A_HALF_OP   0177000
#define TWO_OP              0170000
#define BRANCH_INSTRUCTION  0177400

#define HALT    0000000 //
#define WAIT    0000001
#define RESET   0000005
#define NOP     0000240 //

// One Operand Instructions
#define CLR     0005000 //
#define CLRB    0105000 //
#define INC     0005200 //
#define INCB    0105200 //
#define DEC     0005300
#define DECB    0105300
#define ADC     0005500 //
#define ADCB    0105500
#define SBC     0005600
#define SBCB    0105600
#define TST     0005700
#define TSTB    0105700
#define NEG     0005400
#define NEGB    0105400
#define COM     0005100
#define COMB    0105100
#define ROR     0006000
#define RORB    0106000
#define ROL     0006100
#define ROLB    0106100
#define ASR     0006200
#define ASRB    0106200
#define ASL     0006300
#define ASLB    0106300
#define SWAB    0000300
#define SXT     0106700

// One and a Half Operand Instructions
#define MUL     0070000
#define DIV     0071000
#define ASH     0072000
#define ASHC    0073000
#define XOR     0074000

// Two Operand Instructions
#define MOV     0010000 //
#define MOVB    0110000
#define ADD     0060000
#define SUB     0160000
#define CMP     0020000
#define CMPB    0120000
#define BIS     0050000
#define BISB    0150000
#define BIC     0040000
#define BICB    0140000
#define BIT     0030000
#define BITB    0130000

#define JMP     0000100 //


#endif
