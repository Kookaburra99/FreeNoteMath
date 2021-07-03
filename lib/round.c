/********************************************/
/*************** FreeNoteMath ***************/
/*
 * Source: round.c
 * Implementation of rounding, truncating and
 * other functions like ceil or floor
 * 
 *
 * Copyright (C) 2020 Pedro Gamallo Fernández
 *
 * This program is free software; you can 
 * redistribute it and/or modify it under the 
 * terms of the GNU General Public License as 
 * published by the Free Software Foundation
 */
/********************************************/

#include <math.h>

/** Functions names **/
const char* function_names[] = {
    "ceil",     "abs",     "floor",
    "round",    "trunc",    0   /* Ends with 0 */
};

/** Functions pointers **/
double (*function_ptr[])(double) = {
    ceil,       fabs,       floor,
    round,      trunc,      0   /* Ends with 0 */
};