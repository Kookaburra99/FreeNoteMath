/********************************************/
/*************** FreeNoteMath ***************/
/*
 * A simply numerical computing enviroment
 * that allows simply operations (+, -, *, / 
 * and ^), and calls to more complex computational
 * functions (like trigonometrics)
 *
 * Copyright (C) 2020 Pedro Gamallo Fernández
 *
 * This program is free software; you can 
 * redistribute it and/or modify it under the 
 * terms of the GNU General Public License as 
 * published by the Free Software Foundation
 */
/********************************************/

#include <stdio.h>
#include "ST.h"
#include "sintactic.tab.h"

int main(int argc, char *argv[]) {

    // Instance and initialize the symbol table
    // with the defined commands (commands.h)
    symbol_table *st = st_init(list_commands);
    
    // Begins interactive program execution
    // Starts the syntactic analyzer (sintactic.tab.h)
    startExecution(st);

    return 0;
}