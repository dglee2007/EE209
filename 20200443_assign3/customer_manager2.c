/*Donggeon Lee, 3, customer_manager2.c*/
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "customer_manager.h"

enum {HASH_SIZE = 1024};
enum {HASH_MULTIPLIER = 65599};

struct UserInfo {
  char *name;                   // name of customer
  char *id;                     // id of customer
  int purchase;                 // amount purchased
  struct UserInfo *next_ID;     // pointer which links next item in linked list
  struct UserInfo *next_NAME;   // pointer which links next item in linked list
};

struct DB {
  struct UserInfo **HASH_ID;    // pointer to the HASH Table using id
  struct UserInfo **HASH_NAME;  // pointer to the HASH Table using name
  int HashSizeID;               // Size of HashID 
  int HashSizeNAME;             // Size of HashNAME 
  int numItems;                 // number of items in database
};

/*hash_function() : It returns a hash value when id or name is inputted as parameter pcKey 
                    and size of hash table is inputted as parameter HashSize.*/ 
static int hash_function(const char *pcKey, int HashSize){
  int i; 
  unsigned int uiHash = 0U;
  for (i=0; pcKey[i] !='\0'; i++){
    uiHash = uiHash * (unsigned int)HASH_MULTIPLIER
      + (unsigned int)pcKey[i];
  }
  return (int)(uiHash % (unsigned int)HashSize);
}

/*SearchID() : It searches Userinfo that has id as inputted id
When it success, it returns pointer of satisfied Userinfo
When it fails, it returns NULL*/
struct UserInfo *SearchID(DB_T d, const char *id){
  struct UserInfo *p;
  int h=hash_function(id, d->HashSizeID);
  for (p=d->HASH_ID[h]; p!=NULL; p=p->next_ID){
    if(strcmp(p->id, id)==0){
      return p;
    }
  }
  return NULL;
}

/*SearchNAME() :It searches Userinfo that has name as inputted name
When it success, it returns pointer of satisfied Userinfo
When it fails, it returns NULL*/
struct UserInfo *SearchNAME(DB_T d, const char *name){
  struct UserInfo *p;
  int h=hash_function(name, d->HashSizeNAME);
  for (p=d->HASH_NAME[h]; p!=NULL; p=p->next_NAME){
    if(strcmp(p->name, name)==0){
      return p;
    }
  }
  return NULL;
}

/*CreateCustomerDB(): It allocates memory for a created database
It returns a pointer of DB_T object, but when it fails it return NULL*/
DB_T CreateCustomerDB(void){
  DB_T d;
  d = (DB_T) calloc(1, sizeof(struct DB));
  if (d == NULL) { 
    fprintf(stderr, "Can't allocate a memory for DB_T\n");
    return NULL;
  }
  d->HashSizeID = HASH_SIZE; d->HashSizeNAME = HASH_SIZE; 
  d->numItems = 0;
  d->HASH_ID = (struct UserInfo **)calloc(HASH_SIZE,
					  sizeof(struct UserInfo *));
  d->HASH_NAME = (struct UserInfo **)calloc(HASH_SIZE,
					    sizeof(struct UserInfo *));
  if (d->HASH_ID == NULL || d->HASH_NAME == NULL) {
    fprintf(stderr, "No enough area for Hash Table expansion\n");   
    free(d);
    return NULL;
  }
  return d;
}

/*DestroyCustomerDB(): free every memory of database-*/
void DestroyCustomerDB(DB_T d){
  struct UserInfo *p; 
  struct UserInfo *nextp; 
  int h;
  for (h=0; h<d->HashSizeID; h++){
    for (p = d->HASH_ID[h]; p!=NULL; p = nextp){
      nextp = p->next_ID;
      free(p);
    }
  }
  free(d);
}

/*RegisterCustomer(): It stores new Userinfo in the database
It returns 0 when success.
It returns -1 when fails.
It fails when d,id, name is NULL or purchase is not positive or if same id already exists or same name already exists, it is a failure */
int RegisterCustomer(DB_T d, const char *id,
		 const char *name, const int purchase){
  if(id==NULL||d==NULL||name==NULL||purchase<=0){return -1;}  
  int a= hash_function(id, d->HashSizeID);
  int b= hash_function(name, d->HashSizeNAME);
  struct UserInfo *user = (struct UserInfo *)calloc(1,
					     sizeof(struct UserInfo));
  if (user == NULL) {return -1;}
  user->name = strdup(name);
  user->id = strdup(id);
  if ( user->name == NULL || user->id == NULL){return -1;}
  user->purchase = purchase; user->next_ID=NULL; user->next_NAME=NULL;
  int i,h,x,y;
  struct UserInfo *p; 
  struct UserInfo *nextp;
  if(SearchID(d, id)==NULL&&SearchNAME(d, name)==NULL){
    user->next_ID = d->HASH_ID[a]; d->HASH_ID[a]=user;
    user->next_NAME = d->HASH_NAME[b]; d->HASH_NAME[b]=user;
    d->numItems++;
    if(d->numItems >= 0.75*(d->HashSizeID) && d->HashSizeID<=1048576){
      d->HashSizeID = 2*(d->HashSizeID);
      d->HASH_ID = (struct UserInfo **)realloc(d->HASH_ID,
			     (d->HashSizeID)*sizeof(struct UserInfo *));
      if (d->HASH_ID == NULL) {fprintf(stderr, "No enough area for Hash Table expansion\n");}
      else{
        for (i=0;i<d->HashSizeID;i++){d->HASH_ID[i]=NULL;}
        for (h=0;h<d->HashSizeNAME;h++){
          for (p=d->HASH_NAME[h];p!=NULL;p=nextp){
            nextp=p->next_NAME;
            x=hash_function(p->id,d->HashSizeID);
            p->next_ID=d->HASH_ID[x];
            d->HASH_ID[x]=p;
          }
        } 
      }
      d->HashSizeNAME = 2*(d->HashSizeNAME);
      d->HASH_NAME = (struct UserInfo **)realloc(d->HASH_NAME,
			  (d->HashSizeNAME)*sizeof(struct UserInfo *));
      if (d->HASH_NAME == NULL) { 
        fprintf(stderr, "No enough area for Hash Table expansion\n");
        }
      else{
        for (i=0;i<d->HashSizeNAME;i++){(d->HASH_NAME)[i]=NULL;}
        for (h=0;h<d->HashSizeID;h++){
          for (p=d->HASH_ID[h];p!=NULL;p=nextp){
            nextp=p->next_ID;
            y=hash_function(p->name,d->HashSizeNAME);
            p->next_NAME=d->HASH_NAME[y];
            d->HASH_NAME[y]=p;
          }
        }
        }

    }
    return 0;
  }
  return -1;
}

