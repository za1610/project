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
#include <math.h>

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
static char process_path[MAXIMUM_PATH];
static bool callgrind_log_created = false;
static int thread_id_for_log = 0;




////////////////////////ANOTHER HASH TABLE WITH STRING KEY

typedef struct sHashmap hashmap;

#define HASH_BITS 8
#define HASHMAP_ILLEGAL 0   
#define HASHMAP_INSERT 1    
#define HASHMAP_UPDATE 2    
typedef void(*fHashmapProc)(const char* key, const void* datum);

 /* *************************************************************** structures */
 
 typedef struct
 {
     char* key;  
     void* data; 
 } hashmapEntry;
 
 struct sHashmap
 {
     hashmapEntry* array; 
     size_t size,         
            count;        
 };
 
 /* ******************************************************** private functions */
 
 static void rehash(hashmap* map);
 
 static int insert(hashmap* map, void* data, char* key);
 
 static hashmapEntry* find(const hashmap* map, const char* key);
 
 static int compare(const hashmapEntry* lhs, const hashmapEntry* rhs);
 
 static unsigned long hash1(const char* rawKey)
{
     const unsigned char* s = (const unsigned char*) rawKey;
     unsigned long hash = 0;
     
     do
     {
         hash = *s + (hash << 6) + (hash << 16) - hash;
     }
     while (*++s);
     
     return hash;
 }
 
 static unsigned long hash2(const char* rawKey)
 {
     const unsigned char* s = (const unsigned char*) rawKey;
     unsigned long hash = 0;
     
     do
     {
         hash ^= (unsigned long) *s;
         hash += (hash << 1) + (hash << 4) + (hash << 7) + (hash << 8) + (hash << 24);
     }
     while (*++s);
     
     return hash;
 }
 
 static void rehash(hashmap* map)
 {
     size_t size;
     hashmapEntry* array = map->array;
     
     /* double the size of the array */
     size = ++map->size;
     map->size <<= 1;
     map->array = (hashmapEntry*) calloc(sizeof(hashmapEntry), map->size);
     --map->size;
     map->count = 0;
     
     /* re-insert all elements */
     do
     {
         --size;
         if (array[size].key)
             insert(map, array[size].data, array[size].key);
     }
     while (size);
     
     /* return unused memory */
     free(array);
 }
 
 static hashmapEntry* find(const hashmap* map, const char* key)
 {
     unsigned long index, step, initialIndex;
     hashmapEntry* freeEntry = 0;
     
     initialIndex = index = hash1(key) & map->size;
     
     /* first try */
     if (map->array[index].key)
     {
         if (!strcmp(map->array[index].key, key))
             return &map->array[index];
     }
     else if (!map->array[index].data)
     {
         return &map->array[index];
     }
     else
     {
         freeEntry = &map->array[index];
     }
     
     /* collision */
     step = (hash2(key) % map->size) + 1;
     
     do
     {
         index = (index + step) & map->size;
         
         if (map->array[index].key)
         {
             if (!strcmp(map->array[index].key, key))
                 return &map->array[index];
         }
         else if (!map->array[index].data)
         {
             return (freeEntry) ? freeEntry : &map->array[index];
         }
         else if (!freeEntry)
         {
             freeEntry = &map->array[index];
         }
     }
     while (index != initialIndex);
     
     return freeEntry;
 }
 
 static int insert(hashmap* map, void* data, char* key)
 {
     hashmapEntry* entry;
     
     if (map->size == map->count)
         rehash(map);
     
     do
     {
         entry = find(map, key);
         
         if (entry)
         {
             entry->data = data;
             
             if (entry->key)
             {
                 /* updated the entry */
                 free(key);
                 return HASHMAP_UPDATE;
             }
             else
             {
                 /* inserted the entry */
                 ++map->count;
                 entry->key = key;
                 return HASHMAP_INSERT;
             }
         }
         
         rehash(map);
     }
     while (1);
 }
 
 static int compare(const hashmapEntry* lhs, const hashmapEntry* rhs)
 {
     return (lhs->key)  ? (rhs->key) ? strcmp(lhs->key, rhs->key) : -1
                        : (rhs->key) ? 1 : 0;
 }
 
 /* ******************************************************* exported functions */
 
 hashmap* newHashmap(unsigned int hint)
 {
     hashmap* map = (hashmap*) malloc(sizeof(hashmap));
    
    if (hint < 4)
     {
         hint = 4;
     }
     else if (hint & (hint - 1))
     {
         unsigned int i = 1;
         
         do
         {
             hint |= (hint >> i);
             i <<= 1;
         }
         while (i <= (sizeof(hint) << 2));
         ++hint;
     }
     
     map->array = (hashmapEntry*) calloc(sizeof(hashmapEntry), hint);
     map->size = hint - 1;
     map->count = 0;
     
     return map;
 }
 
 void deleteHashmap(hashmap* map)
 {
     unsigned long index = 0;
     
    // assert(map);
     
     do
     {
         if (map->array[index].key)
             free(map->array[index].key);
     }
     while (++index <= map->size);
 
     free(map->array);
     free(map);
 }
 
 int hashmapSet(hashmap* map, void* data, const char* key)
 {
     return (map && key && *key)  ? insert(map, data, strdup(key))
                                  : HASHMAP_ILLEGAL;
 }
 
 void* hashmapGet(const hashmap* map, const char* key)
 {
     hashmapEntry* entry;
     //assert(map && key && *key);
     
     entry = find(map, key);
     
     if (entry && entry->key)
         return entry->data;
     
     return 0;
 }
 
 void* hashmapRemove(hashmap* map, const char* key)
 {
     void* res = 0;
     hashmapEntry* entry;
     
     //assert(map && key && *key);
     
     entry = find(map, key);
     
     if (entry && entry->key)
     {
         --map->count;
         
         free(entry->key);
         entry->key = 0;
         res = entry->data;
         
         /* setting exist to one indicates that this entry was already in use */
         entry->data = (void*) 1;
     }
     
     return res;
 }
 
 void hashmapProcess(const hashmap* map, fHashmapProc proc)
 {
     hashmapEntry* array;
     size_t size;
     int i;
     
     //assert(map);
     
     size = map->size + 1;
     array = (hashmapEntry*) malloc(sizeof(hashmapEntry) * size);
   
     memcpy(array, map->array, sizeof(hashmapEntry) * size);
     qsort(array, size, sizeof(hashmapEntry),
                 (int(*)(const void*, const void*)) compare);
     
     for (i = 0; i < map->count; ++i)
         proc(array[i].key, array[i].data);
     
     free(array);
}





