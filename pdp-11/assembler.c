//
//  assembler.c
//  pdp-11
//
//  Created by Robert Altenburg on 10/1/12.
//  Copyright (c) 2012 Robert Altenburg. All rights reserved.
//

#include <stdio.h>
#include "assembler.h"

// todo: figure out the binary format. Till then...
// builds a simh command file
void core_to_commands (char *filename)
{
    FILE *fp;
    short temp_w;
    fp = fopen(filename, "w");
    for (int i = 0; i < MEMSIZE/ 2; i++)  {
        if (i % 2 == 0) {
        temp_w = core_read_word(i);
        if (temp_w) {fprintf(fp, "d %o %ho\n", i, temp_w);}
        }
    }
    
    for (int i = 0; i <= 5; i++) {
        if (reg[i]) fprintf(fp, "d R%o %ho\n", i, reg[i]);
    }
    fprintf(fp, "d SP %ho\n", SP);
    fprintf(fp, "d PC %ho\n", PC);
    
    fprintf(fp, "br %o\ng\n",reg[7]);
    fclose(fp);
}
