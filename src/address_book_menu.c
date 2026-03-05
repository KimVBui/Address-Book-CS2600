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
	/*
	* First safety check
	* If the address book pointer is null or no contacts are stored
	* we cannot edit.
	*/
	if(!address_book||address_book->count==0)
	{
		get_option(NONE, "No contacts to edit. Press ENTER...");
		return e_no_match;
	}

	/*Clear screen and show header*/
	menu_header("Edit Contact");

	/*
    * Step 1:
	* Ask the user HOW they want to find the contact.
    * They can search by...
    *   1. Name
    *   2. Phone
    *   3. Email
    */
	printf("Search contact to edit by:\n");
	printf("1. Name\n");
	printf("2. Phone\n");
	printf("3. Email\n");

	int opt = get_option(NUM, "Choose: ");

	char key[128]; //stores search string entered by the user
	int field = -1; //indicates the field to search (0=name,1=phone,2=email)
	/*
    * Based on user choice, determine which field to search
    * and collect the search value.
    */
	if(opt ==1)
	{
		field = 0;
		printf("Enter name: ");
		fgets(key,sizeof(key),stdin);
	}
	else if (opt == 2)
	{
		field = 1;
		printf("Enter phone: ");
		fgets(key, sizeof(key),stdin);
	}
	else if (opt == 3)
	{
		field = 2;
		printf("Enter email: ");
		fgets(key,sizeof(key),stdin);
	}
	else
	{
		get_option(NONE, "Invalid option. Press ENTER...");
		return e_fail;
	}

	/*Remove trailing newline from fgets*/
	key[strcspn(key, "\r\n")] = '\0';

	/*If search key is empty, then abort*/
	if(key[0]=='\0')
	{
		get_option(NONE, "Search value cannot be empty. Press ENTER...");
		return e_fail;
	}

	/*
     * Step 2:
     * Find ALL matching contacts.
     * We store the ORIGINAL indices of matches.
     * We allocate memory equal to maximum possible matches
     * (worst case: every contact matches).
     */
	int *matches = malloc(address_book->count * sizeof(int));
	if(!matches)
	{
		return e_fail;
	}

	int match_count = 0;

	/*Iterate through entire address book*/
	for(int i = 0;i<address_book->count;i++)
	{
		ContactInfo *c = &address_book->list[i];
		int found = 0;
		/*
         * Depending on field selected:
         *   field == 0 -> compare name
         *   field == 1 -> compare phone numbers
         *   field == 2 -> compare email addresses
         */
        if (field == 0)
        {
            if (strcasecmp(c->name[0], key) == 0)
			{
                found = 1;
			}
        }
        else if (field == 1)
        {
            for (int p = 0; p < PHONE_NUMBER_COUNT; p++)
            {
                if (c->phone_numbers[p][0] != '\0' && strcmp(c->phone_numbers[p], key) == 0)
                {
                    found = 1;
                    break;
                }
            }
        }
        else if (field == 2)
        {
            for (int e = 0; e < EMAIL_ID_COUNT; e++)
            {
                if (c->email_addresses[e][0] != '\0' && strcmp(c->email_addresses[e], key) == 0)
                {
                    found = 1;
                    break;
                }
            }
        }

        /* If match found, store original index */
        if (found)
		{
            matches[match_count++] = i;
		}
	}
	/*
	* If no matches found, inform user and stop
	*/
	if(match_count==0)
	{
		free(matches);
		get_option(NONE, "No match found. Press ENTER...");
		return e_no_match;
	}

	/*
     * Step 3:
     * Create a TEMPORARY AddressBook containing ONLY matched contacts.
     * This allows us to reuse list_contacts() for selection.
     */
	AddressBook temp;
	temp.fp = NULL;
	temp.count = match_count;
	temp.list = malloc(match_count * sizeof(ContactInfo));

	if(!temp.list)
	{
		free(matches);
		return e_fail;
	}

	/*
     * Copy matched contacts into temporary structure.
     * Assign local SI numbers 1..match_count
     * so user selection is clean.
     */
	for(int i = 0; i<match_count;i++)
	{
		temp.list[i] =address_book->list[matches[i]];
		temp.list[i].si_no = i + 1;
	}

	/*
	* Step 4:
	* Let the user select which matched contact to edit
	*/
	int local_idx = -1;

	Status r = list_contacts(&temp,"Select Contact to Edit",&local_idx,"Enter option ",e_edit);

	/*
     * If user cancels or selection fails,
     * clean up memory and return.
     */
	if (r != e_success || local_idx < 0 || local_idx >= match_count)
	{
		free(temp.list);
		free(matches);
		return e_back;
	}

	/*
	* convert temporary selection index back to original address_book index.
	*/
	int original_idx = matches[local_idx];

	free(temp.list);
	free(matches);

	/*
     * Step 5:
     * Edit selected contact in ORIGINAL address book.
     */
	ContactInfo *c = &address_book->list[original_idx];

	menu_header("Edit Fields");

	printf("Editing: %s\n\n", c->name[0]);
    printf("1. Edit Name\n");
    printf("2. Edit Phone\n");
    printf("3. Edit Email\n");
    printf("0. Back\n");

	int choice = get_option(NUM, "Choose: ");

	if(choice == 0)
	{
		return e_back;
	}

	/*
	* Edit name
	*/
	if(choice == 1)
	{
		printf("Enter new name: ");
		fgets(c->name[0], NAME_LEN, stdin);
		c->name[0][strcspn(c->name[0], "\r\n")] = '\0';

		if(c->name[0][0] == '\0')
		{
			get_option(NONE, "Name cannot be empty. Press ENTER...");
			return e_fail;
		}
	}
	/*
     * Edit Phone Slot
     */
    else if (choice == 2)
    {
        int slot = get_option(NUM, "Phone slot (1-5): ");

        if (slot < 1 || slot > PHONE_NUMBER_COUNT)
        {
            get_option(NONE, "Invalid slot. Press ENTER...");
            return e_fail;
        }

        while (1)
        {
            printf("Enter new phone: ");
            fgets(c->phone_numbers[slot - 1], NUMBER_LEN, stdin);
            c->phone_numbers[slot - 1][strcspn(c->phone_numbers[slot - 1], "\r\n")] = '\0';

            /* Validate phone (digits only, 7-15 length) */
            int ok = 1;
            int len = (int)strlen(c->phone_numbers[slot - 1]);

            if (len < 7 || len > 15)
			{
                ok = 0;
			}

            for (int k = 0; c->phone_numbers[slot - 1][k]; k++)
                if (!isdigit((unsigned char)c->phone_numbers[slot - 1][k]))
				{
                    ok = 0;
				}

            if (ok)
			{
                break;
			}
            printf("Invalid phone.\n");
        }
    }

    /*
     * Edit Email Slot
     */
    else if (choice == 3)
    {
        int slot = get_option(NUM, "Email slot (1-5): ");

        if (slot < 1 || slot > EMAIL_ID_COUNT)
        {
            get_option(NONE, "Invalid slot. Press ENTER...");
            return e_fail;
        }

        while (1)
        {
            printf("Enter new email: ");
            fgets(c->email_addresses[slot - 1], EMAIL_ID_LEN, stdin);
            c->email_addresses[slot - 1][strcspn(c->email_addresses[slot - 1], "\r\n")] = '\0';

            /* Basic email validation */
            char *at = strchr(c->email_addresses[slot - 1], '@');
            char *dot = at ? strchr(at + 1, '.') : NULL;

            int ok = (at && at != c->email_addresses[slot - 1] &&
                      dot && dot != at + 1 &&
                      dot[1] != '\0');

            if (ok)
			{
                break;
			}

            printf("Invalid email.\n");
        }
    }
    else
    {
        get_option(NONE, "Invalid option. Press ENTER...");
        return e_fail;
    }

    get_option(NONE, "Contact updated. Press ENTER...");
    return e_success;

}

Status delete_contact(AddressBook *address_book)
{
	/* Add the functionality for delete contacts here */
}