//////////////////////////////////HASHTABLE
#define INITIAL_SIZE 10
//#define MAX_CHAIN_LENGTH (8)

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
//#define KEY_COUNT 10 
/*
typedef struct data_struct_s
{
    int key;
    int number;
} data_struct_t;
*/

static map_t mymap;

static hashmap* functionmap;

typedef struct 
{
   char function_name[KEY_MAX_LENGTH];
   char file[KEY_MAX_LENGTH];
   bool hash_created;
   map_t mapAddrs;
} outer_hash_entry;

typedef struct
{
    int addr;  
    int line_number;
    int call_count;
    int no_bits;
    double loss;     
 
} inner_hash_entry;
 
void htinit(){
 	functionmap = newHashmap(10);	
}

int hashmap_it(map_t in, PFany f) {
	int i;
	any_t item = NULL;
	/* Cast the hashmap */
	hashmap_map* m = (hashmap_map*) in;

	/* On empty hashmap, return immediately */
	if (hashmap_length(m) <= 0)
		return MAP_MISSING;	

	/* Linear probing */
	for(i = 0; i< m->table_size; i++)
		if(m->data[i].in_use != 0) {
			if(m->data[i].key != 0){
		//	printf("inside iteration %d\n", m->data[i].key);
			any_t data = (any_t) (m->data[i].data);
			int status = f(item, data);
			if (status != MAP_OK) {
				return status;
			}
			}
		}

        return MAP_OK;
}


int printAddr(any_t t1, inner_hash_entry* entry){
	printf("addr %d line %d\n", entry->line_number, 10);
        dr_fprintf(logOut, ""PIFX" %d %d\n",entry->addr,entry->line_number,entry->call_count);
return 0;
}


void printFunction(char* key, outer_hash_entry* entry){
	printf("function name is %s and file %s\n", key, entry->file);
	dr_fprintf(logOut, "fl=%s\nfn=%s\n",entry->file, entry->function_name);
	hashmap_it(entry->mapAddrs, &printAddr);
}

//////////////HASHTABLE end


struct FP{
unsigned int mantissa: 23;
unsigned int exponent: 8;
unsigned int sign: 1;
};


struct DP{
unsigned long mantissa: 52;
unsigned int exponent: 11;
unsigned int sign: 1;
};

