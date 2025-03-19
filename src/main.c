//#define DSLAB // Uncomment DSLAB directive to emulate DSLAB execution of tests
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "csv.h"
#include "subscription.h"

#ifdef DSLAB
#include <stdlib.h>
#include "test_suite.h"
#include "test_data.h"
#endif

int main(int argc, char** argv)
{
    tCSVData test_data;
    int i;
    tPerson person;
    tPeople people;
    tSubscription subscription;
    tSubscription subscription2;
    tSubscriptions subscriptions;
    char buffer[1024];

    const char* test_people = "98765432J;Hendrik;Lorentz;987654321;hendrik.lorentz@example.com;his street, 5;00001;27/08/1954\n" \
        "33365111X;Marie;Curie;+33123456789;marie.curie@example.com;Radium street, 88;54321;07/10/1867\n" \
        "55565432Z;Nils;Gustaf Dalen;555123123;nils.gustaf@example.com;Picadilly street, 123;12345;25/11/1972\n" \
        "98765432J;Jean Baptiste;Perrin;999654654;jean.perrin@example.com;Champs-Elysees, 44;77777;12/01/1983\n" \
        "12345672C;Anne;Huiller;666789789;anne.huiller@example.com;rue percebe, 125;22222;12/01/1969\n" \
        "47051307Z;James;Bond;707070707;bond.jamesbond@example.com;Downing street, 10;07077;12/01/1952";

    const char* test_subscriptions = "1;98765432J;01/01/2025;31/12/2025;Free;0;1\n" \
        "2;33365111X;01/05/2025;30/04/2026;Standard;29.95;3\n" \
        "3;88762222A;01/08/2025;31/07/2026;Premium;69.95;5\n" \
        "4;12345672C;15/06/2025;14/06/2026;Standard;29.95;3\n" \
        "3;88762222A;01/01/2026;31/12/2026;Premium;69.95;5\n" \
        "5;55565432Z;21/03/2025;20/03/2026;Free;0;1\n" \
        "6;11111111R;01/01/2025;31/12/2025;Premium;69.95;5\n" \
        "5;12345672C;01/01/2026;31/12/2026;Premium;69.95;5\n" \
        "7;47051307Z;01/01/2023;31/12/2028;Premium;29.95;3\n";

    const char* test_subscriptions2 = "7;98765432J;11/09/2025;17/04/2026;Premium;75.50;6\n";

    // Read people data
    csv_init(&test_data);
    csv_parse(&test_data, test_people, "PEOPLE");

    // Initialize people data
    people_init(&people);

    // Add people data
    for (i = 0; i < csv_numEntries(test_data); i++) {
        // Fills a person with all information
        person_parse(&person, csv_getEntry(test_data, i)[0]);
        // Add new person to the list (table) of people
        people_add(&people, person);
    }

#ifdef DSLAB
    // Initialize tests variables and Test Suite
    tAppArguments parameters;
    tTestSuite* testSuite = NULL;
    tTestSection* testSection = NULL;
    bool isOk;
    tBuffer* output = NULL;
    const char* default_readme = "../README.txt";

    // Parse input arguments
    if (!parseArguments(&parameters, argc, argv)) {
        printf("ERROR: Invalid input arguments");
        // Wait user to press a key to ensure error is shown
        waitKey(parameters);
        // Exit with error
        exit(EXIT_FAILURE);
    }

    // Initialize the test suite
    testSuite = (tTestSuite*)malloc(sizeof(tTestSuite));
    testSuite_init(testSuite);
    assert(testSuite != NULL);

    // Load the README.txt file
    if (parameters.readme_file == NULL) {
        testSuite_load_learner(testSuite, default_readme);
    }
    else {
        testSuite_load_learner(testSuite, parameters.readme_file);
    }

    // Create the test section for this activity
    testSuite_addSection(testSuite, "PEC1", "Tests for PEC1 exercise 2");
    testSection = testSuite_getSection(testSuite, "PEC1");
    assert(testSection != NULL);

    // Reserve memory and open buffer
    output = (tBuffer*)malloc(sizeof(tBuffer));
    assert(output != NULL);
    open_buffer(output);

    // Test 1
    start_test(testSection, "Test_1", "Parse and add people");
    isOk = people.count == 5;
    if (isOk) {
        for (i = 0; i < people.count; i++) {
            people_get(people, i, buffer);
            bprintf(output, "%s\n", buffer);
        }
        isOk = strcmp(output->data, result_test1) == 0;
    }
    end_test(testSection, "Test_1", isOk);
#endif

    // Print current people
    printf("People:\n");
    people_print(people);

    // Remove people data
    csv_free(&test_data);

    // Read subscription data
    csv_init(&test_data);
    csv_parse(&test_data, test_subscriptions2, "SUBSCRIPTION");

    // Copy a subscription
    subscription_parse(&subscription, csv_getEntry(test_data, 0)[0]);
    subscription_cpy(&subscription2, subscription);

#ifdef DSLAB
    // Test 2
    start_test(testSection, "Test_2", "Parse, get and duplicate a subscription");

    // Check output and pointers to be sure that there are 2 different variables
    subscription_get(subscription2, buffer);
    isOk = &subscription != &subscription2 && strcmp(result_test2, buffer) == 0;
    end_test(testSection, "Test_2", isOk);
#endif

    // Print duplicated subscription
    printf("\nDuplicated subscription:\n");
    subscription_get(subscription2, buffer);
    printf("%s\n", buffer);

    // Remove subscription data
    csv_free(&test_data);

    // Read subscription data
    csv_init(&test_data);
    csv_parse(&test_data, test_subscriptions, "SUBSCRIPTION");

    // Initialize subscriptions data
    subscriptions_init(&subscriptions);

    // Add subscriptions data
    for (i = 0; i < csv_numEntries(test_data); i++) {
        subscription_parse(&subscription, csv_getEntry(test_data, i)[0]);
        subscriptions_add(&subscriptions, people, subscription);
    }

    // Remove subscriptions data
    csv_free(&test_data);

    // Print current subscriptions
    printf("\nSubscriptions before deleting the one with id = 7:\n");
    subscriptions_print(subscriptions);

#ifdef DSLAB
    // Test 3
    start_test(testSection, "Test_3", "Add subscriptions");
    isOk = subscriptions.count == 5;

    // Test output to be sure that added subscriptions are correct
    clear_buffer(output);
    for (i = 0; i < subscriptions.count; i++) {
        subscriptions_get(subscriptions, i, buffer);
        bprintf(output, "%s\n", buffer);
    }
    isOk = strcmp(output->data, result_test3) == 0;
    end_test(testSection, "Test_3", isOk);

    // Test 4
    start_test(testSection, "Test_4", "Find subscriptions");

    // Test find function to be sure it is implemented
    isOk = subscriptions_find(subscriptions, 2) == 1;
    end_test(testSection, "Test_4", isOk);

#endif

	// Remove a subscription
	subscriptions_del(&subscriptions, 7);

    // Print current subscriptions
    printf("\nSubscriptions after deleting the one with id = 7:\n");
    subscriptions_print(subscriptions);

#ifdef DSLAB
    // Test 5
    start_test(testSection, "Test_5", "Delete a subscription");
    isOk = subscriptions.count == 4;

    // Test output to be sure that added subscriptions are correct
    clear_buffer(output);
    for (i = 0; i < subscriptions.count; i++) {
        subscriptions_get(subscriptions, i, buffer);
        bprintf(output, "%s\n", buffer);
    }
    isOk = strcmp(output->data, result_test4) == 0;
    end_test(testSection, "Test_5", isOk);


    // Send results to DSLab using testSuite_print.
    // DSLab will use parameter --hide-results to give better feedback,
    // showing tests results on a tab and standard output on another tab.
    testSuite_print(testSuite, parameters);

    // Free memory
    testSuite_free(testSuite);
    free(testSuite);
    close_buffer(output);
    free(output);
#endif

    subscriptions_free(&subscriptions);

    printf("\nPress key to end...\n");
#ifdef DSLAB
    if (parameters.wait_on_exit)
#endif
    getchar();
    return 0;
}
