//
//  core.c
//  pdp-11
//
//  Created by Robert Altenburg on 9/16/12.
//  Copyright (c) 2012 Robert Altenburg. All rights reserved.
//

#include <stdio.h>

#include "core.h"



//short * reg = (short *) (core + 0170000);

void process_loop (void)
{
    
    
    while (1)
    {
        
        short opcode = core_read_word(reg[7]);
        
        
        printf("%#7o: %#7o | ", reg[7], opcode);
        for (int i = 0; i<6; i++) {
            printf("%#7o ",reg[i]);
        }
        printf("| SP: %o, PSW: %o \n",SP, PSW);
        
        PC = PC + 2; // increment the program counter
        
        // if PSW T is set & opcode != RTT, trace trap from 14 (BPT)
        
        if (dispatch (opcode) == 1) return;  // process until halt
        
    }
    
}

// get address uses the address modes to return an address instead of an operand.
// calculate the effective address given an appropriate address mode
short get_address (char address_ref)
{
    char register_number = address_ref & 07;
    char address_mode = (address_ref & 070) >> 3;
    
    short address;
    
    switch (address_mode) {
        case 0: // Register: operand in Rn
            printf("Error: use of get_address with immediate addressing\n");
            break;
        case 1: // Register Deferred: Rn contains the address of the operand
            address = reg[register_number];
            break;
        case 2: // Rn contains the address of the operand, then increment Rn
            address = reg[register_number];
            ++reg[register_number];
            if (register_number >= 6) ++reg[register_number];  // SP & PC always address words
            break;
        case 3: // Rn contains the address of the address, then increment Rn by 2
            address = core_read_word(reg[register_number]);
            reg[register_number] = reg[register_number] + 2;
            break;
        case 4: // Decrement Rn, then Rn contains the address of the operand
            --reg[register_number];
            if (register_number >= 6) --reg[register_number];  // SP & PC always address words
            address = reg[register_number];
            break;
        case 5: // Decrement Rn by 2, then Rn contains the address of the operand
            reg[register_number] = reg[register_number] - 2;
            address = reg[register_number];
            break;
        case 6: // Rn+X is the address of the operand  (Assumes PC was +2 incremented once already)
            address = reg[register_number] + core_read_word(PC);
            PC = PC + 2;
            break;
        case 7: // Rn+X is the address of the address (Assumes PC was +2 incremented once already)
            address = core_read_word(reg[register_number] + core_read_word(PC));
            PC = PC + 2;
            break;
        default:
            printf("ERROR: (get_address) Bad Address Mode %o\n", address_mode);
            break;
    }
    
    return address;
}

// the following reads and writes work on six-bit address references
short read_word (char address_ref)
{
     
    short word;
    switch ((address_ref & 070) >> 3) { // switch on address mode
        case 0: // immediate value
            word = reg[address_ref & 07];
            break;
        default: // memory reference
            word = core_read_word(get_address(address_ref));
            break;
    }
    return word;
}

char read_byte (char address_ref)
{
    
    char byte;
    switch ((address_ref & 070) >> 3) { // switch on address mode
        case 0: // immediate value
            byte = (char) reg[address_ref & 07];  //todo: make sure this reads the correct byte
            break;
        default: // memory reference
            byte = core_read_byte(get_address(address_ref));
            break;
    }
    return byte;
}
   
void write_word (char dest_address_ref, short source_value)
{
    
    switch ((dest_address_ref & 070) >> 3) { // switch on address mode
        case 0: // immediate value
            reg[dest_address_ref & 07] = source_value;
            break;
        default: // memory reference
            core_write_word (source_value, get_address(dest_address_ref));
            break;
    }
    
}

void write_byte(char dest_address_ref, char source_value)
{
    
    switch ((dest_address_ref & 070) >> 3) { // switch on address mode
        case 0: // immediate value
            reg[dest_address_ref & 07] = (short) source_value;  //todo: make sure this sign-extends correctly
            break;
        default: // memory reference
            core_write_byte (source_value, get_address(dest_address_ref));
            break;
    }
    
}



// access directly to core given an offset
// these functions are broken out to allow for memory managment
char core_read_byte(short offset)
{
    return core[offset];
}

void core_write_byte (char byte, short offset)
{
    core[offset] = byte;
}

short core_read_word(short offset)
{
    if (offset % 2 > 0 ) { printf("ERROR: attempt to read unaligned word.\n");}
    return  *(short *)(core + offset); //read words from byte array
}

void core_write_word (short word, short offset)
{
    if (offset % 2 > 0 ) { printf("ERROR: attempt to write unaligned word.\n");}
    *(short *)(core + offset) = word; // write words to byte array
    
}

// program status word functions

void psw_set (short values)
{
    PSW = PSW | values;
}
void psw_reset (short values)
{
    PSW = PSW  & ( 0177777 - values);
}

char psw_test (short values)
{
    if ((PSW & values) == values) {
        return 1;
    } else {
        return 0;
    }
    
}

void psw_on_word (short word)
{
    word == 0 ? psw_set(PSW_Z) : psw_reset(PSW_Z);
    word < 0 ? psw_set(PSW_N) : psw_reset(PSW_N);
}

void psw_on_byte (short byte)
{
    byte == 0 ? psw_set(PSW_Z) : psw_reset(PSW_Z);
    byte < 0 ? psw_set(PSW_N) : psw_reset(PSW_N);
}



// stack functions

void push (short word) {
    SP = SP - 2;
    core_write_word (word, SP);
}

short pop () {
    SP = SP + 2;
    return core[SP - 2];
}

// helper functions

void dump_core (short orig, short range)
{
    printf("CORE DUMP:\n");
    for (short i = orig; i <= orig + range; i = i + 2) {
        printf("%o:  %o\n", i, *(short *)(core + i));
    }
}

void load_core (short program[], short length, short start)
{
for (int i = 0; i < length/2; i++) {
    core_write_word(program[i], start + (i * 2));
}
}