void printht(){
	hashmapProcess(functionmap,&printFunction);

float x, y;
int z;
x = 10.123;
y = frexpf(x, &z);
printf("!!!!!!!!!!! %f %d\n", y, z);
//int fl = *(int*)&x;
//printf("hex rep %x\n", fl);
struct FP* fp = (struct FP*)&x;
printf("biased exponent %u mantissa = %u \n",fp->exponent, fp->mantissa);

/*
	outer_hash_entry* entry = hashmapGet(functionmap, "main");
	inner_hash_entry* inVal;
	int error;
//inVal = malloc(sizeof(inner_hash_entry));
	int i;

dr_fprintf(logOut, "fl=%s\nfn=%s\n",entry->file, entry->function_name);
	for(i = 0; i < size_arr; i++){

		printf("Looking for addr %d\n", addr_arr[i]);
		error = hashmap_get(entry->mapAddrs, addr_arr[i], (void**)(&inVal));
		if(error == MAP_OK){
			printf("%d %d %d bits %d\n", inVal->addr, inVal->call_count, inVal->line_number, inVal->no_bits );
        		dr_fprintf(logOut, ""PIFX" %d %d\n",inVal->addr,inVal->line_number,inVal->call_count);
		}
		else 
			printf("not this function %d\n", error);
}
*/
/*
	entry = hashmapGet(functionmap, "substr");

dr_fprintf(logOut, "fl=%s\nfn=%s\n",entry->file, entry->function_name);
	for(i = 0; i < size_arr; i++){

		printf("Looking for addr %d\n", addr_arr[i]);
		error = hashmap_get(entry->mapAddrs, addr_arr[i], (void**)(&inVal));
		if(error == MAP_OK){
			printf("%d %d %d\n", inVal->addr, inVal->call_count, inVal->line_number );
        		dr_fprintf(logOut, ""PIFX" %d %d\n",inVal->addr,inVal->line_number,inVal->call_count);
		}
		else 
			printf("not this function %d\n", error);

}
*/
}




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
	thread_id_for_log = dr_get_thread_id(drcontext);
}


