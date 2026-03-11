#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../src/address_book.h"
#include "../src/address_book_menu.h"
#include "../src/address_book_fops.h"

#include "unity.h"

AddressBook address_book;

void setUp(void) {
    load_file(&address_book);
}

void tearDown(void) {
    int status = remove(DEFAULT_FILE);

    if (status != 0) {
        fprintf(stderr, "Error trying to delete file");
    }
}

// Checks whether or not the file DEFAULT_FILE actually gets created
void test_check_file_existance(void) {
    FILE *file;
    file = fopen(DEFAULT_FILE, "r");

    TEST_ASSERT_TRUE(file != NULL);

    fclose(file);
}

// Checks whether or not the file DEFAULT_FILE has the csv header information
void test_check_file_header(void) {
    FILE *file;
    file = fopen(DEFAULT_FILE, "r");

    if (file == NULL) {
        TEST_FAIL();
        return;
    }

    char buffer[256];
    
    if (fgets(buffer, 256, file) == NULL) {
        TEST_FAIL();
        return;
    }

    TEST_ASSERT_EQUAL_STRING("Name,Phone,Email\n", buffer);

    fclose(file);
}

void test_add_contact(void) {
    // Create a temperary file to act as stdin

    FILE *orig_stdin = stdin;
    FILE *temp = tmpfile();
    if (!temp) {
        TEST_FAIL();
        return;
    }

    // Act as user input

    // Add dummy profile
    fprintf(temp, "1\n1\na\n2\na\n3\na\n0\n");
        
    // Leave Program
    fprintf(temp, "0\nN\n");

    rewind(temp);

    stdin = temp;

    menu(&address_book);

    stdin = orig_stdin;
    fclose(temp);

    TEST_PASS();
}

void test_search_contact(void) {
    // Create a temperary file to act as stdin

    FILE *orig_stdin = stdin;
    FILE *temp = tmpfile();
    if (!temp) {
        TEST_FAIL();
        return;
    }

    // Act as user input

    // Add dummy profile
    fprintf(temp, "1\n1\na\n2\na\n3\na\n0\n");

    // Search for profile
    fprintf(temp, "2\n1\na\n\n");
        
    // Leave Program
    fprintf(temp, "0\nN\n");

    rewind(temp);

    stdin = temp;

    menu(&address_book);

    stdin = orig_stdin;
    fclose(temp);

    TEST_PASS();
}

void test_edit_contact(void) {
    // Create a temperary file to act as stdin

    FILE *orig_stdin = stdin;
    FILE *temp = tmpfile();
    if (!temp) {
        TEST_FAIL();
        return;
    }

    // Act as user input

    // Add dummy profile
    fprintf(temp, "1\n1\na\n2\na\n3\na\n0\n");

    // Edit Contacts
    fprintf(temp, "3\n1\na\nS\n1\n1\naa\n\n");
        
    // Leave Program
    fprintf(temp, "0\nN\n");

    rewind(temp);

    stdin = temp;

    menu(&address_book);

    stdin = orig_stdin;
    fclose(temp);

    TEST_PASS();
}

void test_delete_contact(void) {
    // Create a temperary file to act as stdin

    FILE *orig_stdin = stdin;
    FILE *temp = tmpfile();
    if (!temp) {
        TEST_FAIL();
        return;
    }

    // Act as user input

    // Add dummy profile
    fprintf(temp, "1\n1\na\n2\na\n3\na\n0\n");

    // Delete Contact
    fprintf(temp, "4\nS\n1\ny\n\n");
        
    // Leave Program
    fprintf(temp, "0\nN\n");

    rewind(temp);

    stdin = temp;

    menu(&address_book);

    stdin = orig_stdin;
    fclose(temp);

    TEST_PASS();
}

void test_list_contact(void) {
    // Create a temperary file to act as stdin

    FILE *orig_stdin = stdin;
    FILE *temp = tmpfile();
    if (!temp) {
        TEST_FAIL();
        return;
    }

    // Act as user input

    // Add dummy profile
    fprintf(temp, "1\n1\na\n2\na\n3\na\n0\n");

    // Search for profile
    fprintf(temp, "5\nB\n");
        
    // Leave Program
    fprintf(temp, "0\nN\n");

    rewind(temp);

    stdin = temp;

    menu(&address_book);

    stdin = orig_stdin;
    fclose(temp);

    TEST_PASS();
}

void test_save_contact(void) {
    // Create a temperary file to act as stdin

    FILE *orig_stdin = stdin;
    FILE *temp = tmpfile();
    if (!temp) {
        TEST_FAIL();
        return;
    }

    // Act as user input

    // Add dummy profile
    fprintf(temp, "1\n1\na\n2\na\n3\na\n0\n");

    // Save Contacts
    fprintf(temp, "6\n");
        
    // Leave Program
    fprintf(temp, "0\nN\n");

    rewind(temp);

    stdin = temp;

    menu(&address_book);

    stdin = orig_stdin;
    fclose(temp);

    TEST_PASS();
}

void test_exit_program(void) {
    
    // Create a temperary file to act as stdin

    FILE *orig_stdin = stdin;
    FILE *temp = tmpfile();
    if (!temp) {
        TEST_FAIL();
        return;
    }

    // Act as user input
    fprintf(temp, "0\n");
    rewind(temp);

    stdin = temp;

    menu(&address_book);

    stdin = orig_stdin;
    fclose(temp);

    TEST_PASS();
}

int main(void) {
    UNITY_BEGIN();

    // File operations
    RUN_TEST(test_check_file_existance);
    RUN_TEST(test_check_file_header);

    // Program requirements

    RUN_TEST(test_add_contact);
    RUN_TEST(test_search_contact);
    RUN_TEST(test_edit_contact);
    RUN_TEST(test_delete_contact);
    RUN_TEST(test_list_contact);

    // Saving & Exiting

    RUN_TEST(test_save_contact);
    RUN_TEST(test_exit_program);

    return UNITY_END();
}