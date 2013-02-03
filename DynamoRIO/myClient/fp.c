/* **********************************************************
 * Copyright (c) 2008 VMware, Inc.  All rights reserved.
 * **********************************************************/

/*
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * * Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * 
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * 
 * * Neither the name of VMware, Inc. nor the names of its contributors may be
 *   used to endorse or promote products derived from this software without
 *   specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL VMWARE, INC. OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 */

#include "dr_api.h"

#include "../ext/include/drsyms.h"
#include "../ext/include/hashtable.h"
#include "../ext/include/drvector.h"

#include <stdlib.h>
#include <stdio.h>

# define MAX_SYM_RESULT 256

#ifdef WINDOWS
# define DISPLAY_STRING(msg) dr_messagebox(msg)
#else
# define DISPLAY_STRING(msg) dr_printf("%s\n", msg);
#endif

#define NULL_TERMINATE(buf) buf[(sizeof(buf)/sizeof(buf[0])) - 1] = '\0'

static dr_emit_flags_t bb_event(void *drcontext, void *tag, instrlist_t *bb,
                                bool for_trace, bool translating);
static void exit_event(void);

file_t logF;
file_t logOut;
static int fp_count = 0;
static void *count_mutex; /* for multithread support */
static client_id_t client_id;

#define HASH_BITS 8

static hashtable_t fpTable;
static drvector_t addrTable;

void table_init();
void printTable();


/////////////////////////////////hash with char key

#define INITIAL_SIZE 10
#define MAX_CHAIN_LENGTH (8)

static int addr_arr[10];
static int size_arr;

#define MAP_MISSING -3  /* No such element */
#define MAP_FULL -2 	/* Hashmap is full */
#define MAP_OMEM -1 	/* Out of Memory */
#define MAP_OK 0 	/* OK */
typedef void *any_t;
typedef int (*PFany)(any_t, any_t);
typedef any_t map_t;

// We need to keep keys and values
typedef struct _hashmap_element{
	int key;
	int in_use;
	any_t data;
} hashmap_element;

// A hashmap has some maximum size and current size,
// as well as the data to hold.
typedef struct _hashmap_map{
	int table_size;
	int size;
	hashmap_element *data;
} hashmap_map;



static unsigned long crc32_tab[] = {
      0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL, 0x076dc419L,
      0x706af48fL, 0xe963a535L, 0x9e6495a3L, 0x0edb8832L, 0x79dcb8a4L,
      0xe0d5e91eL, 0x97d2d988L, 0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L,
      0x90bf1d91L, 0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL,
      0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L, 0x136c9856L,
      0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL, 0x14015c4fL, 0x63066cd9L,
      0xfa0f3d63L, 0x8d080df5L, 0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L,
      0xa2677172L, 0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
      0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L, 0x32d86ce3L,
      0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L, 0x26d930acL, 0x51de003aL,
      0xc8d75180L, 0xbfd06116L, 0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L,
      0xb8bda50fL, 0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L,
      0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL, 0x76dc4190L,
      0x01db7106L, 0x98d220bcL, 0xefd5102aL, 0x71b18589L, 0x06b6b51fL,
      0x9fbfe4a5L, 0xe8b8d433L, 0x7807c9a2L, 0x0f00f934L, 0x9609a88eL,
      0xe10e9818L, 0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
      0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL, 0x6c0695edL,
      0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L, 0x65b0d9c6L, 0x12b7e950L,
      0x8bbeb8eaL, 0xfcb9887cL, 0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L,
      0xfbd44c65L, 0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L,
      0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL, 0x4369e96aL,
      0x346ed9fcL, 0xad678846L, 0xda60b8d0L, 0x44042d73L, 0x33031de5L,
      0xaa0a4c5fL, 0xdd0d7cc9L, 0x5005713cL, 0x270241aaL, 0xbe0b1010L,
      0xc90c2086L, 0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
      0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L, 0x59b33d17L,
      0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL, 0xedb88320L, 0x9abfb3b6L,
      0x03b6e20cL, 0x74b1d29aL, 0xead54739L, 0x9dd277afL, 0x04db2615L,
      0x73dc1683L, 0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
      0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L, 0xf00f9344L,
      0x8708a3d2L, 0x1e01f268L, 0x6906c2feL, 0xf762575dL, 0x806567cbL,
      0x196c3671L, 0x6e6b06e7L, 0xfed41b76L, 0x89d32be0L, 0x10da7a5aL,
      0x67dd4accL, 0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,
      0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L, 0xd1bb67f1L,
      0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL, 0xd80d2bdaL, 0xaf0a1b4cL,
      0x36034af6L, 0x41047a60L, 0xdf60efc3L, 0xa867df55L, 0x316e8eefL,
      0x4669be79L, 0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L,
      0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL, 0xc5ba3bbeL,
      0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L, 0xc2d7ffa7L, 0xb5d0cf31L,
      0x2cd99e8bL, 0x5bdeae1dL, 0x9b64c2b0L, 0xec63f226L, 0x756aa39cL,
      0x026d930aL, 0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
      0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L, 0x92d28e9bL,
      0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L, 0x86d3d2d4L, 0xf1d4e242L,
      0x68ddb3f8L, 0x1fda836eL, 0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L,
      0x18b74777L, 0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL,
      0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L, 0xa00ae278L,
      0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L, 0xa7672661L, 0xd06016f7L,
      0x4969474dL, 0x3e6e77dbL, 0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L,
      0x37d83bf0L, 0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
      0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L, 0xbad03605L,
      0xcdd70693L, 0x54de5729L, 0x23d967bfL, 0xb3667a2eL, 0xc4614ab8L,
      0x5d681b02L, 0x2a6f2b94L, 0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL,
      0x2d02ef8dL
   };

