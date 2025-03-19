#ifndef __SUBSCRIPTION_H__
#define __SUBSCRIPTION_H__
#include "csv.h"

#define MAX_DOCUMENT 9
#define MAX_CHARS 250
#define MAX_PHONE 15
#define MAX_CP 5
#define MAX_DATE 10
#define MAX_PEOPLE 100
// Ex 1 and 2: MAX_SUBSCRIPTIONS should not be used in the student submitted code
#define MAX_SUBSCRIPTIONS 50

typedef struct _tDate {
    int day;
    int month;
    int year;
} tDate;

typedef struct _tPerson {
    char document[MAX_DOCUMENT + 1];
    char name[MAX_CHARS + 1];
    char surname[MAX_CHARS + 1];
    char phone[MAX_PHONE + 1];;
    char email[MAX_CHARS + 1];;
    char address[MAX_CHARS + 1];;
    char cp[MAX_CP + 1];;
    tDate birthday;
} tPerson;

typedef struct _tPeople {
    tPerson elems[MAX_PEOPLE];
    int count;
} tPeople;


typedef struct _tSubscription {
    int id;
    char document[MAX_DOCUMENT + 1];
    tDate start_date;
    tDate end_date;
    char plan[MAX_CHARS + 1];
    float price;
    int numDevices;
} tSubscription;

// Ex 1 and 2: change this to dynamic memory
typedef struct _tSubscriptions {
    tSubscription elems[MAX_SUBSCRIPTIONS];
    int count;
} tSubscriptions;

//////////////////////////////////
// Available methods
//////////////////////////////////

// Parse a tDate from string information
void date_parse(tDate* date, const char* s_date);

// Copy the data from the source to destination
void date_cpy(tDate* destination, tDate source);

// Copy the data from the source to destination
void person_cpy(tPerson* destination, tPerson source);

// Get person data using a string
void person_get(tPerson data, char* buffer);

// Initialize people data
void people_init(tPeople* data);

// Get person data of position index using a string
void people_get(tPeople data, int index, char* buffer);

// Return the position of a person with provided document. -1 if it does not exist
int people_find(tPeople data, const char* document);

// Print people data
void people_print(tPeople data);

// Print subscriptions data
void subscriptions_print(tSubscriptions data);

/////////////////////////////////////
// Ex 2: Requested methods ....
/////////////////////////////////////

// Parse input from CSVEntry
void person_parse(tPerson* data, tCSVEntry entry);

// Add a new person to people data
void people_add(tPeople* data, tPerson person);

// Parse input from CSVEntry
void subscription_parse(tSubscription* data, tCSVEntry entry);

// Copy the data from the source to destination (individual data)
void subscription_cpy(tSubscription* destination, tSubscription source);

// Get subscription data using a string
void subscription_get(tSubscription data, char* buffer);

// Initialize subscriptions data
void subscriptions_init(tSubscriptions* data);

// Add a new subscription
void subscriptions_add(tSubscriptions* data, tPeople people, tSubscription subscription);

// Remove a subscription
void subscriptions_del(tSubscriptions* data, int id);

// Remove all elements
void subscriptions_free(tSubscriptions* data);

// Get subscription data of position index using a string
void subscriptions_get(tSubscriptions data, int index, char* buffer);

// Returns the position of a subscription looking for id's subscription. -1 if it does not exist
int subscriptions_find(tSubscriptions data, int id);

////////////////////////////////////////////

#endif
