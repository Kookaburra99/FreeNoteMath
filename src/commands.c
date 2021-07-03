/********************************************/
/*************** FreeNoteMath ***************/
/*
 * Header: commands.c
 * Implementation of the functions defined
 * on the header file commands.h
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
#include <stdlib.h>
#include <dlfcn.h>
#include "commands.h"
#include "ST.h"
#include "sintactic.tab.h"
#include "lex.yy.h"


/* List of predefined functions/commands */
init_commands list_commands[] = {
    {"help", help, COMMAND},
    {"workspace", workspace, COMMAND},
    {"clear", clear, COMMAND},
    {"echo", change_echo, COMMAND},
    {"load", load, COMMAND_P},
    {"include", include, COMMAND_2P},
    {"print", print, COMMAND_P},
    {"quit", quit, COMMAND},
    {0, 0} // Must end with two 0, following st_init() preconditions
};


/** PUBLIC FUNCTIONS: Defined on commands.h **/
void help(void* param){
    printf("\n************************************************************\n");
    printf("\t\tFreeNoteMath\tv1.0\n");
    printf("A simply numerical computing enviroment that allows simply\n");
    printf("operations (+, -, *, / and ^), and calls to more complex \n");
    printf("computational functions (like trigonometrics)\n");

    printf("\nList of commands:\n");
    printf("\t- clear:\t\tDeletes all the defined variables\n");
    printf("\t- echo:\t\t\tEnables/Disables printing in a variable assignment\n");
    printf("\t- help:\t\t\tShows the current help menu\n");
    printf("\t- include(\"file\"):\tLoads functions and constants from a library\n");
    printf("\t- load(\"file\"):\t\tLoads expresions from an extern file\n");
    printf("\t- print(\"message\"):\tPrints a message to the output\n");
    printf("\t- quit:\t\t\tFinishes the execution of the program\n");
    printf("\t- workspace:\t\tShows all the defined variables\n");
    printf("\n");
}

void workspace(void* param){
    symbol_table *st = (symbol_table *)param;
    printf(ANSI_COLOR_BLUE);
    printf("---------------------------\n");
    printf("Workspace defined variables\n");
    printf("---------------------------\n");
    printf("Variables:\n");
    printf(ANSI_COLOR_RESET);
    st_show_lc_items(*st, VAR); // Displays all variables
    printf(ANSI_COLOR_BLUE);
    printf("Constants:\n");
    printf(ANSI_COLOR_RESET);
    st_show_lc_items(*st, CONST); // Displays all variables
    printf("***************************\n");
}

void clear(void *param){
    symbol_table *st = (symbol_table *)param;
    printf(ANSI_COLOR_BLUE "** Clearing all variables... **" ANSI_COLOR_RESET "\n");
    st_delete_lc_items(st, VAR); // Deletes all variables
}

void change_echo(void* param){
    // Changes de value o global variable 'echo'
    // defined on "sintactic.tab.h"
    if(echo == ECHO_OFF){
        printf(ANSI_COLOR_BLUE "** Enabling echo option for assigments... **" ANSI_COLOR_RESET "\n");
        echo = ECHO_ON;
    }
    else{
        printf(ANSI_COLOR_BLUE "** Disabling echo option for assigments... **" ANSI_COLOR_RESET "\n");
        echo = ECHO_OFF;
    }       
}

void load(void* param){
    char *path = (char *)param;
    // Calls function from 'lex.yy.h'
    int status = change_input_file(path);

    if(status == -1)
        printf(ANSI_COLOR_RED "LOAD ERROR: Maximum nested files exceeded" ANSI_COLOR_RESET "\n");
    else if(status == -2)
        printf(ANSI_COLOR_RED "LOAD ERROR: File not found" ANSI_COLOR_RESET "\n");
}

void include(void* param1, void* param2){
    char *path = (char *)param1;
    symbol_table *st = (symbol_table *)param2;

    void *library = dlopen(path, RTLD_LAZY);
    if(library == NULL){
        printf(ANSI_COLOR_RED "INCLUDE ERROR: Library not found" ANSI_COLOR_RESET "\n");
        return;
    }

    int i;

    // Load the array with the function names
    char **fnc_names = (char **)dlsym(library, "function_names"); 
    // Load the array with the function pointers
    double (**fnc_ptr)(double) = dlsym(library, "function_ptr");

    if(fnc_names != NULL){
        printf(ANSI_COLOR_BLUE "Including functions...\n" ANSI_COLOR_RESET);
        // Insert the functions into the symbol table 
        for(i = 0; fnc_names[i] != 0; i++){
            st_insert(st, fnc_names[i], FNCT);
            st_addFunction(st, fnc_names[i], fnc_ptr[i]);
            printf("\t%s\n", fnc_names[i]);
        }
    }
    
    // Load the array with the function names
    char **const_names = (char **)dlsym(library, "const_names"); 
    // Load the array with the function pointers
    double *const_values = (double *)dlsym(library, "const_values");

    if(const_names != NULL){
        printf(ANSI_COLOR_BLUE "Including constants...\n" ANSI_COLOR_RESET);
        // Insert the constants into the symbol table
        for(i = 0; const_names[i] != 0; i++){
            st_insert(st, const_names[i], CONST);
            st_addValue(st, const_names[i], const_values[i]);
            printf("\t%s\n", const_names[i]);
        }
    }

    printf(ANSI_COLOR_BLUE "** Library included succesfully! **\n" ANSI_COLOR_RESET);
}

void print(void* param){
    char *string = (char *)param;

    printf(ANSI_COLOR_CYAN "%s\n" ANSI_COLOR_RESET, string);
}

void quit(void* param){
    symbol_table *st = (symbol_table *)param;

    st_free(st); //Frees memory reserved by the symbol table

    exit(EXIT_SUCCESS);
}