#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "subscription.h"

//////////////////////////////////
// Available methods
//////////////////////////////////

// Parse a tDate from string information
void date_parse(tDate* date, const char* s_date) {
    // Check output data
    assert(date != NULL);

    // Check input date
    assert(s_date != NULL);
    assert(strlen(s_date) == 10);

    // Parse the input date
    sscanf(s_date, "%d/%d/%d", &(date->day), &(date->month), &(date->year));
}

// Copy the data from the source to destination
void date_cpy(tDate* destination, tDate source) {
    destination->day = source.day;
    destination->month = source.month;
    destination->year = source.year;
}

// Copy the data from the source to destination
void person_cpy(tPerson* destination, tPerson source) {
    // Copy identity document data
    strncpy(destination->document, source.document, MAX_DOCUMENT + 1);

    // Copy name data
    strncpy(destination->name, source.name, MAX_CHARS + 1);

    // Copy surname data
    strncpy(destination->surname, source.surname, MAX_CHARS + 1);

    // Copy phone data
    strncpy(destination->phone, source.phone, MAX_PHONE + 1);

    // Copy email data
    strncpy(destination->email, source.email, MAX_CHARS + 1);

    // Copy address data
    strncpy(destination->address, source.address, MAX_CHARS + 1);

    // Copy cp data
    strncpy(destination->cp, source.cp, MAX_CP + 1);

    // Copy the birthday date
    date_cpy(&(destination->birthday), source.birthday);
}

// Get person data using a string
void person_get(tPerson data, char* buffer)
{
    // Print all data at same time
    sprintf(buffer,"%s;%s;%s;%s;%s;%s;%s;%02d/%02d/%04d",
        data.document,
        data.name,
        data.surname,
        data.phone,
        data.email,
        data.address,
        data.cp,
        data.birthday.day, data.birthday.month, data.birthday.year);
}

// Initialize the people data
void people_init(tPeople* data) {
    // Check input/output data
    assert(data != NULL);

    data->count = 0;
}

// Get person data of position index using a string
void people_get(tPeople data, int index, char* buffer)
{
    assert(index >= 0 && index < data.count);
    person_get(data.elems[index], buffer);
}

// Return the position of a person with provided document. -1 if it does not exist
int people_find(tPeople data, const char* document) {
    int i = 0;
    while (i < data.count) {
        if (strcmp(data.elems[i].document, document) == 0) {
            return i;
        }
        i++;
    }

    return -1;
}

// Print people data
void people_print(tPeople data) {
    char buffer[1024];
    int i;
    for (i = 0; i < data.count; i++) {
        people_get(data, i, buffer);
        printf("%s\n", buffer);
    }
}

// Print subscriptions data
void subscriptions_print(tSubscriptions data) {
    char buffer[1024];
    int i;
    for (i = 0; i < data.count; i++) {
        subscriptions_get(data, i, buffer);
        printf("%s\n", buffer);
    }
}

//////////////////////////////////////
// EX2: Implement your methods here....
//////////////////////////////////////

// Parse input from CSVEntry
void person_parse(tPerson* data, tCSVEntry entry) {
    // Check input data
    assert(data != NULL);

    // Check entry fields
    assert(csv_numFields(entry) == 8);

    int pos = 0; // Allow to easy incremental position of the income data

    // Copy identity document data
    assert(strlen(entry.fields[pos]) == MAX_DOCUMENT);
    csv_getAsString(entry, pos, data->document, MAX_DOCUMENT + 1);

    // Copy name data
    csv_getAsString(entry, ++pos, data->name, MAX_CHARS + 1);

    // Copy surname data
    csv_getAsString(entry, ++pos, data->surname, MAX_CHARS + 1);

    // Copy phone data
    csv_getAsString(entry, ++pos, data->phone, MAX_PHONE + 1);

    // Copy email data
    csv_getAsString(entry, ++pos, data->email, MAX_CHARS + 1);

    // Copy address data
    csv_getAsString(entry, ++pos, data->address, MAX_CHARS + 1);

    // Copy cp data
    assert(strlen(entry.fields[++pos]) == MAX_CP);
    csv_getAsString(entry, pos, data->cp, MAX_CP + 1);

    // Parse the birthday date
    assert(strlen(entry.fields[++pos]) == MAX_DATE);
    date_parse(&(data->birthday), entry.fields[pos]);
}

