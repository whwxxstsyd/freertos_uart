/*******************************************************************************
48430 Embedded C - Assignment 3

Name:FADHIL ABBAS ZAIN
Student ID:12447746

Name:MATTHEW JOHN ANGELL
Student ID:12685374

Name:DAVID BELCHER
Student ID:11979782

Date of submission:9 June 2017
*******************************************************************************/

#include <creditCard.h>

/*******************************************************************************
List preprocessing directives.
*******************************************************************************/
#define ERROR -1
static int cards_registered=0;

/*******************************************************************************
Structures.
*******************************************************************************/


/*******************************************************************************
Main
*******************************************************************************/
int main(void){
	menu_control();
	return 0;
}

/*******************************************************************************
menu_control

This function allows the user to choose the option they wish to use.

inputs:
- card_t card[MAX_CARDLIST_SIZE]
- int size
- unsigned int menu_input
- int array_number
outputs:
- int
*******************************************************************************/
int menu_control(void){
	card_t card[MAX_CARDLIST_SIZE];
	int size=MAX_CARDLIST_SIZE;
	unsigned int menu_input=0;
	int array_number;
	/*This loop makes sure all ID numbers of cards is set to zero, signifying
	 they are non-existant*/
	for(array_number=0;array_number<MAX_CARDLIST_SIZE;array_number++){
		card[array_number].ID=0;
	}
	
	while(menu_input!=8){
		print_menu();
		scan_unsigned_int(&menu_input);

		switch(menu_input){
			case 1:/*The add card function*/
				printf("\n\n");
				add_card(card,size);
				break;
			case 2:/*The delete card function*/
				printf("\n\n");
				delete_card(card,size);
				break;
			case 3:/*To display one card from all the cards registered*/
				printf("\n\n");
				display_one_card(card,size);
				break;
			case 4:/*To display all cards from all the cards registered*/
				printf("\n\n");
				display_all_card(card,size);
				break;
			case 5:/*To display cards from all the cards registered that match the name searched*/
				printf("\n\n");
				find_card(card,size);
				break;
			case 6:/*To save the cards registered to database(compress and encrypt)*/
				printf("\n\n");
				printf("DFFFirstName %d\n", card[1].first_name[0] );
				save( card, cards_registered );
				break;
			case 7:/*To read the cards registered from database(compressed and encrypted)*/
				cards_registered = load( card );
				printf("\n\n");
				break;
			default:
				printf("Invalid choice.\n");
				break;
		}
	}
	return 0;
}

/*******************************************************************************
print_menu

This function prints the initial menu with all instructions on how to use this
program.

inputs:
- none
outputs:
- none
*******************************************************************************/
void print_menu(void){
	printf("\n\n"
	"1. Add card\n"
	"2. Delete card\n"
	"3. Display a card from the database\n"
	"4. Display all card from the database\n"
	"5. Find a card from the database\n"
	"6. Save the card list to the database\n"
	"7. Read the card list from the database\n"
	"8. exit the program\n"
	"   Enter your choice>");
}

/*******************************************************************************
add_card

This function adds cards to the card list

inputs:
- card_t *card
- int size
outputs:
- none
*******************************************************************************/
void add_card( card_t *card, int size )
{
	if(cards_registered == MAX_CARDLIST_SIZE)
	{
		printf("Card list is full.\n");
		return;
	}

	/*This loop finds the next empty card slot/s from the card array*/
	int i;
	for( i = 0; i < MAX_CARDLIST_SIZE; ++i )
	{
		if (card[i].ID == 0)
		{
			cards_registered++;
			scan_card( card, size, i );
			break;
		}
	}
}

