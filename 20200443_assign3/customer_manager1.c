/*Donggeon Lee, 3, customer_manager1.c*/
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "customer_manager.h"

#define UNIT_ARRAY_SIZE 1024

struct UserInfo {
    char* name;                // name of customer
    char* id;                  // id of customer
    int purchase;              // amount purchased 
};

struct DB {
    struct UserInfo* pArray;   // pointer to the array
    int curArrSize;            // size of array
    int numItems;              // number of items in array
                               // used when array need to be expaneded
};

/*CreateCustomerDB(): allocate memory for a created database
It returns a pointer of DB_T object, but when it fails it return NULL*/
DB_T CreateCustomerDB(void) {
    DB_T d;
    d = (DB_T)calloc(1, sizeof(struct DB));
    if (d == NULL) {
        fprintf(stderr, "We can't allocate a memory for DB_T\n");
        return NULL;
    }
    d->curArrSize = UNIT_ARRAY_SIZE; // start with 1024 elements
    d->numItems = 0;
    d->pArray = (struct UserInfo*)calloc(d->curArrSize, sizeof(struct UserInfo));
    if (d->pArray == NULL) {
        fprintf(stderr, "We can't allocate a memory for array of size %d\n", d->curArrSize);
        free(d);
        return NULL;
    }
    return d;
}

/*DestroyCustomerDB(): free every memory of database-*/
void DestroyCustomerDB(DB_T d) {
    struct UserInfo* p;
    if (d != NULL) { p = d->pArray; free(p); free(d); }
}

/*RegisterCustomer(): It stores new Userinfo in the database
It returns 0 when success.
It returns -1 when fails.
It fails when d,id, name is NULL or purchase is not positive or if same id already exists or same name already exists, it is a failure */
int RegisterCustomer(DB_T d, const char* id, const char* name, const int purchase) {
    int i = 0;
    if (d == NULL || id == NULL || name == NULL || purchase <= 0) { return -1; }
    d->numItems++;
    if ((d->curArrSize - 1) < (d->numItems)) {
        d->curArrSize += UNIT_ARRAY_SIZE;
        d->pArray = (struct UserInfo*)realloc(d->pArray, (d->curArrSize) * sizeof(struct UserInfo));
    }
    struct UserInfo* p = d->pArray;
    while (p[i].id != NULL && p[i].name != NULL) {
        if (strcmp(p[i].name, name) == 0 || strcmp(p[i].id, id) == 0) {
            d->numItems--; return -1;
        }
        i++;
    }
    p[i].id = strdup(id); p[i].name = strdup(name);
    p[i].purchase = purchase;
    return 0;
}

/*UnregisterCustomerByID(): It unregisters Userinfo whose inputted id is id
When it success it returns 0, but it returns -1 if it fails.
If d,id is NULL or there is isn't Userinfo that has ID as id, then it is a failure.*/
int UnregisterCustomerByID(DB_T d, const char* id) {
    if (d == NULL || id == NULL) { return -1; }
    struct UserInfo* p = d->pArray;
    int i;
    for (i = 0; i < d->curArrSize; i++) {
        if ((p[i].id != NULL) && strcmp(p[i].id, id) == 0) {
            free(p[i].id);
            free(p[i].name);
            p[i].purchase = 0;
            d->numItems--;
            return 0;
        }
    }
    return -1;
}

/*UnregisterCustomerByName(): It unregisters Userinfo whose inputted name is name
When it success it returns 0, but it returns -1 if it fails.
If d, name is NULL or there isn't Userinfo that has name as name, it is a failure.*/
int UnregisterCustomerByName(DB_T d, const char* name) {
    if (name == NULL || d == NULL) { return -1; }
    struct UserInfo* p = d->pArray;
    int i;
    for (i = 0; i < d->curArrSize; i++) {
        if (p[i].name != NULL && strcmp(p[i].name, name) == 0) {
            free(p[i].id);
            free(p[i].name);
            p[i].purchase = 0;
            d->numItems--;
            return 0;
        }
    }
    return -1;
}

/*GetPurchaseByID(): It returns purchase of Userinfo whose inputted id is id
When it success it returns purchase, but it returns -1 if it fails.
If d, id is NULL or there isn't Userinfo that has id as id, it is a failure.*/
int GetPurchaseByID(DB_T d, const char* id) {
    if (id == NULL || d == NULL) { return -1; }
    struct UserInfo* p = d->pArray;
    int i;
    for (i = 0; i < d->curArrSize; i++) {
        if (p[i].id != NULL && strcmp(p[i].id, id) == 0) {
            return p[i].purchase;
        }
    }
    return -1;
}

/*GetPurchaseByName(): It returns purchase of Userinfo whose inputted name is name
When it success it returns purchase, but it returns -1 if it fails.
If d, name is NULL or there isn't Userinfo that has name as name, it is a failure.*/
int GetPurchaseByName(DB_T d, const char* name) {
    if (name == NULL || d == NULL) { return -1; }
    struct UserInfo* p = d->pArray;
    int i;
    for (i = 0; i < d->curArrSize; i++) {
        if (p[i].name != NULL && strcmp(p[i].name, name) == 0) {
            return p[i].purchase;
        }
    }
    return -1;
}

/*GetSumCustomerPurchase():
When it Success, it returns the sum of numbers returned from fp by iteration of Userinfo in database.
When d, fp is NULL, it is a failure and returns -1.*/
int GetSumCustomerPurchase(DB_T d, FUNCPTR_T fp) {
    if (fp == NULL || d == NULL) { return -1; }
    struct UserInfo* p = d->pArray;
    int sum = 0;
    for (int i = 0; i < d->curArrSize; i++) {
        if (p[i].name != NULL && p[i].id != NULL) { sum += fp(p[i].id, p[i].name, p[i].purchase); }
    }
    return sum;
}
