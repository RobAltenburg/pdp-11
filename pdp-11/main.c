//
//  main.c
//  pdp-11
//
//  Created by Robert Altenburg on 9/15/12.
//  Copyright (c) 2012 Robert Altenburg. All rights reserved.
//

#include <stdio.h>
#include "core.h"
#include "dispatch.h"

void process_loop (void)
{
    
    
    while (1)
    {
        
        
        short opcode = get_word(pdp_register[7]);
        
        printf("%o: %o | ", pdp_register[7], opcode);
        for (int i = 0; i<7; i++) {
            printf("%o ",pdp_register[i]);
        }
        printf("\n");
        
        pdp_register[7] = pdp_register[7] + 2;
        
        
        if (dispatch (opcode) == 1) return;  // process until halt
        
    }
    
}



int main(int argc, const char * argv[])
{
    
    // insert code here...
    printf("PDP-11!\n");
    
    //printf("local char = %d bytes.\n", (int) sizeof(char));
    //printf("local short = %d bytes.\n\n", (int) sizeof(short));
    
    pdp_register[7] = 01000;  // Start exeicution at .ORG 1000
    
    
    //load a program into memory
    
    //JMP nxt; 0240, MOV nxt R1, NOP, HALT
    short program[] = {0167, 4, 077, 016700, -4, 010002, 05002, 000};
    for (int i = 0; i < sizeof(program)/2; i++) {
        write_word(program[i], pdp_register[7] + (i * 2));
    }
    
    
    
    process_loop();
    
    //dump_core(01000, 18);
    
    return 0;
}

