#include "CardLibrary.h"	
#include "debug.h"	

UserCardInfo_T	user_info[5];


void CardLib_Init(void)	
{	
	u8 i;
	
	for(i=0;i<sizeof(user_info);i++)	
	{	
		user_info[i].card_ID[4] = {i,i,i,i};						
		user_info[i].user_ID[4] = {i,i,i,i};		
		user_info[i].user_pw[2] = {i,i};
		user_info[i].user_life[4] = {i,i,i,i};					
		user_info[i].user_auth = i;					
	}
}

int CardLib_Clear()
{
		
}

int CardLib_Check()
{
	
}

u16 CardLib_UserNum_Read()		
{	
	return sizeof(user_info);	
}

UserCardInfo_T *CardLib_UserInfo_Read(void)
{


	return 
}


u16 CardLib_UserNum_Write(u16 )			
{
	
}

int CardLib_UserInfo_Write()	
{
	
	
	
}



































//¿¨ºÅ´æ´¢²âÊÔ	

static int cards_registered=0;






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
void scan_card( card_t *card,int array_number)	
{
	int cardnumber = 0;
	int month = 0;
	int year = 0;	
	int ID = 0;

	/*Scan card's number and make sure its withing range*/
	printf("Enter card number>");
	scanf("%lld",&cardnumber);
	while (cardnumber<0 || cardnumber>9999999999999999)
	{
		printf("Invalid card number. Enter card number>");
		scanf("%lld",&cardnumber);	
	}

	/*Scan card's expiration month and make sure its withing range*/
	printf("Enter expiration month>");
	scanf("%d",&month);
	while (month<1 || month>12)
	{
		printf("Invalid expiration month. Enter expiration month>");
		scanf("%d",&month);
	}

	/*Scan card's expiration year and make sure its withing range*/
	printf("Enter expiration year (2 digit form)>");
	scanf("%d",&year); 
	while (year<10 || year>30)
	{
		printf("Invalid expiration year. Enter expiration year (2 digit form)>");
		scanf("%d",&year); 
	}	

	/*Get a new unique ID for the card*/
	ID = get_ID();	

	/*Create the new card*/
	card[array_number]=make_card(cardnumber,month,year,ID);				
}



/*******************************************************************************
get_ID

This function returns a unique ID number for a newly registered card

inputs:
outputs:
- int ID
*******************************************************************************/
int get_ID(){
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
card_t make_card(long long int cardnumber,int month,int year,int ID)	
{
    card_t card;

    card.cardnumber=cardnumber;
    card.month=month;
    card.year=year;	
    card.ID=ID;
	
    return card;
}









void CardLib_Test(void)
{
	card_t card[MAX_CARDLIST_SIZE];
	int size=MAX_CARDLIST_SIZE;
	u16 menu_input=0;	
	int array_number;
	/*This loop makes sure all ID numbers of cards is set to zero, signifying
	 they are non-existant*/	
	for(array_number=0;array_number<MAX_CARDLIST_SIZE;array_number++)
	{
		card[array_number].ID=0;
	}

	while(menu_input!=8)
	{
		print_menu();	
		scan_unsigned_int(&menu_input);	

		switch(menu_input)
		{
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
void print_menu(void)
{
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


int scan_unsigned_int(u16* u)	
{
	if (scanf("%u", u)!=1 || (getchar())!='\n')
	{	
		clear_input_buffer();	
		return ERROR;
	}	
	return 1;
}

	
