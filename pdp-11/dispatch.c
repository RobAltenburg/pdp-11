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

void doRTI (void) {
    PC = pop();
    PS = pop();
    interrupt_flag = SIM_RUN;
}

void doRTT (void) {
    PC = pop();
    PS = pop();
    interrupt_flag = SIM_RUN;
    //todo: inhibit Trace Trap
}

void doBPT (void) {
    push(PS);
    push(PC);
    PC = core_read_word(14);
    PS = core_read_word(16);
}

void doIOT (void) {
    push(PS);
    push(PC);
    PC = core_read_word(20);
    PS = core_read_word(22);
}

void doRESET (void) {
    
    // Sends INIT on the UNIBUS for 10 ms. All devices on the UNI·
    // BUS are reset to their state at power up.
}

void doSPL (short opcode) {
    priority_level = opcode & 07;
}

void doCLR (short opcode) {
    write_word(opcode & 077, 0);
    ps_reset(PS_N + PS_C + PS_V);
    ps_set(PS_Z);
}

void doCLRB (short opcode) {
    write_byte (opcode & 077, 0);
    ps_reset(PS_N + PS_C + PS_V);
    ps_set(PS_Z);
}

void doINC (short opcode) {
    short temp_w;
    temp_w = read_word((opcode & 077));
    temp_w == 077777 ? ps_set(PS_V) : ps_reset(PS_V);
    temp_w++;
    write_word(opcode & 077, temp_w);
    ps_on_word (temp_w);
}

void doINCB (short opcode) {
    char temp_b;
    temp_b = read_byte((opcode & 077));
    temp_b == 0177 ? ps_set(PS_V) : ps_reset(PS_V);
    temp_b++;
    write_byte(opcode & 077, temp_b);
    ps_on_byte (temp_b);
}

void doADC (short opcode) {
    short temp_w;
    temp_w = read_word((opcode & 077));
    (temp_w == 077777) & ps_test(PS_C) ? ps_set(PS_V) : ps_reset(PS_V);
    (temp_w == 0177777) & ps_test(PS_C) ? ps_set(PS_C) : ps_reset(PS_C);
    if (ps_test (PS_C)) {
        temp_w++;
        write_word(opcode & 077, temp_w);
    }
    ps_on_word (temp_w);
}

void doSWAB (short opcode) {
    short temp_w;
    temp_w = read_word((opcode & 077));
    ps_reset(PS_C + PS_V + PS_N + PS_Z);
    write_word(opcode & 077, (temp_w << 8) + (temp_w >> 8));
    ps_reset(PS_C + PS_V);
    // todo: finish flags
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
    ps_reset(PS_N + PS_Z + PS_V);
    
    if ((result < (-2^15)) || (result > (2^15) -1)) ps_set(PS_C);
    
    if (reg_number % 2 == 0) {
        reg[reg_number + 1] = result & 0177777;
        reg[reg_number] = (result >> 16) & 0177777;
        if (reg[reg_number] + reg[reg_number + 1] == 0) ps_set(PS_Z);
        if (reg[reg_number] < 0) ps_set(PS_N);
    } else {
        reg[reg_number] = result & 0177777;
        ps_on_word (reg[reg_number]);
    }
    
}

// two operarnd instructions

void doMOV (short opcode) {
    short temp_w = read_word((opcode & 07700) >> 6);
    write_word(opcode & 0177, temp_w);
    ps_on_word (temp_w);
    ps_reset(PS_V);
}


void dispatch (short opcode) {
    
    
    switch (opcode >> 1) {
        case 000:  // Zero Operand Instructions
            switch (opcode) {
                case HALT: printf("Halt\n"); interrupt_flag = SIM_HALT; break;
                case WAIT: interrupt_flag = SIM_WAIT; break;
                case RTI: doRTI(); break;
                case BPT: doBPT(); break;
                case IOT: doIOT(); break;
                case RTT: doRTT(); break;
                case RESET: doRESET(); break;
            }
            break;
            
        default:
            switch (opcode & HALF_OP) {
                case RTS:   doRTS (opcode); break;
                case SPL:   doSPL (opcode); break;
                default:
                    
                    switch (opcode & ONE_OP) { // One Operand Instructions
                        case JMP:   reg[7] = read_word(opcode); break;
                            
                        case CLR:   doCLR (opcode); break;
                        case CLRB:  doCLRB (opcode); break;
                        case INC:   doINC (opcode); break;
                        case INCB:  doINCB (opcode); break;
                        case ADC:   doADC (opcode); break;
                        case SWAB:  doSWAB (opcode); break;
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
                                            switch (opcode) {
                                                case CLC: ps_reset(opcode - 0240); break;
                                                case CLV: ps_reset(opcode - 0240); break;
                                                case CLZ: ps_reset(opcode - 0240); break;
                                                case CLN: ps_reset(opcode - 0240); break;
                                                case CCC: ps_reset(PS_C + PS_V + PS_Z + PS_N); break;
                                                case SEC: ps_set(opcode - 0260); break;
                                                case SEV: ps_set(opcode - 0260); break;
                                                case SEZ: ps_set(opcode - 0260); break;
                                                case SEN: ps_set(opcode - 0260); break;
                                                case SCC: ps_set(PS_C + PS_V + PS_Z + PS_N); break;
                                                    
                                                default:
                                                    printf ("WARNING: Unknown opcode (%o), treating it like NOP.\n", opcode);
                                                    break;
                                            }
                                            break;
                                    }
                                    break;
                            }
                            break;
                    }
                    break;
            }
            break;
    }
    
    
}