/* Return a 32-bit CRC of the contents of the buffer. */

unsigned long crc32(const unsigned char *s, unsigned int len)
{
  unsigned int i;
  unsigned long crc32val;
  
  crc32val = 0;
  for (i = 0; i < len; i ++)
    {
      crc32val =
crc32_tab[(crc32val ^ s[i]) & 0xff] ^
(crc32val >> 8);
    }
  return crc32val;
}

/*
* Hashing function for a string
*/
unsigned int hashmap_hash_int_str(hashmap_map * m, char* keystring){

    unsigned long key = crc32((unsigned char*)(keystring), strlen(keystring));

/* Robert Jenkins' 32 bit Mix Function */
key += (key << 12);
key ^= (key >> 22);
key += (key << 4);
key ^= (key >> 9);
key += (key << 10);
key ^= (key >> 2);
key += (key << 7);
key ^= (key >> 12);

/* Knuth's Multiplicative Method */
key = (key >> 3) * 2654435761;

return key % m->table_size;
}

/*
* Return the integer of the location in data
* to store the point to the item, or MAP_FULL.
*/
int hashmap_hash_str(map_t in, char* key){
int curr;
int i;

/* Cast the hashmap */
hashmap_map* m = (hashmap_map *) in;

/* If full, return immediately */
if(m->size >= (m->table_size/2)) return MAP_FULL;

/* Find the best index */
curr = hashmap_hash_int_str(m, key);

/* Linear probing */
for(i = 0; i< MAX_CHAIN_LENGTH; i++){
if(m->data[curr].in_use == 0)
return curr;

if(m->data[curr].in_use == 1 && (strcmp(m->data[curr].key,key)==0))
return curr;

curr = (curr + 1) % m->table_size;
}

return MAP_FULL;
}

/*
* Doubles the size of the hashmap, and rehashes all the elements
*/
int hashmap_rehash_str(map_t in){
int i;
int old_size;
hashmap_element* curr;

/* Setup the new elements */
hashmap_map *m = (hashmap_map *) in;
hashmap_element* temp = (hashmap_element *)
calloc(2 * m->table_size, sizeof(hashmap_element));
if(!temp) return MAP_OMEM;

/* Update the array */
curr = m->data;
m->data = temp;

/* Update the size */
old_size = m->table_size;
m->table_size = 2 * m->table_size;
m->size = 0;

/* Rehash the elements */
for(i = 0; i < old_size; i++){
        int status;

        if (curr[i].in_use == 0)
            continue;
            
status = hashmap_put(m, curr[i].key, curr[i].data);
if (status != MAP_OK)
return status;
}

free(curr);

return MAP_OK;
}

/*
* Add a pointer to the hashmap with some key
*/
int hashmap_put_str(map_t in, char* key, any_t value){
int index;
hashmap_map* m;

/* Cast the hashmap */
m = (hashmap_map *) in;

/* Find a place to put our value */
index = hashmap_hash_str(in, key);
while(index == MAP_FULL){
if (hashmap_rehash(in) == MAP_OMEM) {
return MAP_OMEM;
}
index = hashmap_hash_str(in, key);
}

/* Set the data */
m->data[index].data = value;
m->data[index].key = key;
m->data[index].in_use = 1;
m->size++;

return MAP_OK;
}