/*******************************************************************************
scan_card

This function allows the user to input a new cards information, generates a 
unique ID and then returns a card of type card_t through the makecard function.

inputs:
- card_t *card
- int size
- int array_number
outputs:
- none
*******************************************************************************/
void scan_card( card_t *card, int size, int array_number )
{
	char first_name[MAX_NAME_SIZE];
	char initial = 0;
	char last_name[MAX_NAME_SIZE];
	long long int cardnumber = 0;
	int month = 0;
	int year = 0;
	int cvc = 0;
	int ID = 0;

	/* Clear Names */
	int i;
	for( i = 0; i < MAX_NAME_SIZE; ++i )
	{
		first_name[i] = 0;
		last_name[i] = 0;
	}

	/*Scan card's first name*/
	printf("Enter given name>");
	scanf("%s",first_name);
	clear_input_buffer();
	for( i = 0; i < MAX_NAME_SIZE; ++i )
	{
		printf("%d", first_name[i]);
	}

	printf("\n");
	/*Scan card's initial name*/
	printf("Enter initial>");
	scanf("%c",&initial);
	clear_input_buffer();

	/*Scan card's last name*/
	printf("Enter surname>");
	scanf("%s",last_name);
	clear_input_buffer();

	/*Scan card's number and make sure its withing range*/
	printf("Enter card number>");
	scanf("%lld",&cardnumber);
	while (cardnumber<0 || cardnumber>9999999999999999){
		printf("Invalid card number. Enter card number>");
		scanf("%lld",&cardnumber);
	}

	/*Scan card's expiration month and make sure its withing range*/
	printf("Enter expiration month>");
	scanf("%d",&month);
	while (month<1 || month>12){
		printf("Invalid expiration month. Enter expiration month>");
		scanf("%d",&month);
	}

	/*Scan card's expiration year and make sure its withing range*/
	printf("Enter expiration year (2 digit form)>");
	scanf("%d",&year); 
	while (year<10 || year>30){
		printf("Invalid expiration year. Enter expiration year (2 digit form)>");
		scanf("%d",&year); 
	}

	/*Scan card's CVC and make sure its withing range*/
	printf("Enter cvc>");
	scanf("%d",&cvc);
	while (cvc<=0 || cvc>999){
		printf("Invalid cvc. Enter cvc>");
		scanf("%d",&cvc);
	}

	/*Get a new unique ID for the card*/
	ID=get_ID();

	/*Create the new card*/
	card[array_number]=make_card(first_name,initial,last_name,cardnumber,month,year,cvc,ID);
	for( i = 0; i < MAX_NAME_SIZE; ++i )
	{
		printf("%d ", first_name[i]);
	}
	printf("\n");
	for( i = 0; i < MAX_NAME_SIZE; ++i )
	{
		printf("%d", card[array_number].first_name[i]);
	}
	printf("\n");
}

/*******************************************************************************
get_ID

This function returns a unique ID number for a newly registered card

inputs:
outputs:
- int ID
*******************************************************************************/
int get_ID( ){
	static int ID;
	return ++ID;
}

/*******************************************************************************
make_card

This function copies the users scanned information into a format defined by the
card structure and returns a card of type card_t.

inputs:
- char first_name[]
- char initial
- char last_name[]
- long long int cardnumber
- int month
- int year
- int cvc
- int ID
- card_t card
outputs:
- card_t card
*******************************************************************************/
card_t make_card(char first_name[],char initial,char last_name[],
	long long int cardnumber,int month,int year,int cvc,int ID){
    card_t card;

	int i;
	for( i = 0; i < MAX_NAME_SIZE; ++i )
	{
		card.first_name[i] = first_name[i];
		card.last_name[i] = last_name[i];
	}
	card.initial = initial;
    card.cardnumber=cardnumber;
    card.month=month;
    card.year=year;
    card.cvc=cvc;
    card.ID=ID;
    return card;
}

