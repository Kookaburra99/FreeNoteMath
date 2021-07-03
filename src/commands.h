/********************************************/
/*************** FreeNoteMath ***************/
/*
 * Header: commands.h
 * Definitions of the commands availables and
 * their corresponding functions
 *
 * Copyright (C) 2020 Pedro Gamallo Fernández
 *
 * This program is free software; you can 
 * redistribute it and/or modify it under the 
 * terms of the GNU General Public License as 
 * published by the Free Software Foundation
 */
/********************************************/

#ifndef COMMANDS_H
#define COMMANDS_H



typedef struct {
    char *fnct_name;
    void (*fnct)();
    int lc;
} init_commands;

/* List of predefined functions/commands */
extern init_commands list_commands[];


/**
 * Displays the help menu (information, commands list...)
 * 
 * @param param: NULL or any ohter value can be passed.
 * The function doesn't use the parameter. it's declarated
 * only for with the other functions
 */
void help(void* param);

/**
 * Display all variables defined in current workspace and their values
 * 
 * @param param: Pointer to the symbol table must be passed
 * The datatype must be 'symbol_table*'
 */
void workspace(void* param);

/**
 * Deletes all variables defined in current workspace and their values
 * 
 * @param param: Pointer to the symbol table must be passed
 * The datatype must be 'symbol_table*'
 */ 
void clear(void* param);

/**
 * Enables/Disables screen printing of the value of a variable assignment 
 * 
 * @param param: NULL or any ohter value can be passed.
 * The function doesn't use the parameter. it's declarated
 * only for with the other functions
 */
void change_echo(void* param);

/**
 * Load a file and run its expressions
 * 
 * @param param: String indicating the path of the file
 */ 
void load(void* param);

/**
 * Load the functions of the indicated library
 * 
 * @param param: String indicating the path of the library
 */
void include(void* param1, void* param2); 


/**
 * Prints a message to the output
 * 
 * @param param: String to be printed
 */
void print(void* param); 

/**
 * Finalizes program execution
 * Frees memory of used data structures
 * 
 * @param param: Pointer to the symbol table must be passed
 * The datatype must be 'symbol_table*'
 */ 
void quit(void* param);

#endif /* COMMANDS_H */