int hashmap_get_str(map_t in, char* key, any_t *arg){
int curr;
int i;
hashmap_map* m;
m = (hashmap_map *) in;
curr = hashmap_hash_int_str(m, key);
for(i = 0; i<MAX_CHAIN_LENGTH; i++){

        int in_use = m->data[curr].in_use;
        if (in_use == 1){
            if (strcmp(m->data[curr].key,key)==0){
                *arg = (m->data[curr].data);
                return MAP_OK;
            }
}

curr = (curr + 1) % m->table_size;
}

*arg = NULL;
return MAP_MISSING;
}




//////////////////////////////////HASHTABLE

unsigned int hashmap_hash_int(hashmap_map * m, unsigned int key){
/* Robert Jenkins' 32 bit Mix Function */
key += (key << 12);
key ^= (key >> 22);
key += (key << 4);
key ^= (key >> 9);
key += (key << 10);
key ^= (key >> 2);
key += (key << 7);
key ^= (key >> 12);

/* Knuth's Multiplicative Method */
key = (key >> 3) * 2654435761;

return key % m->table_size;
}




/*
 * Return an empty hashmap, or NULL on failure.
 */
map_t hashmap_new() {
	hashmap_map* m = (hashmap_map*) malloc(sizeof(hashmap_map));
	if(!m) goto err;

	m->data = (hashmap_element*) calloc(INITIAL_SIZE, sizeof(hashmap_element));
	if(!m->data) goto err;
	m->table_size = INITIAL_SIZE;
	m->size = 0;

	return m;
	err:
		if (m)
			hashmap_free(m);
		return NULL;
}

int hashmap_hash(map_t in, int key){
	int curr;
	int i;

	/* Cast the hashmap */
	hashmap_map* m = (hashmap_map *) in;

	/* If full, return immediately */
	if(m->size == m->table_size) return MAP_FULL;

	/* Find the best index */
	curr = hashmap_hash_int(m, key);

	/* Linear probling */
	for(i = 0; i< m->table_size; i++){
		if(m->data[curr].in_use == 0)
			return curr;

		if(m->data[curr].key == key && m->data[curr].in_use == 1)
			return curr;

		curr = (curr + 1) % m->table_size;
	}

	return MAP_FULL;
}

/*
 * Doubles the size of the hashmap, and rehashes all the elements
 */
int hashmap_rehash(map_t in){
	int i;
	int old_size;
	hashmap_element* curr;

	/* Setup the new elements */
	hashmap_map *m = (hashmap_map *) in;
	hashmap_element* temp = (hashmap_element *)
		calloc(2 * m->table_size, sizeof(hashmap_element));
	if(!temp) return MAP_OMEM;

	/* Update the array */
	curr = m->data;
	m->data = temp;

	/* Update the size */
	old_size = m->table_size;
	m->table_size = 2 * m->table_size;
	m->size = 0;

	/* Rehash the elements */
	for(i = 0; i < old_size; i++){
		int status = hashmap_put(m, curr[i].key, curr[i].data);
		if (status != MAP_OK)
			return status;
	}

	free(curr);

	return MAP_OK;
}

/*
 * Add a pointer to the hashmap with some key
 */
int hashmap_put(map_t in, int key, any_t value){
	int index;
	hashmap_map* m;

	/* Cast the hashmap */
	m = (hashmap_map *) in;



	/* Find a place to put our value */
	index = hashmap_hash(in, key);
	while(index == MAP_FULL){
		if (hashmap_rehash(in) == MAP_OMEM) {
			return MAP_OMEM;
		}
		index = hashmap_hash(in, key);
	}

	/* Set the data */
	m->data[index].data = value;
	m->data[index].key = key;
	m->data[index].in_use = 1;
	m->size++; 


	return MAP_OK;
}

/*
 * Get your pointer out of the hashmap with a key
 */
int hashmap_get(map_t in, int key, any_t *arg){
	int curr;
	int i;
	hashmap_map* m;

	/* Cast the hashmap */
	m = (hashmap_map *) in;

	/* Find data location */
	curr = hashmap_hash_int(m, key);

	/* Linear probing, if necessary */
	for(i = 0; i< m->table_size; i++){

		if(m->data[curr].key == key && m->data[curr].in_use == 1){
			*arg = (int *) (m->data[curr].data);
			return MAP_OK;
		}

		curr = (curr + 1) % m->table_size;
	}

	*arg = NULL;

	/* Not found */
	return MAP_MISSING;
}


/*
 * Iterate the function parameter over each element in the hashmap.  The
 * additional any_t argument is passed to the function as its first
 * argument and the hashmap element is the second.
 */
