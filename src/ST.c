/********************************************/
/*************** FreeNoteMath ***************/
/*
 * Source: ST.c
 * Implementation of the symbol table defined
 * on the header file ST.h
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sintactic.tab.h"



/** Definition of each node on the BST **/
struct node{
    info_row info;              // Information saved on the node
    struct node *left, *right;  // Pointers to left and right sons
};


/** Declaration of private functions **/
unsigned _is_empty(symbol_table st);
struct node* _node_search(symbol_table *st, char *key);
info_row _delete_reestructure(symbol_table *st);

/** PUBLIC FUNCTIONS: Defined on ST.h **/

symbol_table* st_init(init_commands commands_list[]){
    symbol_table *st = malloc (sizeof(symbol_table));

    int i;
    for(i = 0; commands_list[i].fnct_name != 0; i++){
        st_insert(st, commands_list[i].fnct_name, commands_list[i].lc);
        st_addCommand(st, commands_list[i].fnct_name, commands_list[i].fnct);
    }
    
    return st;
}

int st_search(symbol_table st, char *key){
    // If the current node is empty, the string isn't on it
    if(_is_empty(st))
        return -1;

    // The passed string is compared with that of the current node
    int comp = strcmp(key, st->info.key);

    // If it's the same, returns the lexical component
    if(comp == 0)
        return st->info.lc;

    // If it's less, searchs on the left son
    else if(comp < 0)
        return st_search(st->left, key);

    // If it's greater, searchs on the right son
    else
        return st_search(st->right, key);    
}

int st_insert(symbol_table *st, char *key, int lc){
    // If the current node is empty, the string is inserted on it
    if(_is_empty(*st)){
        *st = malloc(sizeof(struct node));

        info_row new_info;
        // Copy the alphanumeric string inside the info struct
        new_info.key = malloc(sizeof(key));
        strcpy(new_info.key, key);
        new_info.init = 0; // Marks the variable as not initialized
        new_info.lc =lc;
        (*st)->info = new_info;

        (*st)->left = NULL;
        (*st)->right = NULL;

        return 0;
    }

    //Search for the corresponding node (similar to st_search)
    int comp = strcmp(key, (*st)->info.key);
    
    // If it's less, searchs on the left son
    if(comp < 0)
        return st_insert(&(*st)->left, key, lc);
    
    // If it's greater, searchs on the right son
    else if(comp > 0)
        return st_insert(&(*st)->right, key, lc);

    // In other case, the alphanumeric string was already in the table
    return -1;
}

int st_delete(symbol_table *st, char *key){
    symbol_table aux; // Auxiliar symbol tree necessary during deletion

    //Search for the corresponding node (similar to st_search)
    int comp = strcmp(key, (*st)->info.key);
    
    // If it's less, searchs on the left son
    if(comp < 0)
        return st_delete(&(*st)->left, key);
    
    // If it's greater, searchs on the right son
    else if(comp > 0)
        return st_delete(&(*st)->right, key);

    else if(_is_empty((*st)->left) && _is_empty((*st)->right)){
        // If both sons are empty, simply delete the node
        free(*st);
        *st = NULL;

        return 0;
    }

    else if(_is_empty((*st)->left)){
        // If only right node exists, delete the node and
        // replace by the son
        aux = *st;
        *st = (*st)->right;
        free(aux);

        return 0;
    }

    else if(_is_empty((*st)->left)){
        // If only right node exists, delete the node and
        // replace by the son
        aux = *st;
        *st = (*st)->left;
        free(aux);

        return 0;
    }

    else{
        // If both sons exist, delete the node and replace
        // by one of them and reestructure the sucessive sons
        (*st)->info = _delete_reestructure(&(*st)->right);

        return 0;
    }

    return -1;
}

info_row* st_get_info_row(symbol_table *st, char *key){
    // Searches the corresponding node
    struct node* st_node = _node_search(st, key);

    // If the key isn't on the symbol table, returns NULL
    if(st_node == NULL)
        return NULL;
    
    return &(st_node->info);
}

