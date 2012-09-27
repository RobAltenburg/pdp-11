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
    char temp_b, temp_register;
    int temp_int; // porting: make sure this is at least 32-bits
   
    switch (opcode >> 1) {
        case 000:  // Zero Operand Instructions
            switch (opcode) {
                case HALT: // Halt CPU until restarted; abort i/o
                    printf("Halt\n");
                    return 1;
                    break;
            }
            break;
            
        default:
            switch (opcode & ONE_OP) { // One Operand Instructions
                case JMP:
                    reg[7] = get_address(opcode);
                    break;
                case CLR:
                    write_word(opcode & 077, 0);
                    psw_reset(PSW_N + PSW_C + PSW_V);
                    psw_set(PSW_Z);
                    break;
                case CLRB:
                    write_byte (opcode & 077, 0);
                    psw_reset(PSW_N + PSW_C + PSW_V);
                    psw_set(PSW_Z);
                    break;
                case INC:
                    temp_w = read_word((opcode & 077));
                    temp_w == 077777 ? psw_set(PSW_V) : psw_reset(PSW_V);
                    temp_w++;
                    write_word(opcode & 077, temp_w);
                    psw_on_word (temp_w);
                    break;
                case INCB:
                    temp_b = read_byte((opcode & 077));
                    temp_b == 0177 ? psw_set(PSW_V) : psw_reset(PSW_V);
                    temp_b++;
                    write_byte(opcode & 077, temp_b);
                    psw_on_byte (temp_b);
                    break;
                case ADC:
                    temp_w = read_word((opcode & 077));
                    (temp_w == 077777) & psw_test(PSW_C) ? psw_set(PSW_V) : psw_reset(PSW_V);
                    (temp_w == 0177777) & psw_test(PSW_C) ? psw_set(PSW_C) : psw_reset(PSW_C);
                    if (psw_test (PSW_C)) {
                        temp_w++;
                        write_word(opcode & 077, temp_w);
                    }
                    psw_on_word (temp_w);
                    break;
                default:
                    switch (opcode & ONE_AND_A_HALF_OP) {
                        case MUL:
                            temp_register = (opcode & 0700) >> 6;
                            temp_int = reg[temp_register] * read_word(opcode & 077);
                            if (temp_register % 2 == 0) {
                                // high
                            }
                            
                            printf("mul\n");
                            break;
                            
                        default:
                            switch (opcode & TWO_OP) { // Two Operand Instructions
                                case MOV:
                                    temp_w = read_word((opcode & 07700) >> 6);
                                    write_word(opcode & 0177, temp_w);
                                    psw_on_word (temp_w);
                                    psw_reset(PSW_V);
                                    break;
                                    
                                default:
                                    if (opcode == NOP) {
                                        // No operation
                                        printf("NOP\n");
                                    }
                                    
                                    // Processor Status Word Instructions
                                    else if ((opcode >= CLC) & (opcode <= CLN)) {
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