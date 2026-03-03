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


//example input "get_option(NONE, "Press ENTER...");"
int get_option(int type, const char *msg)
{
		char buffer[64];

	/*Print message if provided*/
	if(msg && msg[0] != '\0')
	{
		printf("%s", msg);
		fflush(stdout);
	}

	/*Read input*/
	if(fgets(buffer, sizeof(buffer), stdin)== NULL)
	{
		return e_fail;
	}

	/*If user just pressed ENTER*/
	if(buffer[0]=="\n")
	{
		return e_new_line;
	}

	/*Remove newline character*/
	buffer[strcspn(buffer, "\n")] = '\0';

	if(type == NONE)
	{
		/*Just wait for enter*/
		return e_success;
	}
	else if (type == NUM)
	{
		/*Convert to integer*/
		return atoi(buffer);
	}
	else if (type == CHAR)
	{
		/*Return first character*/
		return buffer[0];
	}

	return e_fail;
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

//example input "list_contacts(address_book, "All Contacts", NULL, "Enter option: ", e_list);"
Status list_contacts(AddressBook *address_book, const char *title, int *index, const char *msg, Modes mode)
{
	int start = 0;

	//checks to see if the addressbook pointer is null or if there's nothing stored
	//if so we couldn't save anything so it says no contacts found.
	if(!address_book || address_book->count==0)
	{
		get_option(NONE, "No contacts found. Press Enter...");
		return e_no_match;
	}

	//infinite loop to keep the user within the list menu until they go back
	while (1)
	{
		//clears the screen to print out the title
		menu_header(title);

		//printing out the table of contacts
		printf("SI.No  Name                             Phone(1)           Email(1)\n");
        printf("-----  ------------------------------   ----------------   ------------------------------\n");

		//Determines the ending index for the page
		//WindowSize is how many contacts should be shown per page
		int end = start + WINDOW_SIZE;

		//makes sure we don't go past the total contact count
		if (end > address_book->count)
		{
			end = address_book->count;
		}

		//loops through contacts in the current page
		for (int i = start; i<end; i++)
		{
			ContactInfo *c = &address_book->list[i];

			//prints the first phone and email for clean format
			printf("%-5d  %-30s   %-16s   %-30s\n",
                   c->si_no,
                   c->name[0],
                   c->phone_numbers[0],
                   c->email_addresses[0]);
		}

		//displays page number info
		printf("\nShowing %d to %d of %d\n",
			start +1,
			end,
			address_book->count);

		//if only listing stuff, show navigation options
		if (mode == e_list)
		{
			printf("Options: N-next, P-prev, B-back\n");
		}
		else
		{
			//if using this for edit/search/delete
			//allows selection of a contact
			printf("Options: N-next, P-prev, S-select, B-back\n");
		}

		//gets a single character of user input and converts to uppercase
		int ch = get_option(CHAR, msg ? msg : "Enter option: ");
        ch = toupper(ch);


		//NAVIGATION HANDLNG

		//next page
		if (ch == 'N')
        {
			//only go forward if there are contacts ahead
            if (end < address_book->count)
			{
                start += WINDOW_SIZE;
			}
        }
		//previous page
        else if (ch == 'P')
        {
            start -= WINDOW_SIZE;
			//prevents negative index
            if (start < 0)
			{
                start = 0;
			}
        }
		//back to preious menu
        else if (ch == 'B' || ch == e_new_line)
        {
            return e_back;
        }
		//Selection option (only if not simple listing) (MAY NOT BE NECESSARY, delete if so)
        else if (ch == 'S' && mode != e_list)
        {
			//asks user for serial number
            int sel = get_option(NUM, "Enter SI.No to select: ");

			//validates serial number
            if (sel < 1 || sel > address_book->count)
            {
                get_option(NONE, "Invalid SI.No. Press ENTER...");
                continue;
            }

			//converts serial number to array index
            if (index)
			{
                *index = sel - 1;   /* convert SI.No to array index */
			}
            return e_success; //return success with selected index
        }
		//any invalid input
        else
        {
            get_option(NONE, "Invalid option. Press ENTER...");
        }
	}
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
	printf("0. Back");
	printf("1. Name: ");
	printf("2. Phone No 1: ");
	printf("3. Email ID 1:");
	if (getChar() == '1'){
		char newChar;
		scanf("%c",newChar);
	}
	if(getChar() == '2'){
		int newPho;
		scanf("%d", newPho);
	}
	//unfinished, will finish later -Kimberly

	printf
}

Status search(const char *str, AddressBook *address_book, int loop_count, int field, const char *msg, Modes mode)
{
	/* Add the functionality for adding contacts here */
}

Status search_contact(AddressBook *address_book)
{
	/* Add the functionality for search contacts here */
}

Status edit_contact(AddressBook *address_book)
{
	/* Add the functionality for edit contacts here */
}

Status delete_contact(AddressBook *address_book)
{
	/* Add the functionality for delete contacts here */
}


