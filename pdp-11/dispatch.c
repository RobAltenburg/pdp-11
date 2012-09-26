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
    char temp_b;
    
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

    // One Operand Instructions
   
    else if ((opcode & ONE_OP) == CLR ) {
        write_word(opcode & 077, 0);
        psw_reset(PSW_N + PSW_C + PSW_V);
        psw_set(PSW_Z);
    }
    
    else if ((opcode & ONE_OP) == CLRB ) {
        write_byte (opcode & 077, 0);
        psw_reset(PSW_N + PSW_C + PSW_V);
        psw_set(PSW_Z);
    }
    
   
    else if ((opcode & ONE_OP) == INC ) {
        temp_w = read_word((opcode & 077));
        temp_w == 077777 ? psw_set(PSW_V) : psw_reset(PSW_V);
        temp_w++;
        write_word(opcode & 077, temp_w);
        psw_on_word (temp_w);
    }
    
    else if ((opcode & ONE_OP) == INCB ) {
        temp_b = read_byte((opcode & 077));
        temp_b == 0177 ? psw_set(PSW_V) : psw_reset(PSW_V);
        temp_b++;
        write_byte(opcode & 077, temp_b);
        psw_on_byte (temp_b);
    }
    
    else if ((opcode & ONE_OP) == ADC ) {
        temp_w = read_word((opcode & 077));
        (temp_w == 077777) & psw_test(PSW_C) ? psw_set(PSW_V) : psw_reset(PSW_V);
        (temp_w == 0177777) & psw_test(PSW_C) ? psw_set(PSW_C) : psw_reset(PSW_C);
        if (psw_test (PSW_C)) {
            temp_w++;
            write_word(opcode & 077, temp_w);
        }
        psw_on_word (temp_w);
    }
    
    // Two Operand Instructions.
    
    else if ((opcode & TWO_OP) == MOV) { 
        temp_w = read_word((opcode & 07700) >> 6);
        write_word(opcode & 0177, temp_w);
        psw_on_word (temp_w);
        psw_reset(PSW_V);
        
        
    }
    
    // Other Transfer of Control Instructions
    
    else if ((opcode & ONE_OP) == JMP) {
        reg[7] = get_address(opcode);
        //printf ("JMP %o\n", reg[7]);
    }
    
    else {
        printf ("WARNING: Unknown opcode (%o), treating it like NOP.\n", opcode);
    }
    
    
    return 0; // 
    
}