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



short get_word (short offset)
{
    // todo, raise an error if offset isn't aligned
    if (offset % 2 > 0 ) { printf("ERROR: attempt to read unaligned word.\n");}
    
    return  *(short *)(core + offset);
}


void write_word (short word, short offset)
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
            address = pdp_register[register_number];
            ++pdp_register[register_number];
            break;
        case 3: // Rn contains the address of the operand, then increment Rn by 2
            address = *(short *)(core + pdp_register[register_number]);
            pdp_register[register_number] = pdp_register[register_number] + 2;
            break;
        case 4: // Decrement Rn, then Rn contains the address of the operand
            --pdp_register[register_number];
            address = pdp_register[register_number];
            break;
        case 5: // Decrement Rn by 2, then Rn contains the address of the operand
            pdp_register[register_number] = pdp_register[register_number] - 2;
            address = *(short *)(core + pdp_register[register_number]);
            break;
        case 6: // Rn+X is the address of the operand  (Assumes PC was +2 incremented once already)
            address = pdp_register[register_number] + get_word(pdp_register[7]);
            pdp_register[register_number] = pdp_register[register_number] + 2;
            break;
        case 7: // Rn+X is the address of the address (Assumes PC was +2 incremented once already)
            address = *(short *)(core + pdp_register[register_number] + get_word(pdp_register[7]));
            pdp_register[register_number] = pdp_register[register_number] + 2;
            break;
        default:
            printf("ERROR: (get_address) Bad Address Mode %o\n", address_mode);
            break;
    }
    
    //printf("GA: %o, %o, %o\n", register_number, address_mode, address);
    
    return address;
}

short get_operand (char addr)
{
     
    short operand;
    switch ((addr & 070) >> 3) { // switch on address mode
        case 0: // immediate value
            operand = pdp_register[addr & 07];
            break;
        default: // memory reference
            operand = *(short *)(core + get_address(addr));
            break;
    }
    return operand;
}

void set_word (char dest_addr, short source_value)
{
    
    switch ((dest_addr & 070) >> 3) { // switch on address mode
        case 0: // immediate value
            pdp_register[dest_addr & 07] = source_value;
            break;
        default: // memory reference
           *(short *)(core + get_address(dest_addr)) = source_value;
            break;
    }
    
}

void dump_core (short orig, short range)
{
    printf("CORE DUMP:\n");
    for (short i = orig; i <= orig + range; i = i + 2) {
        printf("%o:  %o\n", i, *(short *)(core + i));
    }
}
