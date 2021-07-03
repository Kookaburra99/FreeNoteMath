/********************************************/
/*************** FreeNoteMath ***************/
/*
 * Source: sintactic.y
 * Sintactic analysis source code in bison
 * format that recognizes the syntactic rules
 * formed by the lexical components passed
 * from the lexical analysis
 *
 * Copyright (C) 2020 Pedro Gamallo Fernández
 *
 * This program is free software; you can 
 * redistribute it and/or modify it under the 
 * terms of the GNU General Public License as 
 * published by the Free Software Foundation
 */
/********************************************/

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>   // pow() and trigonometric functions are available by default
#include "sintactic.tab.h"
#include "ST.h"
#include "lex.yy.h"

// Colors to print the output
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"


// Definition of function called by yyparse when preparing to read a new line
void newline();

// Definition of function called by yyparse to manage syntax errors
void syntax_error(symbol_table *st, char *info);
// Definition of function called by yyparse to manage semantic errors
void semantic_error(symbol_table *st, char *info, char *symbol);

/* Variable that controls if an assignment value is displayed */
int echo = ECHO_OFF; // Disabled by default

/* Variable that controls if an error has ocurred */
int flag_error = 0;
%}

%parse-param {symbol_table *st}
%lex-param {symbol_table *st}


%union{
    double      val;        // Type for numbers
    info_row    *id;        // Type for pointers to symbol table
    char        *str;       // Type for strings
}

%token      <val>   NUM                 // Final symbol: number literal
%token      <id>    VAR CONST FNCT      // Final symbol: alphanumeric string (variable, function)

%nonassoc   <id>    COMMAND COMMAND_P COMMAND_2P   // Final symbol: alphanumeric string (command)

%token      <str>   STRING              // Final symbol: string


%type       <id>    command             // Non-final symbol: command
%type       <val>   assign exp          // Non-final symbol: expression

// Accepted operators and their associativity and precedence
%right  '='
%left   '+' '-'
%left   '*' '/'
%left   '%'                 // Module
%left   NEG                 // Negation
%right  '^'                 // Exponentiation


%%
/*      GRAMATICAL RULES        */

input:        /* empty */
            | input line            { newline(); }
;

line:         '\n'
            | command '\n'
            | assign '\n'           {   if(echo == ECHO_ON && !flag_error) 
                                            printf("\t%.10g\n", $1);                      
                                    }
            | exp '\n'              {   
                                        if(!flag_error)
                                            printf("\t%.10g\n", $1); 
                                    }
            | error '\n'            { syntax_error(st, "Unrecognized expression"); yyerrok; }
;

command:      COMMAND                       { (*($1->value.command))(st); }
            | COMMAND_P '(' STRING ')'      { (*($1->value.command))($3); free($3); }
            | COMMAND_2P '(' STRING ')'      { (*($1->value.command))($3, st); free($3); }
;

assign:       VAR '=' assign        {
                                        if(!flag_error){
                                            $$ = $3; 
                                            $1->value.var = $3; 
                                            $1->init = 1; 
                                        }
                                        else if(!$1->init)
                                            st_delete(st, $1->key);                                               
                                    }
            | VAR '=' exp           { 
                                        if(!flag_error){
                                            $$ = $3; 
                                            $1->value.var = $3; 
                                            $1->init = 1; 
                                        } 
                                        else if(!$1->init)
                                            st_delete(st, $1->key);
                                    }
            | CONST '=' assign      {
                                        semantic_error(st, "Trying to assing a value to constant", $1->key);
                                    }
            | CONST '=' exp         {
                                        semantic_error(st, "Trying to assing a value to constant", $1->key);
                                    }
;

exp:          NUM                   { $$ = $1; }
            | CONST                 { $$ = $1->value.var; }
            | VAR                   {
                                        if($1->init == 1) 
                                            $$ = $1->value.var;
                                        else{
                                            semantic_error(st, "Uninitialized variable", $1->key);
                                            st_delete(st, $1->key);
                                        }
                                            
                                    }
            | FNCT '(' exp ')'      { $$ = (*($1->value.fnct_d))($3); }
            | exp '+' exp           { $$ = $1 + $3; }
            | exp '-' exp		    { $$ = $1 - $3; }
	        | exp '*' exp		    { $$ = $1 * $3; }
	        | exp '/' exp		    { $$ = $1 / $3; }
            | exp '%' exp           { $$ = fmod($1, $3); }
	        | '-' exp %prec NEG	    { $$ = -$2; }
	        | exp '^' exp		    { $$ = pow($1, $3); }
	        | '(' exp ')'		    { $$ = $2; }
;

%%

// Function that starts de sintactic and lexical analyzer
void startExecution(symbol_table *st){
    printf("**************************************************\n");
    printf("\t\tFreeNoteMath\tv 1.0\n");
    printf("--------------------------------------------------\n");
    printf("A simply numerical computing enviroment that allows\n");
    printf("simply operations (+, -, *, / * and ^), and calls to\n");
    printf("more complex computational functions (like trigonometrics)\n");
    printf("\n");
    printf("Copyright (C) 2020 Pedro Gamallo Fernández\n\n");
    printf("This program is free software; you can redistribute it\n");
    printf("and/or modify it under the terms of the GNU General Public\n");
    printf("License as published by the Free Software Foundation\n");
    printf("\n");
    printf("Type 'help' to see the list of available commands\n");
    printf("**************************************************\n");
    printf("$> ");

    yyparse(st);
}

// Function called by yyparse when preparing to read a new line
void newline(){
    if(!input_file)
        printf("$> "); // Displays the prompt only if not reading a file
    flag_error = 0;
}

// Function called by yyparse to manage errors
void yyerror(symbol_table *st, char *s){
    if(strcmp(s, "syntax error") == 0){
        return;
    }
    printf(ANSI_COLOR_RED "%s" ANSI_COLOR_RESET "\n", s);
    flag_error = 1;
}

// Function called by yyparse to manage semantic errors
void syntax_error(symbol_table *st, char *info){
    flag_error = 1;
    char *message = malloc(strlen("SYNTAX ERROR: ") + strlen(info) + 1);
    strcpy(message, "SYNTAX ERROR: ");
    strcat(message, info);
    yyerror(st, message);
    free(message);
}

// Function called by yyparse to manage semantic errors
void semantic_error(symbol_table *st, char *info, char *symbol){
    flag_error = 1;
    char *message = malloc(strlen("SEMANTIC ERROR: ") + strlen(info) + 1 + strlen(symbol) + 1);
    strcpy(message, "SEMANTIC ERROR: ");
    strcat(message, info);
    strcat(message, " ");
    strcat(message, symbol);
    yyerror(st, message);
    free(message);
}