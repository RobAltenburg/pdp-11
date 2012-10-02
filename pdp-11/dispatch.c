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

void doCLR (short opcode) {
    write_word(opcode & 077, 0);
    psw_reset(PSW_N + PSW_C + PSW_V);
    psw_set(PSW_Z);
}

void doCLRB (short opcode) {
    write_byte (opcode & 077, 0);
    psw_reset(PSW_N + PSW_C + PSW_V);
    psw_set(PSW_Z);
}

void doINC (short opcode) {
    short temp_w;
    temp_w = read_word((opcode & 077));
    temp_w == 077777 ? psw_set(PSW_V) : psw_reset(PSW_V);
    temp_w++;
    write_word(opcode & 077, temp_w);
    psw_on_word (temp_w);
}

void doINCB (short opcode) {
    char temp_b;
    temp_b = read_byte((opcode & 077));
    temp_b == 0177 ? psw_set(PSW_V) : psw_reset(PSW_V);
    temp_b++;
    write_byte(opcode & 077, temp_b);
    psw_on_byte (temp_b);
}

void doADC (short opcode) {
    short temp_w;
    temp_w = read_word((opcode & 077));
    (temp_w == 077777) & psw_test(PSW_C) ? psw_set(PSW_V) : psw_reset(PSW_V);
    (temp_w == 0177777) & psw_test(PSW_C) ? psw_set(PSW_C) : psw_reset(PSW_C);
    if (psw_test (PSW_C)) {
        temp_w++;
        write_word(opcode & 077, temp_w);
    }
    psw_on_word (temp_w);
}

// one and a half operand instructions

void doJSR (short opcode) {  
    short temp_w = read_word(opcode);
    char temp_r = (opcode & 0700) >> 6;
    push(reg[temp_r]);
    if (temp_r != 7) reg[temp_r] = PC;
    PC = temp_w;
}

void doRTS (short opcode) {  
    char temp_r = opcode & 07;
    if (temp_r != 7) PC = reg[temp_r];
    reg[temp_r] = pop();
}

void doMUL (short opcode) {
    char reg_number = (opcode & 0700) >> 6;
    int result = (int) read_word((opcode & 0700) >> 6) * read_word(opcode & 077);
    //int temp_int = reg[temp_register] * read_word(opcode & 077); // porting: make sure int is at least 32-bits
    reg[reg_number] = result & 0177777;
    if (reg_number % 2 == 0) {
        reg[reg_number + 1] = result & 0177777;
        reg[reg_number] = (result >> 16) & 0177777;
    } else {
        reg[reg_number] = result & 0177777;
    }

}

// two operarnd instructions

void doMOV (short opcode) {
    short temp_w = read_word((opcode & 07700) >> 6);
    write_word(opcode & 0177, temp_w);
    psw_on_word (temp_w);
    psw_reset(PSW_V);
}


short dispatch (short opcode) {
    
  
    switch (opcode >> 1) {
        case 000:  // Zero Operand Instructions
            switch (opcode) {
                case HALT: printf("Halt\n"); return 1; break;
            }
            break;
            
        default:
            switch (opcode & ONE_OP) { // One Operand Instructions
                case JMP:   reg[7] = read_word(opcode); break;
                case RTS:   doRTS (opcode); break;
                case CLR:   doCLR (opcode); break;
                case CLRB:  doCLRB (opcode); break;
                case INC:   doINC (opcode); break;
                case INCB:  doINCB (opcode); break;
                case ADC:   doADC (opcode); break;
                default:
                    switch (opcode & ONE_AND_A_HALF_OP) {
                        case JSR: doJSR(opcode); break;
                        case MUL: doMUL(opcode); break;                            
                        default:
                            switch (opcode & TWO_OP) { // Two Operand Instructions
                                case MOV: doMOV(opcode); break;
                                case NOP:  printf("NOP\n"); break;
                                default:
                                    
                                    // Processor Status Word Instructions
                                    if ((opcode >= CLC) & (opcode <= CLN)) {
                                        psw_reset(opcode - 0240);
                                    }
                                    
                                    else if ((opcode >= SEC) & (opcode <= SEN)) {
                                        psw_set(opcode - 0260);
                                    }
                                    
                                    else if (opcode == CCC) {
                                        psw_reset(PSW_C + PSW_V + PSW_Z + PSW_N);
                                    }
                                    
                                    else if (opcode == SCC) {
                                        psw_set(PSW_C + PSW_V + PSW_Z + PSW_N);
                                    }
                                    
                                  
                                    else {
                                        printf ("WARNING: Unknown opcode (%o), treating it like NOP.\n", opcode);
                                        // todo: handle this with a trap
                                    }
                                    break;
                            }

                            break;
                    }
                    
                    break;
            }
            

            break;
    }
   

    
    return 0; //
    
}




