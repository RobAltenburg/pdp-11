//
//  dispatch.c
//  pdp-11
//
//  Created by Robert Altenburg on 9/17/12.
//  Copyright (c) 2012 Robert Altenburg. All rights reserved.
//

#include <stdio.h>
#include "core.h"
#include "dispatch.h"


short dispatch (short opcode) {
    
    short temp_w;
    
    // Zero Operand Instructions
    
    if (opcode == HALT) {
        // Halt CPU until restarted; abort i/o
        printf("Halt\n");
        return 1;
    }
    else if (opcode == NOP) {
        // No operation
        printf("NOP\n");
    }
    
   
    else if ((opcode & ONE_OP) == CLR ) {
     
        write_word(opcode & 077, 0);
        psw_reset(PSW_N + PSW_C + PSW_V);
        psw_set(PSW_Z);
         

    }
    
    else if ((opcode & ONE_OP) == CLRB ) {
        
        write_byte (opcode & 077, 0);
        
    }
    
    
    // Two Operand Instructions.
    
    else if ((opcode & TWO_OP) == MOV) { // MOV
        
        temp_w = read_word((opcode & 07700) >> 6);
        write_word(opcode & 077, temp_w);
 

        
    }
    
    // Other Transfer of Control Instructions
    
    else if ((opcode & ONE_OP) == JMP) { // JMP
        reg[7] = get_address(opcode);
        //printf ("JMP %o\n", reg[7]);
    }
    
    else {
        printf ("WARNING: Unknown opcode (%o), treating it like NOP.\n", opcode);
    }
    
    
    return 0; // 
    
}