int hashmap_iterate(map_t in, PFany f, any_t item) {
	int i;

	/* Cast the hashmap */
	hashmap_map* m = (hashmap_map*) in;

	/* On empty hashmap, return immediately */
	if (hashmap_length(m) <= 0)
		return MAP_MISSING;	

	/* Linear probing */
	for(i = 0; i< m->table_size; i++)
		if(m->data[i].in_use != 0) {
			any_t data = (any_t) (m->data[i].data);
			int status = f(item, data);
			if (status != MAP_OK) {
				return status;
			}
		}

        return MAP_OK;
}

/*
 * Remove an element with that key from the map
 */
int hashmap_remove(map_t in, int key){
	int i;
	int curr;
	hashmap_map* m;

	/* Cast the hashmap */
	m = (hashmap_map *) in;

	/* Find key */
	curr = hashmap_hash_int(m, key);

	/* Linear probing, if necessary */
	for(i = 0; i< m->table_size; i++){
		if(m->data[curr].key == key && m->data[curr].in_use == 1){
			/* Blank out the fields */
			m->data[curr].in_use = 0;
			m->data[curr].data = NULL;
			m->data[curr].key = 0;

			/* Reduce the size */
			m->size--;
			return MAP_OK;
		}
		curr = (curr + 1) % m->table_size;
	}


	/* Data not found */
	return MAP_MISSING;
}

/* Deallocate the hashmap */
void hashmap_free(map_t in){
	hashmap_map* m = (hashmap_map*) in;
	free(m->data);
	free(m);
}

/* Return the length of the hashmap */
int hashmap_length(map_t in){
	hashmap_map* m = (hashmap_map *) in;
	if(m != NULL) return m->size;
	else return 0;
}


#define KEY_MAX_LENGTH (256)
#define KEY_COUNT 10 
//(1024*1024)

typedef struct data_struct_s
{
    int key;
    int number;
} data_struct_t;

typedef struct function_data
{
    char func_name[KEY_MAX_LENGTH];
    int size;
    int addrs[10];
} function_data;


static map_t funcmap;
static map_t mymap;

void htinit(){
	mymap = hashmap_new();
	funcmap = hashmap_new();
}

int func(int n, data_struct_t * value){
	printf("hhhh %p\n", value);
printf("IN FUNC: Key is "PIFX" and Value is %d\n",value->key, value->number );
return 0;
}


int hashmap_it() {
	int i;
	/* Cast the hashmap */
	hashmap_map* m = (hashmap_map*) mymap;

	/* On empty hashmap, return immediately */
	if (hashmap_length(m) <= 0)
		return MAP_MISSING;	

	/* Linear probing */
	for(i = 0; i< m->table_size; i++)
		if(m->data[i].in_use != 0) {
			
			data_struct_t* data = (m->data[i].data);

printf("IN FUNC: Key is "PIFX" \n",data->key );
			int status = func(3, (data_struct_t* )data);
	
		if (status != MAP_OK) {
				return status;
			}
		}

        return MAP_OK;
}

void printht(){

//hashmap_it();
printf("HashMap size is %d\n", ((hashmap_map*)mymap)->size);
int i;
data_struct_t * value;
/*char str[KEY_MAX_LENGTH];
function_data* fvalue; 
snprintf(str, KEY_MAX_LENGTH, "%s", "Add1");
int  error = hashmap_get_str(funcmap, str, (void**)(&fvalue));
if(error!=MAP_OK) printf("error\n");
if(error == MAP_MISSING) printf("didn't insert \n");
dr_printf(logOut, "fn=%s\n", "main");
/*
for(i = 0; i < fvalue->size; i++){

//printf("Array %d\n", addr_arr[])
error = hashmap_get(mymap, fvalue->addrs[i], (void**)(&value));
printf("Key is "PIFX" and Value is %d\n",value->key, value->number );

dr_printf(logOut, ""PIFX" %d ", value->key, value->number);
}
*/

for(i = 0; i < size_arr; i++){
	int  error = hashmap_get(mymap, addr_arr[i], (void**)(&value));
	printf("Key is "PIFX" and Value is %d\n",value->key, value->number );
}

}

//////////////HASHTABLE end








static void writeLog(void* drcontext);

DR_EXPORT void
dr_init(client_id_t id)
{
size_arr = 0;

    dr_register_exit_event(exit_event);
    dr_register_bb_event(bb_event);
    dr_register_thread_init_event(writeLog); 
    count_mutex = dr_mutex_create();
    client_id = id;
#ifdef SHOW_SYMBOLS
    if (drsym_init(0) != DRSYM_SUCCESS) {
        dr_log(NULL, LOG_ALL, 1, "WARNING: unable to initialize symbol translation\n");
    }
#endif

//table_init();
htinit();

}