/*******************************************************************************
delete_card

This function deletes a card from the program based on the cards ID provided 
by the user.
These conditions allow the program to delete according to three situations;
-the card list is empty, cannot delete card
-the card list is occupied but the card ID doesnt exist, cannot delete card
-the card list is occupied and the card ID exists , can delete card

inputs:
- card_t *card
- int size
outputs:
- none
*******************************************************************************/
void delete_card(card_t *card,int size){
	int ID;
	
	if(cards_registered==0){
		printf("No card registered.\n");
	}
	else{
		printf("Please enter card ID to be deleted>");
		scanf("%d",&ID);
		if(find_card_array_number(card,size,ID)==0){
			printf("Card did not exist.\n");
		}
		/*This condition allows the user to confirm whether that they want 
		to delete the card*/
		else if(find_card_array_number(card,size,ID)>0){
			printf("Enter ID again to confirm deletion>");
			clear_input_buffer();
			int confirmID;
			scanf("%d", &confirmID);
			if(confirmID == ID){
				printf("Card had been deleted.\n");
				card[find_card_array_number(card,size,ID)].ID=0;
			}
			else{
				printf("Card deletion canceled.\n");
			}
		}
	}
	card_rearrange(card,size);
}

/*******************************************************************************
find_card_array_number

This function returns the array number of a card from the card array based on 
its ID

inputs:
- card_t *card
- int size
- int ID
outputs:
- int array_number
*******************************************************************************/
int find_card_array_number(card_t *card,int size,int ID){
	int array_number;
	
	for(array_number=0;array_number<=cards_registered;array_number++){
		if(card[array_number].ID==ID){
			return array_number;
		}
	}
	return 0;
}

/*******************************************************************************
empty_slot

This function returns the number of an empty slot in the card array

inputs:
- card_t *card
- int size
outputs:
- int empty_slot
*******************************************************************************/
int empty_slot(card_t *card,int size){
	int empty_slot=0;
	int array_number;

	for(array_number=0;array_number<=cards_registered;array_number++){
		if(cards_registered>0 && card[array_number].ID==0){
			empty_slot++;
		}
	}
	return empty_slot;
}

/*******************************************************************************
card_rearrange

This function rearranges the card array if it has zero cards or too many empty 
slots caused by deletion.

inputs:
- card_t *card
- int size
- int array_number
- int array_number2
- int cards_registered_real
- int empty_slots
outputs:
- none
*******************************************************************************/
void card_rearrange(card_t *card,int size){
	int array_number;/*array number of empty card slot*/
	int array_number2;/*array number of occupied card slot*/
	int cards_registered_real=0;
	int empty_slots=empty_slot(card,size);

	/*This loop rearranges the card array if it had zero cards or more than five
	 empty slots*/
	if(empty_slots==cards_registered || empty_slots>5){
		for(array_number=0;array_number<=cards_registered;array_number++){
			if(card[array_number].ID==0){
				for(array_number2=0;array_number2<=cards_registered;array_number2++){
					if(card[array_number2].ID>0){
						card[array_number]=card[array_number2];
						card[array_number2].ID=0;
					}
				}
			}
		}
		/*array number of empty card slots*/
		for(array_number=0;array_number<=cards_registered;array_number++){
			if(card[array_number2].ID>0){
				cards_registered_real++;
			}
		}
		/*This equation reassigns the new number of cards registered 
		to the program*/
		cards_registered=cards_registered_real;
	}
}

/*******************************************************************************
display_one_card

This function displays one card from all the cards registered based on card ID 
provided by user.
These conditions allow the program to display according to three situations:
-the card list is empty, cannot display card
-the card list is occupied but the card ID doesn't exist, cannot display card
-the card list is occupied and the card ID exist,can display card

inputs:
- card_t *card
- int size
outputs:
- none
*******************************************************************************/
void display_one_card(card_t *card,int size){
	int ID;

	if(cards_registered==0){
		printf("No card registered.\n");
	}
	else{
		printf("Enter Credit card ID Number>"); 
		scanf("%d",&ID);
		if(find_card_array_number(card,size,ID)==0){
			printf("Card did not exist.\n");
		}
		else if(find_card_array_number(card,size,ID)>0){
			print_card_heading();
			printf("%s", card->first_name);
			print_card(card[find_card_array_number(card,size,ID)]);
		}
	}
}