static void
print_address(app_pc addr, int bits, double loss)
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
    snprintf(process_path, MAXIMUM_PATH,"%s",data->full_path);


    if(!callgrind_log_created){
	writeCallgrind(thread_id_for_log);
	callgrind_log_created = true;
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


	char key_string[KEY_MAX_LENGTH];
	snprintf(key_string, KEY_MAX_LENGTH, "%s", sym->name);

	outer_hash_entry* value;
	if(hashmapGet(functionmap, key_string) == 0){
		value = malloc(sizeof(outer_hash_entry));
		value->mapAddrs = hashmap_new();
		snprintf(value->function_name, KEY_MAX_LENGTH, "%s", sym->name);
		snprintf(value->file, KEY_MAX_LENGTH, "%s", sym->file);
		int error = hashmapSet(functionmap, value, value->function_name);
		printf("Inserted success %d\n", error);
	}
	value = hashmapGet(functionmap, key_string);
	inner_hash_entry* inVal;
	int error;
	inVal = malloc(sizeof(inner_hash_entry));
	error = hashmap_get(value->mapAddrs, addr, (void**)(&inVal));
	if(error == MAP_MISSING){
		printf("Map missing case\n");
		free(inVal);
		inVal = malloc(sizeof(inner_hash_entry));
		addr_arr[size_arr] = addr;
		size_arr++;
		inVal->call_count = 0;
		inVal->line_number = sym->line;
	}

	inVal->addr = addr;        
	inVal->call_count++;
	inVal->no_bits = bits;
	inVal->loss = loss;
        error = hashmap_put(value->mapAddrs, addr, inVal);
        if(error!=MAP_OK){printf("Error\n");}


	if(hashmapGet(functionmap, key_string) == 0){
		printf("Error, didn't insert\n");

	}


//add check for line not available
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


void
writeCallgrind(int thread_id){
	char logname[MAXIMUM_PATH];
	char *dirsep;
    	int len;
	char * tmp = process_path;

	len = dr_snprintf(logname, sizeof(logname)/sizeof(logname[0]),
                      "%s", tmp);

	DR_ASSERT(len > 0);
	for (dirsep = logname + len; *dirsep != '/'; dirsep--)
        DR_ASSERT(dirsep > logname);
    	len = dr_snprintf(dirsep + 1,
                      (sizeof(logname) - (dirsep - logname))/sizeof(logname[0]),
                      "callgrind.%d.out", thread_id);
    	DR_ASSERT(len > 0);
    	NULL_TERMINATE(logname);
    	logOut = dr_open_file(logname, 
                             DR_FILE_WRITE_OVERWRITE | DR_FILE_ALLOW_LARGE);
    	DR_ASSERT(logOut != INVALID_FILE);
//    	dr_log(drcontext, LOG_ALL, 1, 
 //          "floating point: log for thread %d is fp.%03d\n",thread_id, thread_id);
	#ifdef SHOW_RESULTS
    	if (dr_is_notify_on()) {
//        	dr_fprintf(STDERR, "<floating point instruction operands for thread %d in %s>\n",
//                  dr_get_thread_id(drcontext), logname);
    	}
	#endif	

       	dr_fprintf(logOut, "version: 1\n");
       	dr_fprintf(logOut, "creator: callgrind-3.6.1-Debian\n");
       	dr_fprintf(logOut, "positions: instr line\n");
       	dr_fprintf(logOut, "events: Ir\n\n\n");

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
	dr_mcontext_t mcontext;
   	memset(&mcontext, 0, sizeof(dr_mcontext_t));
   	mcontext.flags = DR_MC_MULTIMEDIA;
   	mcontext.size = sizeof(dr_mcontext_t);
   	bool result = dr_get_mcontext(dr_get_current_drcontext(), &mcontext);
	int r, s;
	int bits = 0;
	double loss = 0;
	if(is_single_precision_instr(opcode)){
		float op1, op2;
//		for(r=0; r<16; ++r)
//			for(s=0; s<4; ++s)
//		     		printf("reg %i.%i: %f\n", r, s, 
//					*((float*) &mcontext.ymm[r].u32[s]));
		op1 = *((float*) &mcontext.ymm[s1].u32[0]);
		op2 = *((float*) &mcontext.ymm[s2].u32[0]);
       		dr_fprintf(logF, "%d: %f  %f\n",opcode, op1, op2);
		int exp1, exp2;
		float mant1, mant2;
		/*mant1 = frexpf(op1, &exp1);
		mant2 = frexpf(op2, &exp2);
		bits = abs(exp1-exp2);
		printf("op1 %g mantissa %g exp %d\n", op1, mant1, exp1);
		printf("op2 %g mantissa %g exp %d\n", op2, mant2, exp2);
		*/
		//////adding zero case
		struct FP* fp = (struct FP*)&op1;
	        exp1 = fp->exponent - 127;
		mant1 = fp->mantissa;	
		fp = (struct FP*)&op2;
	        exp2 = fp->exponent - 127;
		mant2 = fp->mantissa;
		bits =abs(exp1-exp2);	
		printf("op1 %g mantissa %g exp %d\n", op1, mant1, exp1);
		printf("op2 %g mantissa %g exp %d\n", op2, mant2, exp2);
		int mask = 0;
		int ind = 0;
		for(ind= 0; ind < bits; ind++){
			mask = mask << 1;
			mask = mask | 1;
		}
		printf("mask value in hex %x\n", mask);
		if(exp1 < exp2){

		}
		else{

		}


	}
	else{
		double op1, op2;
//		for(r=0; r<16; ++r)
//    			for(s=0; s<2; ++s)
//	     			printf("reg %i.%i: %f\n", r, s, 
//					*((double*) &mcontext.ymm[r].u64[s]));
		op1 = *((double*) &mcontext.ymm[s1].u64[0]);
		op2 = *((double*) &mcontext.ymm[s2].u64[0]);
       		dr_fprintf(logF, "%d: %lf  %lf\n",opcode, op1, op2);
		int exp1, exp2;
		double mant1, mant2;
		/*mant1 = frexp(op1, &exp1);
		mant2 = frexp(op2, &exp2);
		bits = abs(exp1-exp2);
		printf("op1 %g mantissa %g exp %d\n", op1, mant1, exp1);
		printf("op2 %g mantissa %g exp %d\n", op2, mant2, exp2);
		*/
		struct DP* dp = (struct DP*)&op1;
	        exp1 = dp->exponent - 1023;	
		mant1 = dp->mantissa;
		dp = (struct DP*)&op2;
	        exp2 = dp->exponent - 1023;
		mant2 = dp->mantissa;
		bits =abs(exp1-exp2);
		printf("op1 %g mantissa %g exp %d\n", op1, mant1, exp1);
		printf("op2 %g mantissa %g exp %d\n", op2, mant2, exp2);
	}
	print_address(addr, bits, loss);
}

static void
callback(reg_id_t reg, int displacement, reg_id_t destReg, int opcode, app_pc addr){
	int r, s;
   	const char * destRegName = get_register_name(destReg);
   	int regId = atoi(destRegName + 3 * sizeof(char));
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

	int bits = 0;
	double loss = 0;
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
		int exp1, exp2;
		float mant1, mant2;
		/*mant1 = frexpf(op1, &exp1);
		mant2 = frexpf(op2, &exp2);
		bits = abs(exp1-exp2);
		printf("op1 %g mantissa %g exp %d\n", op1, mant1, exp1);
		printf("op2 %g mantissa %g exp %d\n", op2, mant2, exp2);*/
		struct FP* fp = (struct FP*)&op1;
	        exp1 = fp->exponent - 127;	
		mant1 = fp->mantissa;
		fp = (struct FP*)&op2;
	        exp2 = fp->exponent - 127;
		mant2 = fp->mantissa;
		bits =abs(exp1-exp2);	
		printf("op1 %g mantissa %g exp %d\n", op1, mant1, exp1);
		printf("op2 %g mantissa %g exp %d\n", op2, mant2, exp2);
		unsigned int mask = 0;
		int ind = 0;
		for(ind= 0; ind < bits; ind++){
			mask = mask << 1;
			mask = mask | 1;
		
		}
		unsigned int expmask = 4286578688;
		unsigned int totalmask = expmask | mask;
		printf("mask value in hex %x exp %x total %x\n", mask, expmask, totalmask);
		if(exp1>exp2){
			int intop2 = *(int*)&op2;
			printf("op2 in hex %x\n", intop2);
			int bin = intop2 & totalmask;
			printf("lost in binary %x\n", bin);
			float lostbits = *(float*)&bin;
			printf("lost bits are %f\n", lostbits);
		}
		else{
			int intop2 = *(int*)&op1;
			printf("op2 in hex %x\n", intop2);
			int bin = intop2 & totalmask;
			printf("lost in binary %x\n", bin);
			float lostbits = *(float*)&bin;
			printf("lost bits are %f\n", lostbits);
		}

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
		int exp1, exp2;
		double mant1, mant2;
		/*mant1 = frexp(op1, &exp1);
		mant2 = frexp(op2, &exp2);
		bits = abs(exp1-exp2);
		printf("op1 %g mantissa %g exp %d\n", op1, mant1, exp1);
		printf("op2 %g mantissa %g exp %d\n", op2, mant2, exp2);
		*/
		struct DP* dp = (struct DP*)&op1;
	        exp1 = dp->exponent - 1023;	
		mant1 = dp->mantissa;
		dp = (struct DP*)&op2;
	        exp2 = dp->exponent - 1023;
		mant2 = dp->mantissa;
		bits =abs(exp1-exp2);	
		printf("op1 %g mantissa %g exp %d\n", op1, mant1, exp1);
		printf("op2 %g mantissa %g exp %d\n", op2, mant2, exp2);
	
	}
	print_address(addr, bits, loss);
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
//		printf("opcode is   %d\n", opcode);
//    		printf("number of sources  %d\n", instr_num_srcs(instr));  
 //   		printf("number of dests  %d\n", instr_num_dsts(instr));
		//assert(number of sources = 2);
		opnd_t source1 = instr_get_src(instr,0);
		opnd_t source2 = instr_get_src(instr,1);
		opnd_t dest = instr_get_dst(instr,0);
		if(opnd_is_memory_reference(source1)){
	//		dr_print_instr(drcontext, logF, instr, "INSTR: ");
//			dr_print_opnd(drcontext, logF, source1, "OPND1: ");
//			dr_print_opnd(drcontext, logF, source2, "OPND2: ");
			reg_id_t rd = opnd_get_reg(source2);
			reg_id_t rs = opnd_get_reg_used(source1, 0);
			dr_insert_clean_call(drcontext, bb, instr, 
				(void*) callback, true, 5, 
				OPND_CREATE_INTPTR(rs), OPND_CREATE_INTPTR(opnd_get_disp(source1)),
				OPND_CREATE_INTPTR(rd), OPND_CREATE_INTPTR(opcode), OPND_CREATE_INTPTR(instr_get_app_pc(instr)));

		}
		else if(opnd_is_reg(source1) && opnd_is_reg(source2)){
			reg_id_t reg1 = opnd_get_reg(source1);
			reg_id_t reg2 = opnd_get_reg(source2);
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