static void
exit_event(void)
{
#ifdef SHOW_RESULTS
    char msg[512];
    int len;
    len = dr_snprintf(msg, sizeof(msg)/sizeof(msg[0]),
                      "Instrumentation results:\n"
                      "Processed %d  instructions\n"
                      ,fp_count);
    DR_ASSERT(len > 0);
    NULL_TERMINATE(msg);
    DISPLAY_STRING(msg);
#endif /* SHOW_RESULTS */

    dr_mutex_destroy(count_mutex);

#ifdef SHOW_SYMBOLS
    if (drsym_exit() != DRSYM_SUCCESS) {
        dr_log(NULL, LOG_ALL, 1, "WARNING: error cleaning up symbol library\n");
    }
#endif
printht();

//printTable();

}

void
table_init(){
  hashtable_init(&fpTable, HASH_BITS, HASH_INTPTR, false);
  int key = 4195601;
  int value = 1;
  hashtable_add(&fpTable, &key, value);
  int key1 = 4195623;
  int value1 = 23;
  hashtable_add(&fpTable, &key1, value1);

  int key2 = 4195657;
  int value2 = 57;
  hashtable_add(&fpTable, &key2, value2);



  drvector_init(&addrTable, 10, true, NULL);

}

void
printTable(){
	int i, v, k;
	for(i = 0; i < 10; i++){
		k = (int*)drvector_get_entry(&addrTable, i);
		v = (int* )hashtable_lookup(&fpTable, &k);
		printf("Table values are: %d %d\n", k, v);
	}

  int key1 = 4195623;
  int d = (int*) hashtable_lookup(&fpTable, &key1);
  printf("RESULT&&&&&&&&&&& %d\n", d);

  int key2 = 4195657;
  d = (int*) hashtable_lookup(&fpTable, &key2);
  printf("RESULT&&&&&&&&&&& %d\n", d);
}

void
writeLog(void* drcontext){
	char logname[MAXIMUM_PATH];
	char *dirsep;
    	int len;
	len = dr_snprintf(logname, sizeof(logname)/sizeof(logname[0]),
                      "%s", dr_get_client_path(client_id));
	DR_ASSERT(len > 0);
	for (dirsep = logname + len; *dirsep != '/'; dirsep--)
        DR_ASSERT(dirsep > logname);
    	len = dr_snprintf(dirsep + 1,
                      (sizeof(logname) - (dirsep - logname))/sizeof(logname[0]),
                      "floatingpoint.%d.log", dr_get_thread_id(drcontext));
    	DR_ASSERT(len > 0);
    	NULL_TERMINATE(logname);
    	logF = dr_open_file(logname, 
                             DR_FILE_WRITE_OVERWRITE | DR_FILE_ALLOW_LARGE);
    	DR_ASSERT(logF != INVALID_FILE);
    	dr_log(drcontext, LOG_ALL, 1, 
           "floating point: log for thread %d is fp.%03d\n",
           dr_get_thread_id(drcontext), dr_get_thread_id(drcontext));
	#ifdef SHOW_RESULTS
    	if (dr_is_notify_on()) {
//        	dr_fprintf(STDERR, "<floating point instruction operands for thread %d in %s>\n",
//                  dr_get_thread_id(drcontext), logname);
    	}
	#endif	

writeCallgrind(drcontext);
}

void
writeCallgrind(void* drcontext){
	char logname[MAXIMUM_PATH];
	char *dirsep;
    	int len;
	len = dr_snprintf(logname, sizeof(logname)/sizeof(logname[0]),
                      "%s", dr_get_client_path(client_id));
	DR_ASSERT(len > 0);
	for (dirsep = logname + len; *dirsep != '/'; dirsep--)
        DR_ASSERT(dirsep > logname);
    	len = dr_snprintf(dirsep + 1,
                      (sizeof(logname) - (dirsep - logname))/sizeof(logname[0]),
                      "callgrind.%d.out", dr_get_thread_id(drcontext));
    	DR_ASSERT(len > 0);
    	NULL_TERMINATE(logname);
    	logOut = dr_open_file(logname, 
                             DR_FILE_WRITE_OVERWRITE | DR_FILE_ALLOW_LARGE);
    	DR_ASSERT(logOut != INVALID_FILE);
    	dr_log(drcontext, LOG_ALL, 1, 
           "floating point: log for thread %d is fp.%03d\n",
           dr_get_thread_id(drcontext), dr_get_thread_id(drcontext));
	#ifdef SHOW_RESULTS
    	if (dr_is_notify_on()) {
//        	dr_fprintf(STDERR, "<floating point instruction operands for thread %d in %s>\n",
//                  dr_get_thread_id(drcontext), logname);
    	}
	#endif	

       	dr_fprintf(logOut, "events: Instructions Flops\npositions: instr line\n");

}


