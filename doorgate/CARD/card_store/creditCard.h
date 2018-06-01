#ifndef CREDIT_CARD
#define CREDIT_CARD

/*******************************************************************************
List of header files.
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "huffman.h"
#include "dataTypes.h"

/*******************************************************************************
Function prototypes.
*******************************************************************************/
void print_menu(void);
void add_card(card_t *card,int size);
void delete_card(card_t *card,int size);
int find_card_array_number(card_t *card,int size,int ID);
void scan_card(card_t *card,int size,int array_number);
int get_ID();
card_t make_card(char first_name[],char initial,char last_name[],
	long long int cardnumber,int month,int year,int cvc,int ID);
void display_one_card(card_t *card,int size);
void display_all_card(card_t *card,int size);
void print_card_heading(void);
void print_card(card_t card);
int scan_unsigned_int(unsigned int* u);
void find_card(card_t *card,int size);
void clear_input_buffer(void);
int menu_control(void);
int empty_slot(card_t *card,int size);
void card_rearrange(card_t *card,int size);
/*void debugger(card_t *card,int size);*/

#endif /* CREDIT_CARD */
