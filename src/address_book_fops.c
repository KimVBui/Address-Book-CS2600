#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <ctype.h>
#include <stdbool.h>

#include "address_book.h"
#include "address_book_fops.h"

Status load_file(AddressBook *address_book)
{
	char line [100];
	address_book->count = 0;
	int tempCount = 0;
	address_book->list = NULL;
	address_book -> fp = NULL;
	
	// Check if the file actually exists

	FILE *fp = fopen(DEFAULT_FILE,"r");
	
	if (fp == NULL && errno == ENOENT){
		// If it doesnt, create it with the csv header information.
		fp = fopen(DEFAULT_FILE,"w");
		if (fp == NULL){
			return e_fail;
		}
		fprintf(fp, "Name,Phone,Email\n");
		fclose(fp);
	} 
	else if (fp == NULL && errno != ENOENT ){
		return e_fail;
	} 
	else {
		fclose(fp);
	}
	
	// Open the file in read mode
	fp = fopen(DEFAULT_FILE,"r");
	if(fp == NULL){
		return e_fail;
	}

	// Read each of the non-blank lines in the file.
	fgets(line,100,fp);
	while (fgets(line,100,fp)){
		int is_blank = 1;
		for (int i = 0; line[i] != '\0'; i++) {
			if (!isspace((unsigned char)line[i])) {
				is_blank = 0;
				break;
			}
		}
		if (!is_blank){
			tempCount++;
		}
	}
	fclose(fp);

	if(tempCount == 0){
		address_book->list = NULL;
		address_book -> count = 0;
		return e_success;
	}
	
	address_book->list = malloc(tempCount * sizeof(ContactInfo));
	if (address_book->list == NULL) {
		return e_fail;
	}
	address_book->count = tempCount;

	/* 
	 * Reopen and parse file contents
	 */
	fp = fopen(DEFAULT_FILE,"r");
	if (fp == NULL){
		return e_fail;
	}
	
	int index = 0;
	fgets(line,100,fp);
	while (fgets(line,100,fp)){
		int is_blank = 1;
		for (int i = 0; line[i] != '\0'; i++) {
			if (!isspace((unsigned char)line[i])) {
				is_blank = 0;
				break;
			}
		}
		if (!is_blank){
			size_t len = strlen(line);
			while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r')) {
				line[len-1] = '\0';
				len--;
			}
			char* name_token = strtok(line, ",");
			char* phone_number_token = strtok(NULL, ",");
			char* email_token = strtok(NULL, ","); 
			
			if(name_token && phone_number_token && email_token){
				strcpy(address_book->list[index].name[0], name_token);
				strcpy(address_book->list[index].phone_numbers[0], phone_number_token);
				strcpy(address_book->list[index].email_addresses[0], email_token);
				index++;
			}
		}
	}
	
	address_book->count = index;
	fclose(fp);
	
	return e_success;
}

Status save_file(AddressBook *address_book)
{
	/*
	 * Write contacts back to file.
	 * Re write the complete file currently
	 */ 
	address_book->fp = fopen(DEFAULT_FILE, "w");

	if (address_book->fp == NULL)
	{
		return e_fail;
	}

	fprintf(address_book->fp,"Name,Phone,Email\n");

	for (int i= 0; i < address_book->count; i++){
		fprintf(address_book->fp,"%s,%s,%s\n",address_book->list[i].name[0],
			address_book->list[i].phone_numbers[0],address_book->list[i].email_addresses[0]);
	}
	/*
	 * Add the logic to save the file
	 * Make sure to do error handling
	 */ 

	fclose(address_book->fp);

	return e_success;
}