static void
print_address(app_pc addr)
{

    const char* prefix = "PRINT ADDRESS: ";
    drsym_error_t symres;
    drsym_info_t *sym;
    char sbuf[sizeof(*sym) + MAX_SYM_RESULT];
    module_data_t *data;
    data = dr_lookup_module(addr);
    if (data == NULL) {
        dr_fprintf(logF, "%s data is null "PFX" \n", prefix, addr);
        return;
    }
    sym = (drsym_info_t *) sbuf;
    sym->struct_size = sizeof(*sym);
    sym->name_size = MAX_SYM_RESULT;   

    symres = drsym_lookup_address(data->full_path, addr - data->start, sym,
                           DRSYM_DEFAULT_FLAGS);


    if (symres == DRSYM_SUCCESS || symres == DRSYM_ERROR_LINE_NOT_AVAILABLE) {
        const char *modname = dr_module_preferred_name(data);
        if (modname == NULL)
            modname = "<noname>";
        dr_fprintf(logF, "%s "PFX" %s, function name is: %s, "PIFX", line off "PFX" \n", prefix, addr,
                   modname, sym->name, addr - data->start - sym->start_offs, sym->line_offs);
        data_struct_t* value;
	hashmap_get(mymap, addr, (void**)(&value));
 
/*
	int error;
	function_data* fvalue;
	fvalue = malloc(sizeof(function_data));
	error = hashmap_get_str(funcmap, sym->name, (void**)(&fvalue));
	if(error == MAP_MISSING){
		free(fvalue);
		fvalue = malloc(sizeof(function_data));
//		addr_arr[size_arr] = addr;
		fvalue->size = -1;
	}
	fvalue->size++;
	fvalue->addrs[fvalue->size] = addr;
	snprintf(fvalue->func_name, KEY_MAX_LENGTH, "%s", sym->name);
        error = hashmap_put_str(funcmap, sym->name, fvalue);
        if(error!=MAP_OK){printf("Error\n");}
	printf("has inserted '%s'\n", fvalue->func_name);
	function_data* dd;
        error = hashmap_get_str(funcmap, sym->name,(void**)&dd);
	printf("has got  '%s' %d\n", dd->func_name, dd->size);
*/
        dr_fprintf(logOut, "fl=%s\nfn=%s\n",sym->file, sym->name );
        dr_fprintf(logOut, ""PIFX" %d %d\n",addr,sym->line,value->number);
        if (symres == DRSYM_ERROR_LINE_NOT_AVAILABLE) {
            dr_fprintf(logF, "%s Line is not available\n", prefix);
        } else {
            dr_fprintf(logF, "Line number is  %s:%"UINT64_FORMAT_CODE" %d\n",
                       sym->file, sym->line, sym->line_offs);
        }
    } else
        dr_fprintf(logF, "%s some error "PFX" \n", prefix, addr);
  
    dr_free_module_data(data);
}


bool is_SIMD_arithm(int opcode){
	return (opcode == OP_addss || opcode == OP_addsd || opcode == OP_mulss || opcode == OP_mulsd ||
	    opcode == OP_subss || opcode == OP_subsd || opcode == OP_divss || opcode == OP_divsd ||	
	    opcode == OP_sqrtss || opcode == OP_sqrtsd || opcode == OP_rsqrtss);	

}

bool is_SIMD_packed(int opcode){
	return (opcode == OP_addps || opcode == OP_addpd || opcode == OP_mulps || opcode == OP_mulpd ||
	    opcode == OP_subps || opcode == OP_subpd || opcode == OP_divps || opcode == OP_divpd ||	
	    opcode == OP_sqrtps || opcode == OP_sqrtpd || opcode == OP_rsqrtps);	
}


bool is_single_precision_instr(int opcode){
	return (opcode == OP_addss || opcode == OP_mulss || opcode == OP_subss || 
		opcode == OP_divss || opcode == OP_sqrtss  || opcode == OP_rsqrtss);	
}


