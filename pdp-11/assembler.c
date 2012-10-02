//
//  assembler.c
//  pdp-11
//
//  Created by Robert Altenburg on 10/1/12.
//  Copyright (c) 2012 Robert Altenburg. All rights reserved.
//

#include <stdio.h>

// todo: figure out the binary format. Till then...
// builds a simh command file
void command_file (short program[], short length, short start, char *filename)
{
    FILE *fp;
    fp = fopen(filename, "w");
    for (int i = 0; i < length/2; i++) {
        fprintf(fp, "d %o %o\n", start + (i * 2), program[i]);
    }
    fprintf(fp, "d PC %o\n",start);
    fprintf(fp, "br %o\ng\n",start);
    fclose(fp);
}