/*UnregisterCustomerByID(): It unregisters Userinfo whose inputted id is id
When it success it returns 0, but it returns -1 if it fails.
If d,id is NULL or there is isn't Userinfo that has id as id, then it is a failure.*/
int UnregisterCustomerByID(DB_T d, const char *id){
  if(id==NULL||d==NULL){return -1;}
  struct UserInfo *p; struct UserInfo *prevp;
  int h=hash_function(id, d->HashSizeID);
  p=d->HASH_ID[h];
  if(p!=NULL){
    if(strcmp(p->id, id)==0){
      (d->HASH_ID[h])=(p->next_ID);
      UnregisterCustomerByName(d, p->name);
      d->numItems--;
      return 0;
    }
    else{
      while(p!=NULL){
	prevp = p;
        p = p->next_ID;
        if(p==NULL){return -1;}
        if(strcmp(p->id, id)==0){
	  prevp->next_ID=p->next_ID;
	  UnregisterCustomerByName(d, p->name);
	  d->numItems--;
	  return 0;
	}
      }
    }
  }
  return -1;
}

/*UnregisterCustomerByName(): It unregisters Userinfo whose inputted name is name
When it success it returns 0, but it returns -1 if it fails.
If d, name is NULL or there isn't Userinfo that has name as name, it is a failure.*/
int UnregisterCustomerByName(DB_T d, const char *name){
  if(name==NULL||d==NULL){return -1;}
  struct UserInfo *p; struct UserInfo *prevp;
  int h=hash_function(name, d->HashSizeNAME);
  p=d->HASH_NAME[h];
  if(p!=NULL){
    if(strcmp(p->name, name)==0){
      (d->HASH_NAME[h])=(p->next_NAME);
      UnregisterCustomerByID(d, p->id); free(p);
      d->numItems--;
      return 0;
    }
    else{
      while(p!=NULL){
	prevp = p;
        p = p->next_NAME;
        if(p==NULL){return -1;}
        if(strcmp(p->name, name)==0){
	  prevp->next_NAME=p->next_NAME;
	  UnregisterCustomerByID(d, p->id);
	  free(p);
	  d->numItems--;
	  return 0;
	}
      }
    }
  }
  return -1;
}

/*GetPurchaseByID(): It returns purchase of Userinfo whose inputted id is id
When it success it returns purchase, but it returns -1 if it fails.
If d, id is NULL or there isn't Userinfo that has id as id, it is a failure.*/
int GetPurchaseByID(DB_T d, const char* id){
  if(id==NULL||d==NULL){return -1;}
  struct UserInfo *p=SearchID(d, id);
  if(p!=NULL){
	return p->purchase;
  }
  return -1; 
}

/*GetPurchaseByName(): It returns purchase of Userinfo whose inputted name is name
When it success it returns purchase, but it returns -1 if it fails.
If d, name is NULL or there isn't Userinfo that has name as name, it is a failure.*/
int GetPurchaseByName(DB_T d, const char* name){
  if(name==NULL||d==NULL){return -1;}
  struct UserInfo *p=SearchNAME(d, name);
  if(p!=NULL){
	return p->purchase;
  }
  return -1;  
}

/*GetSumCustomerPurchase(): 
When it Success, it returns the sum of numbers returned from fp by iteration of Userinfo in database.
When d, fp is NULL, it is a failure and returns -1.*/
int GetSumCustomerPurchase(DB_T d, FUNCPTR_T fp){
  if(fp==NULL||d==NULL){return -1;}
  struct UserInfo *p; 
  struct UserInfo *nextp; 
  int h; int sum=0;
  for (h=0; h<d->HashSizeID; h++){
    for (p = d->HASH_ID[h]; p!=NULL; p = nextp){
      sum+=fp(p->id, p->name, p->purchase);
      nextp = p->next_ID;
    }
  }
  return sum;
}