void st_show_lc_items(symbol_table st, int lc){
    if(!_is_empty(st)){
        // In-order search (LNR) to show the strings in alphabetical order
        st_show_lc_items(st->left, lc);

        if(st->info.lc == lc)
            printf("\t%s:\t%.10g\n", st->info.key, st->info.value.var);

        st_show_lc_items(st->right, lc);
    }
}

void st_delete_lc_items(symbol_table *st, int lc){
    symbol_table aux; // Auxiliar symbol_table necessary during deletion

    if(!_is_empty(*st)){
        st_delete_lc_items(&(*st)->left, lc);
        st_delete_lc_items(&(*st)->right, lc);

        // Only deletes the string with the lexical component 'lc'
        if((*st)->info.lc == lc){
            if(_is_empty((*st)->left) && _is_empty((*st)->right)){
                // If both sons are empty, simply delete the node
                free(*st);
                *st = NULL;
            }
            else if(_is_empty((*st)->left)){
                // If only right node exists, delete the node and
                // replace by the son
                aux = *st; // Copy the node to be deleted
                *st = (*st)->right;
                free(aux);
            }
            else if(_is_empty((*st)->right)){
                // If only left node exists, delete the node and
                // replace by the son
                aux = *st; // Copy the node to be deleted
                *st = (*st)->left;
                free(aux);
            }
            else{
                // If both sons exist, delete the node and replace
                // by one of them and reestructure the sucessive sons
                (*st)->info = _delete_reestructure(&(*st)->right);
            }
        }
    }    
}

int st_addValue(symbol_table *st, char *key, double value){
    // Searches the corresponding node
    struct node* st_node = _node_search(st, key);

    // If the key isn't on the symbol table, returns an error
    if(st_node == NULL)
        return -1;

    // Updates the value only if the key is a variable
    if(st_node->info.lc == CONST){
        st_node->info.value.var = value;
        st_node->info.init = 1;
        return 0;
    }
    else
        return -1; // Returns an error in any other case
}

int st_addCommand(symbol_table *st, char *key, void (*command)()){
    // Searches the corresponding node
    struct node* st_node = _node_search(st, key);

    // If the key isn't on the symbol table, returns an error
    if(st_node == NULL)
        return -1;

    st_node->info.value.command = command; 
    return 0; 
}

int st_addFunction(symbol_table *st, char *key, double (*fnc_ptr)()){
    // Searches the corresponding node
    struct node* st_node = _node_search(st, key);

    // If the key isn't on the symbol table, returns an error
    if(st_node == NULL)
        return -1;

    st_node->info.value.fnct_d = fnc_ptr; 
    return 0; 
}

void st_free(symbol_table *st){
    if(!_is_empty(*st)){
        st_free(&((*st)->left)); // Frees the left son
        st_free(&((*st)->right)); // Frees the right son

        free((*st)->info.key);
        free(*st);
        *st = NULL;
    }
}


/** Implementation of private functions **/

unsigned _is_empty(symbol_table st){
    return (st == NULL);
}

struct node* _node_search(symbol_table *st, char *key){
    // If the current node is empty, the key isn't on the symbol table
    if(_is_empty(*st))
        return NULL;

    //Search for the corresponding node (similar to st_search)
    int comp = strcmp(key, (*st)->info.key);

    // If it's the same, return the node
    if(comp == 0){
        return *st;
    }
        
    // If it's less, searchs on the left son
    else if(comp < 0)
        return _node_search(&((*st)->left), key);

    // If it's greater, searchs on the right son
    else if(comp > 0)
        return _node_search(&((*st)->right), key);    
    
    // In any other case, returns NULL
    else
        return NULL;
}

info_row _delete_reestructure(symbol_table *st){
    symbol_table aux; // Auxiliar symbol_table necessary during deletion
    info_row info_node; // info_row to be returned

    if(_is_empty((*st)->left)){
        info_node = (*st)->info;
        aux = *st; // Copy the node to be deleted
        *st = (*st)->right;
        free(aux);
        return info_node;
    }
    else{
        return _delete_reestructure(&(*st)->left);
    }
}
