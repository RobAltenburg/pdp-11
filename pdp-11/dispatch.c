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
    
    //short source_w, dest_w;
    //char source_b, dest_b;
    /*
     switch (opcode) {
     case 000000: // HALT
     // Halt CPU until restarted; abort i/o
     printf("Halt\n");
     return;
     break;
     case 000001: // WAIT (TODO: Right now, this is the same as HALT)
     // Halt CPU until restarted or interrupted
     printf("Wait\n");
     return;
     break;
     case 000005: // RESET
     // Reset all i/o devices
     break;
     case 000240: // NOP
     // No operation
     printf("NOP\n");
     break;
     default:
     break;
     }
     */
    
    // Zero Operand Instructions
    
    if (opcode == 0) { //HALT
        // Halt CPU until restarted; abort i/o
        printf("Halt\n");
        return 1;
    }
    else if (opcode == 0240) { // NOP
        // No operation
        //printf("NOP\n");
    }
    
    // One Operand Instructions
    
    else if ((opcode >> 6) == 050 ) { // CLR
     
        set_word(opcode & 077, 0);
        
    }
    
    // Two Operand Instructions.
    
    else if ((opcode >> 12) == 01) { // MOV
        
        set_word(opcode & 077,get_operand((opcode & 07700) >> 6));
        
    }
    
    // Other Transfer of Control Instructions
    
    else if ((opcode >> 6) == 01) { // JMP
        pdp_register[7] = get_address(opcode);
        //printf ("JMP %o\n", pdp_register[7]);
    }
    
    else {
        printf ("WARNING: Unknown opcode (%o), treating it like NOP.\n", opcode);
    }
    
    
    return 0; // 
    
}