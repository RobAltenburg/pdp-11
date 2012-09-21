//
//  core.h
//  pdp-11
//
//  Created by Robert Altenburg on 9/16/12.
//  Copyright (c) 2012 Robert Altenburg. All rights reserved.
//

// implementation for a pdp-11/40

#ifndef pdp_11_core_h
#define pdp_11_core_h

char core[64 * 1024];

#define PSW_C   001
#define PSW_V   002
#define PSW_Z   004
#define PSW_N   010
#define PSW_T   020

// registers

//short * reg = (short *) (core + 0170000);

short reg[8];

short processor_status_word;

char core_read_byte(short offset);
short core_read_word(short offset);

void core_write_byte(char byte, short offset);
void core_write_word (short word, short offset);

short get_operand (char addr);
short get_address (char addr);

short read_word (char addr);
char read_byte (char addr);

void write_word (char dest_addr, short source_value);
void write_byte(char dest_addr, char source_value);

void psw_set (short values);
void psw_reset (short values);

short byte_swap (short word);
void dump_core (short orig, short range);

#endif