static void 
getRegReg(reg_id_t r1, reg_id_t r2, int opcode, app_pc addr){
	
	const char * r1Name = get_register_name(r1);
	const char * r2Name = get_register_name(r2);
	int s1        = atoi(r1Name + 3 * sizeof(char));
	int s2        = atoi(r2Name + 3 * sizeof(char));

	int error;
	data_struct_t* value;
	value = malloc(sizeof(data_struct_t));
	error = hashmap_get(mymap, addr, (void**)(&value));
	if(error == MAP_MISSING){
		free(value);
		value = malloc(sizeof(data_struct_t));
		addr_arr[size_arr] = addr;
		size_arr++;
		value->number = 0;
	}
	value->key = addr;        
	value->number++;
        error = hashmap_put(mymap, addr, value);
        if(error!=MAP_OK){printf("Error\n");}

/*
	int key = (int)addr;
	printf("Address is "PIFX" %d\n", addr, key);	
        int value = (int*)hashtable_lookup(&fpTable,&key);
	printf("value is %d\n");
	if(hashtable_lookup(&fpTable,&key) == NULL ){
		printf("HERE!!!!!!!!!!!!!!!\n");
		value = 1;
        	hashtable_add(&fpTable, &key, value);
		drvector_append(&addrTable, &key);
	}
	else{
		printf("else case\n");
		value++;
        	hashtable_add_replace(&fpTable, &key, value);
	}
	
	int v1 = (int*) hashtable_lookup(&fpTable,&key);
	printf("value after %d\n", v1);
*/

	dr_mcontext_t mcontext;
   	memset(&mcontext, 0, sizeof(dr_mcontext_t));
   	mcontext.flags = DR_MC_MULTIMEDIA;
   	mcontext.size = sizeof(dr_mcontext_t);
   	bool result = dr_get_mcontext(dr_get_current_drcontext(), &mcontext);
	int r, s;
	float op1, op2;
	if(is_single_precision_instr(opcode)){
//		for(r=0; r<16; ++r)
//			for(s=0; s<4; ++s)
//		     		printf("reg %i.%i: %f\n", r, s, 
//					*((float*) &mcontext.ymm[r].u32[s]));
		op1 = *((float*) &mcontext.ymm[s1].u32[0]);
		op2 = *((float*) &mcontext.ymm[s2].u32[0]);
	}
	else{
//		for(r=0; r<16; ++r)
//    			for(s=0; s<2; ++s)
//	     			printf("reg %i.%i: %f\n", r, s, 
//					*((double*) &mcontext.ymm[r].u64[s]));
		op1 = *((double*) &mcontext.ymm[s1].u64[0]);
		op2 = *((double*) &mcontext.ymm[s2].u64[0]);
	}
       	dr_fprintf(logF, "%d: %f  %f\n",opcode, op1, op2);
	print_address(addr);
}

static void
callback(reg_id_t reg, int displacement, reg_id_t destReg, int opcode, app_pc addr){
	int r, s;
   	const char * destRegName = get_register_name(destReg);
   	int regId = atoi(destRegName + 3 * sizeof(char));
	
	int error;
	data_struct_t* value;
	value = malloc(sizeof(data_struct_t));
	error = hashmap_get(mymap, addr, (void**)(&value));
	if(error == MAP_MISSING){
		free(value);
		value = malloc(sizeof(data_struct_t));
		addr_arr[size_arr] = addr;
		size_arr++;
		value->number = 0;
	}
	value->key = addr;        
	value->number++;
        error = hashmap_put(mymap, value->key, value);
        if(error!=MAP_OK){printf("Error\n");}

/*
        int value = (int*)hashtable_lookup(&fpTable,&addr);
	if(value == NULL){
		value = 1;
        	hashtable_add(&fpTable, &addr, value);
		drvector_append(&addrTable, &addr);
	}
	else{
		value++;
        	hashtable_add_replace(&fpTable, &addr, value);
	}
*/

   	dr_mcontext_t mcontext;
   	memset(&mcontext, 0, sizeof(dr_mcontext_t));
   	mcontext.flags = DR_MC_ALL;
   	mcontext.size = sizeof(dr_mcontext_t);
   	bool result = dr_get_mcontext(dr_get_current_drcontext(), &mcontext);
	printf("displacement is %d\n", displacement);

	reg_t mem_reg;
	if(reg == DR_REG_RAX)
		mem_reg = mcontext.rax;
	else if(reg == DR_REG_RBP)
		mem_reg = mcontext.rbp;
	else if(reg == DR_REG_RBX)
		mem_reg = mcontext.rbx;
	else if(reg == DR_REG_RCX)
		mem_reg = mcontext.rcx;
	else if(reg == DR_REG_RDI)
		mem_reg = mcontext.rdi;
	else if(reg == DR_REG_RDX)
		mem_reg = mcontext.rdx;
	else if(reg == DR_REG_RSI)
		mem_reg = mcontext.rsi;
	else if(reg == DR_REG_RSP)
		mem_reg = mcontext.rsp;
	else
		mem_reg = NULL;
//deal with a null case, rip enum doesn't exist

	if(is_single_precision_instr(opcode)){
   		float op1, op2;
   		printf("Mem reg contents: %f\n", *(float*)(mem_reg + displacement));
   		op2 = *(float*)(mem_reg + displacement);
//		for(r=0; r<16; ++r)
//			for(s=0; s<4; ++s)
//		     		printf("reg %i.%i: %f\n", r, s, 
//					*((float*) &mcontext.ymm[r].u32[s]));
		op1 = *((float*) &mcontext.ymm[regId].u32[0]);
   		dr_fprintf(logF, "%d: %f  %f\n",opcode, op1, op2);
	}
	else{
		double op1, op2;
   		printf("Mem reg contents: %lf\n", *(double*)(mem_reg + displacement));
   		op2 = *(double*)(mem_reg + displacement);
//		for(r=0; r<16; ++r)
 //   			for(s=0; s<2; ++s)
//	     			printf("reg %i.%i: %lf\n", r, s, 
//					*((double*) &mcontext.ymm[r].u64[s]));
		op1 = *((double*) &mcontext.ymm[regId].u64[0]);
   		dr_fprintf(logF, "%d: %lf  %lf\n",opcode, op1, op2);
	}
	print_address(addr);
}