/*******************************************************************************
display_all_card

This function displays all cards from the cards registered array.
These conditions allow the program to display according to two situations:
-the card list is empty, cannot display card
-the card list is occupied, can display card
inputs:
- card_t *card
- int size
outputs:
- none
*******************************************************************************/
void display_all_card(card_t *card,int size){
	int array_number;

	if(cards_registered==0){
		printf("No card registered.\n");
	}
	else{
		print_card_heading();
		for(array_number=0;array_number<=cards_registered;array_number++){
			if(card[array_number].ID>0){
				print_card(card[array_number]);
			}
		}
	}
}

/*******************************************************************************
find_card

This function displays card/s from all the cards registered based on card name 
provided by the user.
These conditions allow the program to display according to three situations;
-the card list is empty, cannot display card
-the card list is occupied but the card name doesn't exist, cannot display card
-the card list is occupied and the card name exist,can display card/s

inputs:
- card_t *card
- int size
outputs:
- none
*******************************************************************************/
void find_card(card_t *card,int size){
	char name[MAX_NAME_SIZE];
	int array_number,similar=0;

	if(cards_registered==0){
		printf("No card registered.\n");
	}
	else{
		printf("Enter Credit card name to find>"); 
		scanf("%s",name);
		for(array_number=0;array_number<=cards_registered;array_number++){
			if(strstr(card[array_number].first_name,name)==NULL && card[array_number].ID>0){
				similar=0;
			}
			else{
				similar=1;
			}
		}
		if(similar==0){
			printf("Card did not exist.\n");
		}
		else{
			print_card_heading();
			for(array_number=0;array_number<=cards_registered;array_number++){
				if(strstr(card[array_number].last_name,name)!=NULL && card[array_number].ID>0){
					print_card(card[array_number]);
				}
			}
		}
	}
}

/*******************************************************************************
print_card_heading

This function displays the heading of the cards for card printing.

inputs:
- none
outputs:
- none
*******************************************************************************/
void print_card_heading(void){
	printf("First Name Initial Last Name  Card Number      Expiry CVC ID\n");
	printf("---------- ------- ---------- ---------------- ------ --- --\n");
}

/*******************************************************************************
print_card

This function prints out a single card

inputs:
- card_t card
outputs:
- none
*******************************************************************************/
void print_card(card_t card){
	printf("%-10.10s ",card.first_name);
	printf("%c       ",card.initial);
	printf("%-10.10s ",card.last_name);
	printf("%16.16lld ",card.cardnumber);
	printf("%2.2u/",card.month);
	printf("%2.2u  ",card.year);
	printf("%3.3u ",card.cvc);
	printf("%2.2u\n",card.ID);
}

/*******************************************************************************
scan_unsigned_int

This function scans and unsigned menu input

inputs:
- unsigned int* u
outputs:
- 1
- ERROR
*******************************************************************************/
int scan_unsigned_int(unsigned int* u){
	if (scanf("%u", u)!=1 || (getchar())!='\n'){
		clear_input_buffer();
		return ERROR;
	}
	return 1;
}

/*******************************************************************************
clear_input_buffer

This function clears the input buffer

inputs:
- none
outputs:
- none
*******************************************************************************/
void clear_input_buffer(void){
	while (getchar()!='\n');
}

/*******************************************************************************
void debugger

This function is for developer to find bugs in the program by displaying 
vital informations

inputs:
- card_t *card
- int size
- int array_number
- int empty_slots
outputs:
- none
*******************************************************************************/
/*void debugger(card_t *card,int size){
	int array_number;
	int empty_slots;

	printf("_______________________________________________\n");
	printf("id count:%d\n",card[0].ID);
	printf("cards registered:%d\n",cards_registered);
	empty_slots=empty_slot(card,size);
	printf("empty slot:%d\n",empty_slots);

	for(array_number=0;array_number<=cards_registered;array_number++){
		print_card(card[array_number]);
	}
	printf("_______________________________________________\n");
}*/