// Add a new person to people data
void people_add(tPeople* data, tPerson person) {
    // Check input data
    assert(data != NULL);

    // If person does not exist add it
    if (people_find(*data, person.document) < 0) {
        // Copy the data to the new position if there is space
        assert(data->count < MAX_PEOPLE);
        person_cpy(&(data->elems[data->count]), person);

        // Increase the number of elements
        data->count++;
    }
}

// Parse input from CSVEntry
void subscription_parse(tSubscription* data, tCSVEntry entry) {
    // Check input data
    assert(data != NULL);

    // Check entry fields
    assert(csv_numFields(entry) == 7);

    int pos = 0; // Allow to easy incremental position of the income data

    // Copy subscription's id data
    data->id = csv_getAsInteger(entry, pos);

    // Copy identity document data
    assert(strlen(entry.fields[++pos]) == MAX_DOCUMENT);
    csv_getAsString(entry, pos, data->document, MAX_DOCUMENT + 1);

    // Parse start date
    assert(strlen(entry.fields[++pos]) == MAX_DATE);
    date_parse(&(data->start_date), entry.fields[pos]);

    // Parse end date
    assert(strlen(entry.fields[++pos]) == MAX_DATE);
    date_parse(&(data->end_date), entry.fields[pos]);

    // Copy plan data
    csv_getAsString(entry, ++pos, data->plan, MAX_CHARS + 1);

    // Copy price data
    data->price = csv_getAsReal(entry, ++pos);

    // Copy number of devices data
    data->numDevices = csv_getAsInteger(entry, ++pos);

    // Check preconditions that needs the readed values
    assert(data->price >= 0);
    assert(data->numDevices >= 1);
}

// Copy the data from the source to destination (individual data)
void subscription_cpy(tSubscription* destination, tSubscription source) {
    // Copy subscription's id data
    destination->id = source.id;

    // Copy identity document data
    strncpy(destination->document, source.document, MAX_DOCUMENT + 1);

    // Copy start date
    date_cpy(&(destination->start_date), source.start_date);

    // Copy end date
    date_cpy(&(destination->end_date), source.end_date);

    // Copy plan data
    strncpy(destination->plan, source.plan, MAX_CHARS + 1);

    // Copy price data
    destination->price = source.price;

    // Copy number of devices data
    destination->numDevices = source.numDevices;
}

// Get subscription data using a string
void subscription_get(tSubscription data, char* buffer)
{
    // Print all data at same time
    sprintf(buffer,"%d;%s;%02d/%02d/%04d;%02d/%02d/%04d;%s;%g;%d",
        data.id,
        data.document,
        data.start_date.day, data.start_date.month, data.start_date.year,
        data.end_date.day, data.end_date.month, data.end_date.year,
        data.plan,
        data.price,
        data.numDevices);
}

// Initialize subscriptions data
void subscriptions_init(tSubscriptions* data) {
    // Check input data
    assert(data != NULL);
    // Ex 2
    data->count = 0;
}

// Add a new subscription
void subscriptions_add(tSubscriptions* data, tPeople people, tSubscription subscription) {
    // Check input data
    assert(data != NULL);

    // If subscription does not exist add it
    if (subscriptions_find(*data, subscription.id) < 0) {
        // Check person exists
        if (people_find(people, subscription.document) >= 0) {
            // Copy the data to the new position
			// Ex 2

            // Increase the number of elements
            data->count++;
        }
    }
}

// Remove a subscription
void subscriptions_del(tSubscriptions* data, int id) {
    // Ex2
}

// Get subscription data of position index using a string
void subscriptions_get(tSubscriptions data, int index, char* buffer)
{
    assert(index >= 0 && index < data.count);
    subscription_get(data.elems[index], buffer);
}

// Returns the position of a subscription looking for id's subscription. -1 if it does not exist
int subscriptions_find(tSubscriptions data, int id) {
    int i = 0;
    while (i < data.count) {
        if (data.elems[i].id == id) {
            return i;
        }
        i++;
    }

    return -1;
}

// Remove all elements 
void subscriptions_free(tSubscriptions* data) { 
    // Ex2
}