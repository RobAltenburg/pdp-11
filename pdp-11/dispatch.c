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

// No ops

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

//half operarnd instructions

void doRTS (short opcode) {
    char temp_r = opcode & 07;
    if (temp_r != 7) PC = reg[temp_r];
    reg[temp_r] = pop();
}

void doSPL (short opcode) {
    priority_level = opcode & 07;
}

// one operarnd instructions

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


void doCOM(short opcode) {
    short temp_w;
    temp_w = ~ read_word(opcode & 077);
    write_word(opcode & 077, temp_w);
    ps_reset(PS_V);
    ps_set(PS_C);
    ps_on_word (temp_w);
}

void doCOMB(short opcode) {
    char temp_c;
    temp_c = ~ read_byte(opcode & 077);
    write_byte(opcode & 077, temp_c);
    ps_reset(PS_V);
    ps_set(PS_C);
    ps_on_word (temp_c);
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

void doDEC (short opcode) {
    short temp_w;
    temp_w = read_word((opcode & 077));
    temp_w == 0100000 ? ps_set(PS_V) : ps_reset(PS_V);
    temp_w--;
    write_word(opcode & 077, temp_w);
    ps_on_word (temp_w);
}

void doDECB (short opcode) {
    char temp_b;
    temp_b = read_byte((opcode & 077));
    temp_b == 0200 ? ps_set(PS_V) : ps_reset(PS_V);
    temp_b--;
    write_byte(opcode & 077, temp_b);
    ps_on_byte (temp_b);
}

void doNEG (short opcode) {
    short temp_w;
    temp_w = read_word((opcode & 077));
    temp_w == 0100000 ? ps_set(PS_V) : ps_reset(PS_V);
    temp_w = -temp_w;
    write_word(opcode & 077, temp_w);
    ps_on_word (temp_w);
}

void doNEGB (short opcode) {
    char temp_b;
    temp_b = read_byte((opcode & 077));
    temp_b == 0200 ? ps_set(PS_V) : ps_reset(PS_V);
    temp_b = -temp_b;
    write_byte(opcode & 077, temp_b);
    ps_on_byte (temp_b);
}

void doTST (short opcode) {
    ps_on_word (read_word((opcode & 077)));
    ps_reset(PS_V + PS_C);
}

void doTSTB (short opcode) {
    ps_on_byte(read_byte((opcode & 077)));
    ps_reset(PS_V + PS_C);    
}

void doASR (short opcode) { 
    short temp_w;
    temp_w = read_word((opcode & 077));
    temp_w & 01 ? ps_set(PS_C) : ps_reset(PS_C);
    temp_w = (temp_w & 0100000) | (temp_w >> 1);
    write_word(opcode & 077, temp_w);
    ps_on_word (temp_w);
    ps_test(PS_N) ^ ps_test(PS_C) ? ps_set(PS_V) : ps_reset(PS_V);
}

void doASRB (short opcode) { //
    short temp_b;
    temp_b = read_byte((opcode & 077));
    temp_b & 01 ? ps_set(PS_C) : ps_reset(PS_C);
    temp_b = (temp_b & 0200) | (temp_b >> 1);
    write_byte(opcode & 077, temp_b);
    ps_on_byte(temp_b);
    ps_test(PS_N) ^ ps_test(PS_C) ? ps_set(PS_V) : ps_reset(PS_V);
}

void doASL (short opcode) {
    short temp_w;
    temp_w = read_word((opcode & 077));
    temp_w & 0100000 ? ps_set(PS_C) : ps_reset(PS_C);
    temp_w = temp_w << 1;
    write_word(opcode & 077, temp_w);
    ps_on_word (temp_w);
    ps_test(PS_N) ^ ps_test(PS_C) ? ps_set(PS_V) : ps_reset(PS_V);
}

void doASLB (short opcode) { //
    short temp_b;
    temp_b = read_byte((opcode & 077));
    temp_b & 0200 ? ps_set(PS_C) : ps_reset(PS_C);
    temp_b = temp_b << 1;
    write_byte(opcode & 077, temp_b);
    ps_on_byte(temp_b);
    ps_test(PS_N) ^ ps_test(PS_C) ? ps_set(PS_V) : ps_reset(PS_V);
}


void doROR (short opcode) {
    
    short temp_w, temp_lo;
    temp_w = read_word((opcode & 077));
    temp_lo = temp_w & 01;
    temp_w = (ps_test(PS_C) << 15) | ((temp_w >> 1) & 077777);
    temp_lo  ? ps_set(PS_C) : ps_reset(PS_C);
    write_word(opcode & 077, temp_w);
    ps_on_word (temp_w);
    ps_test(PS_N) ^ ps_test(PS_C) ? ps_set(PS_V) : ps_reset(PS_V);
    
}

void doRORB (char opcode) { //
    char temp_b, temp_lo;
    temp_b = read_byte((opcode & 077));
    temp_lo = temp_b & 01;
    temp_b = temp_b >> 1;
    temp_b = (ps_test(PS_C) << 7) | ((temp_b >> 1) & 0177);
    write_byte(opcode & 077, temp_b);
    temp_lo ? ps_set(PS_C) : ps_reset(PS_C);
    ps_on_byte(temp_b);
    ps_test(PS_N) ^ ps_test(PS_C) ? ps_set(PS_V) : ps_reset(PS_V);
}

void doROL (short opcode) {
    short temp_w, temp_hi;
    temp_w = read_word((opcode & 077));
    temp_hi = temp_w & 0100000;
    temp_w = temp_w << 1 | ps_test(PS_C);
    temp_hi ? ps_set(PS_C) : ps_reset(PS_C);
    write_word(opcode & 077, temp_w);
    ps_on_word (temp_w);
    ps_test(PS_N) ^ ps_test(PS_C) ? ps_set(PS_V) : ps_reset(PS_V);
}

void doROLB (short opcode) { //
    short temp_b, temp_hi;
    temp_b = read_byte((opcode & 077));
    temp_hi = temp_b & 0200;
    temp_b = temp_b << 1 | ps_test(PS_C);
    temp_hi ? ps_set(PS_C) : ps_reset(PS_C);
    write_byte(opcode & 077, temp_b);
    ps_on_byte(temp_b);
    ps_test(PS_N) ^ ps_test(PS_C) ? ps_set(PS_V) : ps_reset(PS_V);
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
    write_word((opcode & 077), word_flip(temp_w));
    ps_reset(PS_C + PS_V);
    // todo: finish flags
}

// branch instructions

void doBR(short opcode) {
    PC = PC + ((opcode & 0377) << 1);
}

void doBNE(short opcode) {
    if  (! ps_test(PS_Z)) {
        PC = PC + ((opcode & 0377) << 1);
    }
}

void doBEQ(short opcode) {
    if  (ps_test(PS_Z)) {
        PC = PC + ((opcode & 0377) << 1);
    }
}

void doBPL(short opcode) {
    if  (! ps_test(PS_N)) {
        PC = PC + ((opcode & 0377) << 1);
    }
}

void doBMI(short opcode) {
    if  (ps_test(PS_N)) {
        PC = PC + ((opcode & 0377) << 1);
    }
}

void doBVC(short opcode) {
    if  (! ps_test(PS_V)) {
        PC = PC + ((opcode & 0377) << 1);
    }
}

void doBVS(short opcode) {
    if  (ps_test(PS_V)) {
        PC = PC + ((opcode & 0377) << 1);
    }
}

void doBCC(short opcode) {
    if  (! ps_test(PS_C)) {
        PC = PC + ((opcode & 0377) << 1);
    }
}

void doBCS(short opcode) {
    if  (ps_test(PS_C)) {
        PC = PC + ((opcode & 0377) << 1);
    }
}

void doBGE(short opcode) {
    if  ((ps_test(PS_N)==0) | (ps_test(PS_V)==0)) {
        PC = PC + ((opcode & 0377) << 1);
    }
}

void doBLT(short opcode) {  
    if  ((ps_test(PS_N)==1) | (ps_test(PS_V)==1)){
        PC = PC + ((opcode & 0377) << 1);
    }
}

void doBGT(short opcode) {  
    if  (ps_test(PS_N) ^ ps_test(PS_V)) {
        PC = PC + ((opcode & 0377) << 1);
    }
}

void doBLE(short opcode) {
    if  ((ps_test(PS_N) == 0) ^ (ps_test(PS_V) == 0)) {
        PC = PC + ((opcode & 0377) << 1);
    }
}

void doBHI(short opcode) {
    if   ((ps_test(PS_C) == 0) ^ (ps_test(PS_Z) == 0)) {
        PC = PC + ((opcode & 0377) << 1);
    }
}

void doBLOS(short opcode) {
    if  (ps_test(PS_C) | ps_test(PS_Z)) {
        PC = PC + ((opcode & 0377) << 1);
    }
}


// one and a half operand instructions

void doJSR (short opcode) {
    short temp_w = read_word(opcode);
    char temp_r = (opcode & 0700) >> 6;
    push(reg[temp_r]);
    if (temp_r != 7) reg[temp_r] = PC;
    PC = temp_w;
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
                        case COM:   doCOM (opcode); break;
                        case COMB:  doCOMB (opcode); break;
                        case INC:   doINC (opcode); break;
                        case INCB:  doINCB (opcode); break;
                        case DEC:   doDEC (opcode); break;
                        case DECB:  doDECB (opcode); break;
                        case NEG:   doNEG (opcode); break;
                        case NEGB:  doNEGB (opcode); break;
                        case TST:   doTST (opcode); break;
                        case TSTB:  doTSTB (opcode); break;
                        case ASR:   doASR (opcode); break;
                        case ASRB:  doASRB (opcode); break;
                        case ASL:   doASL (opcode); break;
                        case ASLB:  doASLB (opcode); break;
                        case ROR:   doROR (opcode); break;
                        case RORB:  doRORB (opcode); break;
                        case ROL:   doROL (opcode); break;
                        case ROLB:  doROLB (opcode); break;
                        case ADC:   doADC (opcode); break;
                        case SWAB:  doSWAB (opcode); break;
                        default:
                            switch (opcode & BRANCH_INSTRUCTION) {
                                case BR: doBR (opcode); break;
                                case BNE: doBNE (opcode); break;
                                case BEQ: doBEQ (opcode); break;
                                case BGE: doBGE (opcode); break;
                                case BLT: doBLT (opcode); break;
                                case BGT: doBGT (opcode); break;
                                case BLE: doBLE (opcode); break;
                                case BPL: doBPL (opcode); break;
                                case BMI: doBMI (opcode); break;
                                case BHI: doBHI (opcode); break;
                                case BLOS: doBLOS (opcode); break;
                                case BVC: doBVC (opcode); break;
                                case BCC: doBCC (opcode); break;
                                case BCS: doBCS (opcode); break;
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
            break;
    }
    
}