static dr_emit_flags_t
bb_event(void* drcontext, void *tag, instrlist_t *bb, bool for_trace, bool translating)
{
    instr_t *instr, *next_instr;
    int opcode;
    for (instr = instrlist_first(bb); instr != NULL; instr = next_instr) {
        next_instr = instr_get_next(instr);
        opcode = instr_get_opcode(instr);
	if(instr_is_floating(instr)){
   		dr_fprintf(logF, "Has seen FPU instruction with opcode %d\n",opcode);
	
	}
	else if(is_SIMD_packed(opcode)){
   		dr_fprintf(logF, "Has seen SIMD packed instruction with opcode %d\n",opcode);
	}
//AVX?rcpps?

	else if(is_SIMD_arithm(opcode)){
		int is_single = 0;
		printf("opcode is   %d\n", opcode);
    		printf("number of sources  %d\n", instr_num_srcs(instr));  
    		printf("number of dests  %d\n", instr_num_dsts(instr));
		//assert(number of sources = 2);
		opnd_t source1 = instr_get_src(instr,0);
		opnd_t source2 = instr_get_src(instr,1);
		opnd_t dest = instr_get_dst(instr,0);
		if(opnd_is_memory_reference(source1)){
/*
	opnd_t   ref;
    	reg_id_t reg1 = DR_REG_XBX; 
    	reg_id_t reg2 = DR_REG_XCX;
	dr_save_reg(drcontext, bb, instr, reg1, SPILL_SLOT_2);
	dr_save_reg(drcontext, bb, instr, reg2, SPILL_SLOT_3);
	ref = instr_get_src(instr, 0);
	drutil_insert_get_mem_addr(drcontext, bb, instr, ref, reg1, reg2);
*/
	//		dr_print_instr(drcontext, logF, instr, "INSTR: ");
//			dr_print_opnd(drcontext, logF, source1, "OPND1: ");
//			dr_print_opnd(drcontext, logF, source2, "OPND2: ");
			reg_id_t rd = opnd_get_reg(source2);
			reg_id_t rs = opnd_get_reg_used(source1, 0);
			dr_insert_clean_call(drcontext, bb, instr, 
				(void*) callback, true, 5, 
				OPND_CREATE_INTPTR(rs), OPND_CREATE_INTPTR(opnd_get_disp(source1)),
				OPND_CREATE_INTPTR(rd), OPND_CREATE_INTPTR(opcode), OPND_CREATE_INTPTR(instr_get_app_pc(instr)));

//	writeLog(drcontext);	
		}
		else if(opnd_is_reg(source1) && opnd_is_reg(source2)){
			reg_id_t reg1 = opnd_get_reg(source1);
			reg_id_t reg2 = opnd_get_reg(source2);
			printf("register1 is %s\n", get_register_name(reg1));
			printf("register2 is %s\n", get_register_name(reg2));
			
//	writeLog(drcontext);	
//			dr_print_instr(drcontext, logF, instr, "INSTR: ");
//			dr_print_opnd(drcontext, logF, source1, "OPND1: ");
//			dr_print_opnd(drcontext, logF, source2, "OPND2: ");

			dr_insert_clean_call(drcontext,bb,instr, (void*)getRegReg, 
				true, 4, 
				OPND_CREATE_INTPTR(reg1), OPND_CREATE_INTPTR(reg2)
				,OPND_CREATE_INTPTR(opcode), OPND_CREATE_INTPTR(instr_get_app_pc(instr))
			); 
		}
		else{
		//should not be the case, throw an exception
		}
	        fp_count++; 
      }
    }

    return DR_EMIT_DEFAULT;
}

