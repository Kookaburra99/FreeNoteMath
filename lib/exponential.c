/********************************************/
/*************** FreeNoteMath ***************/
/*
 * Source: exponential.c
 * Implementation of the exponential functions,
 * logaritmic functions square root and cube root
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
    "cbrt",     "exp",      "exp2",
    "log",      "log10",    "log2",
    "sqrt",     0   /* Ends with 0 */
};

/** Functions pointers **/
double (*function_ptr[])(double) = {
    cbrt,       exp,        exp2,
    log,        log10,      log2,
    sqrt,       0   /* Ends with 0 */
};

/** Constant names **/
const char* const_names[] = {
    "E",       0   /* Ends with 0 */
};

/** Constant values **/
double const_values[] = {
    M_E,       0   /** Ends with 0 **/
};