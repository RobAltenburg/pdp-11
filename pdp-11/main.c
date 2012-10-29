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
#include "assembler.h"


int main(int argc, const char * argv[])
{
    
    // insert code here...
    printf("PDP-11/40!\n");
    
    interrupt_flag = 0;
    priority_level = 7; // todo: implement the priority system
    
    reg[7] = 01000;  // Start execution at .ORG 1000
    
    //load the program into memory
    
    short program[] = {MOV +02706, 01700, CLR, MOV +02703, 01234, MOV +02700, 03, ASH + 0300, HALT};
    //short subroutine[] = {INC +02, MOV +02700, 077, RTS +01};
    
    load_core(program, sizeof(program), 01000);
    core_to_commands("/Users/rca/test.simh");
    
    //dump_core(01000, 016);
    
    process_loop();
    
    
    return 0;
}

