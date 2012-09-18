//
//  core.h
//  pdp-11
//
//  Created by Robert Altenburg on 9/16/12.
//  Copyright (c) 2012 Robert Altenburg. All rights reserved.
//

#ifndef pdp_11_core_h
#define pdp_11_core_h

char core[64000];

// registers
short pdp_register[8];



short get_word(short offset);
void write_word (short word, short offset);


short byte_swap (short word);

short get_operand (char addr);
short get_address (char addr);

void set_word (char dest_addr, short source_value);

void dump_core (short orig, short range);

#endif
