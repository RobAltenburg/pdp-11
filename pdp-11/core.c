//
//  core.c
//  pdp-11
//
//  Created by Robert Altenburg on 9/16/12.
//  Copyright (c) 2012 Robert Altenburg. All rights reserved.
//

#include <stdio.h>

#include "core.h"
#include "dispatch.h"

//short byte_swap (short word)
//{
//    
//    return (word << 8) + (word >> 8);
//}


// access directly to core
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
    // todo, raise an error if offset isn't aligned
    if (offset % 2 > 0 ) { printf("ERROR: attempt to read unaligned word.\n");}
    
    return  *(short *)(core + offset);
}

void core_write_word (short word, short offset)
{
    *(short *)(core + offset) = word;
    
}


// get address uses the address modes to return an address instead of an operand.
// should be just like get_operand without the 'core[]', but there is no direct register mode.
short get_address (char addr)
{
    
    char register_number = addr & 07;
    char address_mode = (addr & 070) >> 3;
    
    short address;
    
    switch (address_mode) {
        case 1: // Rn contains the address of the operand
            printf("Error: request register as address.\n");
            break;
        case 2: // Rn contains the address of the operand, then increment Rn
            address = reg[register_number];
            ++reg[register_number];
            break;
        case 3: // Rn contains the address of the operand, then increment Rn by 2
            address = *(short *)(core + reg[register_number]);
            reg[register_number] = reg[register_number] + 2;
            break;
        case 4: // Decrement Rn, then Rn contains the address of the operand
            --reg[register_number];
            address = reg[register_number];
            break;
        case 5: // Decrement Rn by 2, then Rn contains the address of the operand
            reg[register_number] = reg[register_number] - 2;
            address = *(short *)(core + reg[register_number]);
            break;
        case 6: // Rn+X is the address of the operand  (Assumes PC was +2 incremented once already)
            address = reg[register_number] + core_read_word(reg[7]);
            reg[register_number] = reg[register_number] + 2;
            break;
        case 7: // Rn+X is the address of the address (Assumes PC was +2 incremented once already)
            address = *(short *)(core + reg[register_number] + core_read_word(reg[7]));
            reg[register_number] = reg[register_number] + 2;
            break;
        default:
            printf("ERROR: (get_address) Bad Address Mode %o\n", address_mode);
            break;
    }
    
    //printf("GA: %o, %o, %o\n", register_number, address_mode, address);
    
    return address;
}

short read_word (char addr)
{
     
    short word;
    switch ((addr & 070) >> 3) { // switch on address mode
        case 0: // immediate value
            word = reg[addr & 07];
            break;
        default: // memory reference
            word = core_read_word(get_address(addr));
            break;
    }
    return word;
}

char read_byte (char addr)
{
    
    char byte;
    switch ((addr & 070) >> 3) { // switch on address mode
        case 0: // immediate value
            byte = (char) reg[addr & 07];  //todo: make sure this reads the correct byte
            break;
        default: // memory reference
            byte = core_read_byte(get_address(addr));
            break;
    }
    return byte;
}
   
void write_word (char dest_addr, short source_value)
{
    
    switch ((dest_addr & 070) >> 3) { // switch on address mode
        case 0: // immediate value
            reg[dest_addr & 07] = source_value;
            break;
        default: // memory reference
            core_write_word (source_value, get_address(dest_addr));
            break;
    }
    
}

void write_byte(char dest_addr, char source_value)
{
    
    switch ((dest_addr & 070) >> 3) { // switch on address mode
        case 0: // immediate value
            reg[dest_addr & 07] = (short) source_value;  //todo: make sure this sign-extends correctly
            break;
        default: // memory reference
            core_write_byte (source_value, get_address(dest_addr));
            break;
    }
    
}


void psw_set (short values)
{
    processor_status_word = processor_status_word | values;
}
void psw_reset (short values)
{
    processor_status_word = processor_status_word & ( 0177777 - values);
}

char psw_test (short values)
{
    if ((processor_status_word & values) == values) {
        return 1;
    } else {
        return 0;
    }
    
}

// these next two functions set the program status word depending on the value
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


void dump_core (short orig, short range)
{
    printf("CORE DUMP:\n");
    for (short i = orig; i <= orig + range; i = i + 2) {
        printf("%o:  %o\n", i, *(short *)(core + i));
    }
}
