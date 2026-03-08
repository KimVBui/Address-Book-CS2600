#include <stdio.h>
//#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "address_book.h"
#include "address_book_menu.h"

//#include "abk_fileops.h"
//#include "abk_log.h"
//#include "abk_menus.h"
//#include "abk.h"

int get_option(int type, const char *msg)
{
	/*
	 * Mutilfuction user intractions like
	 * Just an enter key detection
	 * Read an number
	 * Read a charcter
	 */ 
	/* Fill the code to add above functionality */	
	
}

Status save_prompt(AddressBook *address_book)
{
	char option;

	do
	{
		main_menu();

		option = get_option(CHAR, "\rEnter 'N' to Ignore and 'Y' to Save: ");

		if (option == 'Y')
		{
			save_file(address_book);
			printf("Exiting. Data saved in %s\n", DEFAULT_FILE);

			break;
		}
	} while (option != 'N');

	free(address_book->list);

	return e_success;
}

Status list_contacts(AddressBook *address_book, const char *title, int *index, const char *msg, Modes mode)
{
	/* 
	 * Add code to list all the contacts availabe in address_book.csv file	
	 * Should be menu based
	 * The menu provide navigation option if the entries increase the page size
	 */ 



	return e_success;
}

void menu_header(const char *str)
{
	fflush(stdout);

	system("clear");

	printf("#######  Address Book  #######\n");
	if (str != '\0')
	{
		printf("#######  %s\n", str);
	}
}

void main_menu(void)
{
	menu_header("Features:\n");

	printf("0. Exit\n");
	printf("1. Add Contact\n");
	printf("2. Search Contact\n");
	printf("3. Edit Contact\n");
	printf("4. Delete Contact\n");
	printf("5. List Contacts\n");
	printf("6. Save\n");
	printf("\n");
	printf("Please select an option: ");
}

Status menu(AddressBook *address_book)
{
	ContactInfo backup;
	Status ret;
	int option;

	do
	{
		main_menu();

		option = get_option(NUM, "");

		if ((address_book-> count == 0) && (option != e_add_contact))
		{
			get_option(NONE, "No entries found!!. Would you like to add? Use Add Contacts");

			continue;
		}

		switch (option)
		{
			case e_add_contact:
				/* Add your implementation to call add_contacts function here */
				break;
			case e_search_contact:
				search_contact(address_book);
				break;
			case e_edit_contact:
				edit_contact(address_book);
				break;
			case e_delete_contact:
				delete_contact(address_book);
				break;
			case e_list_contacts:
				break;
				/* Add your implementation to call list_contacts function here */
			case e_save:
				save_file(address_book);
				break;
			case e_exit:
				break;
		}
	} while (option != e_exit);

	return e_success;
}

Status add_contacts(AddressBook *address_book)
{
	/* Add the functionality for adding contacts here */
	if (address_book->count >= 5){
		printf("Sorry, but your address book is full.")
	}
	if(loadFile(address_book) == NULL){
		fprintf
	}

	char op;
	//don't need to make new variables for name, email, etc, they already exist.
	ContactInfo *newContract = &address_book->list[address_book->count];
	//The function up here, creates a pointer variable and addresses the enum function;
	//Retrieves the memory address of address_book and goes to get the list from ContactInfo, and inside the list, it goes to receieve content of count from memory

	//infinite loop until op == 0
	while (op == 0){
		if (op == '1'){
			printf("Enter the name:\n");
			scanf("%s", (newContract->name[address_book->count])); //newContract is a pointer that references the variable in enum, and inside, addresses the location of the empty array spot
		}
		if(op == '2'){
			prinf("Enter the Phone Number:\n");
			scanf("%s", (newContract->phone_numbers[address_book->count]));
		}
		if(op == '3'){
			printf("Enter the Email ID: \n");
			scanf("%s", (newContract->email_addresses[address_book->count]));
		}
		printf("0. Back");
		printf("1. Name: %c", newContract->name[address_book->count]);
		printf("2. Phone No 1: %d", newContract->phone_numbers[address_book->count]);
		printf("3. Email ID 1: %c", newContract->email_addresses[address_book->count]);
		printf("Please select an Option:" );
		op = getChar();
	}
	newContract->si_no = address_book->count + 1;
	address_book->count++;

	if (op == "0"){
		main_menu();
	}
	//Gets all the inputs and puts them in the file

	fprintf = (fptr, "Name: %c; Phone: %d; Email: %c \n", name, number, email);
	
	//here, goes back to the main menn

	printf
	if (op == "0"){
		main_menu();
	}
}

Status search(const char *str, AddressBook *address_book, int loop_count, int field, const char *msg, Modes mode)
{
	/* Add the functionality for adding contacts here */
}

Status search_contact(AddressBook *address_book)
{
	/* Add the functionality for search contacts here */
	char op = getChar();
	printf("0. Back\n");
	printf("1. Name\n");
	printf("2. Phone No.\n");
	printf("3. Email ID\n");
	printf("4. Serial No.\n");
	printf("\n");
	printf("")
	
}

Status edit_contact(AddressBook *address_book)
{
	/* Add the functionality for edit contacts here */
}

Status delete_contact(AddressBook *address_book)
{
	/* Add the functionality for delete contacts here */
}
