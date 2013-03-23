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




#define KEY_MAX_LENGTH (256)

//static hashmap* functionmap;
/*
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
    drvector_t lost_bits_vec;
 
} inner_hash_entry;

 typedef struct
{
    int bits;
    double value;  
    double dvalue; 
} vector_entry;

*/
static double *testarr;
static int testcount = 0;


 
void htinit(){
// 	functionmap = newHashmap(10);
	testarr = malloc(17000000*sizeof(double));	
}




#define round(x) ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))


void printFunction(){
//	printf("function name is %s and file %s\n", key, entry->file);
//	dr_fprintf(logOut, "fl=%s\nfn=%s\n",entry->file, entry->function_name);
//	hashmap_it(entry->mapAddrs, &printAddr);
int i;
double loss = 0;
	for(i = 0; i < testcount; i++){
		loss += fabs(testarr[i]);
	}
	double mean = (double)loss/testcount;
	dr_fprintf(logOut, "mean %.13lf\n",mean);


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
printFunction();
//	hashmapProcess(functionmap,&printFunction);
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

printf("Started dr_init\n");

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
print_address(app_pc addr, int bits, double loss, double lossD)
{

    const char* prefix = "PRINT ADDRESS: ";
    drsym_error_t symres;
    drsym_info_t *sym;
    char sbuf[sizeof(*sym) + MAX_SYM_RESULT];
    module_data_t *data;
    data = dr_lookup_module(addr);
    if (data == NULL) {
       // dr_fprintf(logF, "%s data is null "PFX" \n", prefix, addr);
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
        //dr_fprintf(logF, "%s "PFX" %s, function name is: %s, "PIFX", line off "PFX" \n", prefix, addr,
                   //modname, sym->name, addr - data->start - sym->start_offs, sym->line_offs);


	char key_string[KEY_MAX_LENGTH];
	snprintf(key_string, KEY_MAX_LENGTH, "%s", sym->name);

/*	outer_hash_entry* value;
	if(hashmapGet(functionmap, key_string) == 0){
		value = malloc(sizeof(outer_hash_entry));
		value->mapAddrs = hashmap_new();
		snprintf(value->function_name, KEY_MAX_LENGTH, "%s", sym->name);
		snprintf(value->file, KEY_MAX_LENGTH, "%s", sym->file);
		int error = hashmapSet(functionmap, value, value->function_name);
		printf("Inserted success %s %d\n", value->function_name, error);
	}
*/
	testarr[testcount] = lossD;
	testcount++;
	printf("test ins %d\n", testcount);
//	value = hashmapGet(functionmap, key_string);
/*	inner_hash_entry* inVal;
	int error;
//	inVal = malloc(sizeof(inner_hash_entry));
	error = hashmap_get(value->mapAddrs, addr, (void**)(&inVal));
	if(error == MAP_MISSING){
//		printf("Map missing case %x\n", addr);
		//free(inVal);
		inVal = malloc(sizeof(inner_hash_entry));
		inVal->call_count = 0;
		inVal->no_bits = 0;
		inVal->line_number = sym->line;
		if(!drvector_init(&inVal->lost_bits_vec, 10, false,NULL)){
			printf("error in drvector_init bits for %s\n", key_string);	
		}
	}
	inVal->addr = addr;        
	inVal->call_count++;
	printf("Inserting for %x with error code %d and count %d\n", addr, error, 	inVal->call_count);
	if(inVal->no_bits < bits){
		inVal->no_bits = bits;
	}
	if(inVal->loss < loss){
		inVal->loss = loss;
	}
	vector_entry* ve = malloc(sizeof(vector_entry));
	ve->bits = bits;
	ve->value = loss;
	ve->dvalue = lossD;
	if(!drvector_append(&inVal->lost_bits_vec, ve)){
		printf("couldn't add to bits vector\n");
	}
        error = hashmap_put(value->mapAddrs, addr, inVal);
        if(error != MAP_OK){printf("Error %d\n", error);}
*/

	

//add check for line not available
       if (symres == DRSYM_ERROR_LINE_NOT_AVAILABLE) {
           // dr_fprintf(logF, "%s Line is not available\n", prefix);
        } else {
           // dr_fprintf(logF, "Line number is  %s:%"UINT64_FORMAT_CODE" %d\n",
                      // sym->file, sym->line, sym->line_offs);
        }
    } else
      //  dr_fprintf(logF, "%s some error "PFX" \n", prefix, addr);
  
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
       	dr_fprintf(logOut, "events: Average Max\n\n\n");

}



bool is_SIMD_arithm(int opcode){
	return (opcode == OP_addss || opcode == OP_subss
//	|| opcode == OP_addsd ||  opcode == OP_subsd 
	//    || opcode == OP_mulss || opcode == OP_mulsd || opcode == OP_divss || opcode == OP_divsd ||	
	//    opcode == OP_sqrtss || opcode == OP_sqrtsd || opcode == OP_rsqrtss
	);	

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
	double lossD = 0;
	if(is_single_precision_instr(opcode)){
		float op1, op2;
//		for(r=0; r<16; ++r)
//			for(s=0; s<4; ++s)
//		     		printf("reg %i.%i: %f\n", r, s, 
//					*((float*) &mcontext.ymm[r].u32[s]));
		op1 = *((float*) &mcontext.ymm[s1].u32[0]);
		op2 = *((float*) &mcontext.ymm[s2].u32[0]);
       //		dr_fprintf(logF, "%d: %f  %f\n",opcode, op1, op2);
		int exp1, exp2;
		float mant1, mant2;
		mant1 = frexpf(op1, &exp1);
		mant2 = frexpf(op2, &exp2);
		bits = abs(exp1-exp2);

		double dop1 = op1;
		double dop2 = op2;
		if(opcode == OP_addss){
			double dadd = dop1 + dop2;
			float fadd = op1 + op2;
			lossD = dadd - fadd;
//		printf("double %.13lf float %.13f\n", dadd, fadd);
		}
		else{
			double dsub = dop1 - dop2;
			float fsub = op1 - op2;	
			lossD = dsub - fsub;
		}

//		printf("diff of double and float is %.13lf\n", lossD);
	}
	else{
		double op1, op2;
//		for(r=0; r<16; ++r)
//    			for(s=0; s<2; ++s)
//	     			printf("reg %i.%i: %f\n", r, s, 
//					*((double*) &mcontext.ymm[r].u64[s]));
		op1 = *((double*) &mcontext.ymm[s1].u64[0]);
		op2 = *((double*) &mcontext.ymm[s2].u64[0]);
      // 		dr_fprintf(logF, "%d: %.13lf  %.13lf\n",opcode, op1, op2);
		int exp1, exp2;
		double mant1, mant2;
		mant1 = frexp(op1, &exp1);
		mant2 = frexp(op2, &exp2);
		bits = abs(exp1-exp2);
		printf("op1 %.13lf mantissa %.13lf exp %d\n", op1, mant1, exp1);
		printf("op2 %.13lf mantissa %.13lf exp %d\n", op2, mant2, exp2);
	}
	print_address(addr, bits, loss, lossD);
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
	double lossD = 0;
	if(is_single_precision_instr(opcode)){
   		float op1, op2;
//   		printf("Mem reg contents: %f\n", *(float*)(mem_reg + displacement));
   		op2 = *(float*)(mem_reg + displacement);
//		for(r=0; r<16; ++r)
//			for(s=0; s<4; ++s)
//		     		printf("reg %i.%i: %f\n", r, s, 
//					*((float*) &mcontext.ymm[r].u32[s]));
		op1 = *((float*) &mcontext.ymm[regId].u32[0]);
  // 		dr_fprintf(logF, "%d: %f  %f\n",opcode, op1, op2);
		int exp1, exp2;
		float mant1, mant2;
		mant1 = frexpf(op1, &exp1);
		mant2 = frexpf(op2, &exp2);
		bits = abs(exp1-exp2);
		double dop1 = op1;
		double dop2 = op2;
		if(opcode == OP_addss){
			double dadd = dop1 + dop2;
			float fadd = op1 + op2;
			lossD = dadd - fadd;
		//printf("double %.13lf float %.13f\n", dadd, fadd);
		}
		else{
			double dsub = dop1 - dop2;
			float fsub = op1 - op2;	
			lossD = dsub - fsub;
		}
//		printf("diff of double and float is %.13lf\n", lossD);
	}
	else{
		double op1, op2;
   		printf("Mem reg contents: %.13lf\n", *(double*)(mem_reg + displacement));
   		op2 = *(double*)(mem_reg + displacement);
//		for(r=0; r<16; ++r)
 //   			for(s=0; s<2; ++s)
//	     			printf("reg %i.%i: %lf\n", r, s, 
//					*((double*) &mcontext.ymm[r].u64[s]));
		op1 = *((double*) &mcontext.ymm[regId].u64[0]);
  // 		dr_fprintf(logF, "%d: %.13lf  %.13lf\n",opcode, op1, op2);
		int exp1, exp2;
		double mant1, mant2;
		mant1 = frexp(op1, &exp1);
		mant2 = frexp(op2, &exp2);
		bits = abs(exp1-exp2);
		printf("op1 %.13lf mantissa %.13lf exp %d\n", op1, mant1, exp1);
		printf("op2 %.13lf mantissa %.13lf exp %d\n", op2, mant2, exp2);
	}
	print_address(addr, bits, loss, lossD);
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
   	//	dr_fprintf(logF, "Has seen FPU instruction with opcode %d\n",opcode);
	
	}
	else if(is_SIMD_packed(opcode)){
   	//	dr_fprintf(logF, "Has seen SIMD packed instruction with opcode %d\n",opcode);
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

