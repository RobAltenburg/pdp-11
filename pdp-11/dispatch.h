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

#define HALT    0000000
#define WAIT    0000001
#define RESET   0000005
#define NOP     0000240

#define MOV     0010000
#define MOVB    0110000

#define CLR     0005000 
#define CLRB    0105000 
#define INC     0005200
#define INCB    0105200
#define DEC     0005300
#define DECB    0105300


#define JMP     0000100

#endif
