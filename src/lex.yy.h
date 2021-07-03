/********************************************/
/*************** FreeNoteMath ***************/
/*
 * Header: lex.yy.h
 * Definitions of functions used by the
 * lexical analyzer
 *
 * Copyright (C) 2020 Pedro Gamallo Fernández
 *
 * This program is free software; you can 
 * redistribute it and/or modify it under the 
 * terms of the GNU General Public License as 
 * published by the Free Software Foundation
 */
/********************************************/

#include "ST.h"

// Variable to control whether the current input depends on one or more files
extern unsigned int input_file;

/**
 * Main function of the lexical analyzer.
 * Starts the parser that scans the input.
 * Assigns the corresponding value to the
 * global variable 'yylval' of the sintactic
 * analyzer and inserts new alphanumeric strings
 * into the symbol table
 * 
 * @param st: Symbol table that contains the alphanumeric strings
 * @return The lexical component of the input read
 */ 
int yylex(symbol_table *st);

/**
 * Function to change the input to a file
 * of the lexical analyzer
 * 
 * @param file: String indicating the path of the file
 * @return 0 if everything went well, -1 if maximum nested files
 * exceed or -2 if the file could not be opened
 */ 
int change_input_file(char *file);
