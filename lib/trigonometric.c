/********************************************/
/*************** FreeNoteMath ***************/
/*
 * Source: trigonometric.c
 * Implementation of the trigonometric library
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
    "cos",      "sin",      "tan",
    "cosh",     "sinh",     "tanh",
    "acos",     "asin",     "atan",
    "acosh",    "asinh",    "atanh",
    0   /* Ends with 0 */
};

/** Functions pointers **/
double (*function_ptr[])(double) = {
    cos,        sin,        tan,
    cosh,       sinh,       tanh,
    acos,       asin,       atan,
    acosh,      asinh,      atanh,
    0   /* Ends with 0 */
};

/** Constant names **/
const char* const_names[] = {
    "PI",       0   /* Ends with 0 */
};

/** Constant values **/
double const_values[] = {
    M_PI,       0   /** Ends with 0 **/
};