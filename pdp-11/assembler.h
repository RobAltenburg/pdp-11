//
//  assembler.h
//  pdp-11
//
//  Created by Robert Altenburg on 10/1/12.
//  Copyright (c) 2012 Robert Altenburg. All rights reserved.
//

#ifndef pdp_11_assembler_h
#define pdp_11_assembler_h

#include "dispatch.h"

void assemble  (short program[], short length,  const char *filename);
void command_file (short program[], short length, short start, char *filename);

#endif
