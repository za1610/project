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

#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <utility>

# define MAX_SYM_RESULT 256
//# define MAX_MEMORY 194700000

# define MAX_MEMORY 350000000

#ifdef WINDOWS
# define DISPLAY_STRING(msg) dr_messagebox(msg)
#else
# define DISPLAY_STRING(msg) dr_printf("%s\n", msg);
#endif

#define NULL_TERMINATE(buf) buf[(sizeof(buf)/sizeof(buf[0])) - 1] = '\0'

// Work-around for broken calloc

void *fixed_calloc(const size_t nmemb, const size_t element_size)
{
  const size_t size = nmemb * element_size;
  void *const data = malloc(size);

  if (data != NULL)
    memset(data, 0, size);

  return data;
}

#define calloc fixed_calloc

static dr_emit_flags_t bb_event(void *drcontext, void *tag, instrlist_t *bb,
                                bool for_trace, bool translating);
static void exit_event(void);

static void writeCallgrind(int thread_id);

file_t logF;
file_t logOut;
static int fp_count = 0;
static void *count_mutex; /* for multithread support */
static client_id_t client_id;
static bool callgrind_log_created = false;
static int thread_id_for_log = 0;

static double memory_used = 0;


typedef struct{
//	int calls;
  double dv;
  float sv;
}
memv_entry;

typedef struct
{
    int bits;
//  double value;  
    double dvalue; 
} vector_entry;

typedef struct
{

    double dsum;
    float fsum;
    app_pc addr;  
    int line_number;
    int call_count;

std::map<float*, memv_entry*> mem_map;

    int no_bits; //max num of bits
    double loss;  //max loss
    std::vector<vector_entry> lost_bits_vec;
    bool use_vector;
bool memory;	
	double sum_of_loss;
	long int sum_of_bits;
} inner_hash_entry;

typedef struct 
{
   std::string function_name;
   std::string file;
   bool hash_created;
   std::map<app_pc, inner_hash_entry*> mapAddrs;
} outer_hash_entry;



typedef struct
{
//  double value;
  app_pc addr;
  double dsum;
  float fsum;
  std::vector<memv_entry> values;
}
mem_map_entry;

static std::map<std::string, outer_hash_entry> functionmap;

//static std::map<float*, mem_map_entry*> memorymap;



static long int total = 0;

#define round(x) ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))




void printMemVar(const std::pair<float*, memv_entry*>& pair){
        const memv_entry* entry = pair.second;
	printf("\tmemory address used for storing a var: %x dsum is %.13lf and fsum is %.13f\n", pair.first, entry->dv,  entry->sv);
//	*max = entry->dv - entry->sv;
}

int printAddr(const std::pair<app_pc, inner_hash_entry*>& pair){
        const inner_hash_entry* entry = pair.second;

	double mem_max = 0;
	int test = 0;
	float* testaddr = NULL;
	if(entry->memory){

		for (std::map<float*, memv_entry*>::const_iterator it=entry->mem_map.begin(); it!=entry->mem_map.end(); ++it){
			double diff = fabs(it->second->dv - it->second->sv); 
	/*		if(diff == 0){
				printf("Difference is zero:  %.13lf   %.13f\n", it->second->dv, it->second->sv);

	
			}*/
			if(diff > mem_max){
				mem_max = diff;
//				test = it->second->calls;
				testaddr = it->first;
			}

		}

  		printf(" "PIFX" double sum: %.13lf, float sum %.13f number of calls %d, mem size is %d\n",entry->addr, entry->dsum, entry->fsum, entry->call_count, entry->mem_map.size()); 
		printf("!!!!!! %x  %d\n", testaddr, test);
//		if(entry->mem_map.size() < 300) 
 //  		std::for_each(entry->mem_map.begin(), entry->mem_map.end(), &printMemVar);
	}
	int i;

	const vector_entry* ve;
	int num_of_bits = 0;
	double loss = 0;

	int bits_distr[25];
	for(i = 0; i < 25; i++)
		bits_distr[i] = 0;
        if(entry->use_vector){

	  for(i = 0; i < entry->call_count; i++){
                ve = &entry->lost_bits_vec[i];
		num_of_bits += ve->bits;
		bits_distr[ve->bits]++;
		loss += fabs(ve->dvalue);
//		printf("drvector bits %d %x %d %.13lf %.13lf\n",i, entry->addr, ve->bits, ve->value, fabs(ve->dvalue)); 
	  }
/*	
	int c = 0;
	for(i = 0 ; i < 25; i++){
		c+=bits_distr[i];
		dr_fprintf(logF, "%d: %d  \n",i, bits_distr[i]);
	}
		dr_fprintf(logF, "callcount %d \n",c);
*/
	  double mean = (double)loss/entry->call_count;
	  double sumup = 0;
//		printf("Loss is %.13lf with calls %d\nmean is %.13lf\n", loss, entry->call_count, mean);  
	  double sumdown = 0;        
	  for(i = 0; i < entry->call_count; i++){
            ve = &entry->lost_bits_vec[i];
	    sumup += (fabs(ve->dvalue) - mean)*(fabs(ve->dvalue) - mean)*(fabs(ve->dvalue) - mean);	
	    sumdown += (fabs(ve->dvalue) - mean)*(fabs(ve->dvalue) - mean);
	  }
	  double nsqrt = sqrt((double)entry->call_count);
	  sumdown = sqrt(sumdown*sumdown*sumdown);
	  double skewness = 0;
	  if(sumdown != 0)
		skewness = nsqrt* ((double)sumup/sumdown);
/*
	double mean1 =  (double) entry->sum_of_loss/entry->call_count;
	double sk1 = ((double)entry->sum_of_cubes/entry->call_count - mean1*mean1*mean1);
	double vr =  (double)entry->sum_of_squares/entry->call_count - mean1*mean1;
	double sk2 = sk1/ sqrt(vr*vr*vr);
	dr_fprintf(logOut, ""PIFX" %d %ld %d  mean %.13lf skewness %.13lf skewness vec %.13lf \n",entry->addr,entry->line_number,
				round((double)entry->sum_of_bits/entry->call_count),entry->no_bits, (double) entry->sum_of_loss/entry->call_count, sk2, skewness);
*/
		if(entry->memory){
	  		dr_fprintf(logOut, ""PIFX" %d %ld %d %.0lf  %.0lf              %.0lf\n",entry->addr,entry->line_number,
				round((double)num_of_bits/entry->call_count),entry->no_bits, 
				mean*10000000000000,  entry->loss*10000000000000, mem_max*10000000000000);

			//dr_fprintf(logOut, ""PIFX" %d %ld %d %.13lf  %.13lf  %.13lf %.13lf\n",entry->addr,entry->line_number,
		//		round((double)num_of_bits/entry->call_count),entry->no_bits, 
		//		mean,  entry->loss, mem_max, skewness);

		}
		else{
			dr_fprintf(logOut, ""PIFX" %d %ld %d %.0lf  %.0lf  \n",entry->addr,entry->line_number,
				round((double)num_of_bits/entry->call_count),entry->no_bits, 
				mean*10000000000000,  entry->loss*10000000000000);
//			dr_fprintf(logOut, ""PIFX" %d %ld %d %.13lf  %.13lf %.13lf\n",entry->addr,entry->line_number,
//				round((double)num_of_bits/entry->call_count),entry->no_bits, 
//				mean,  entry->loss, skewness);


		}
        }
	else{
//printf("NOT MEMORY\n");
	  double mean =  (double) entry->sum_of_loss/entry->call_count;
	  if(entry->memory){
	  	dr_fprintf(logOut, ""PIFX" %d %ld %d %.0lf %.0lf                %.0lf\n",entry->addr,entry->line_number,
				round((double)entry->sum_of_bits/entry->call_count),entry->no_bits, 
				mean*10000000000000,  entry->loss*10000000000000, mem_max*10000000000000);
	//		dr_fprintf(logOut, ""PIFX" %d %ld %d %.13lf  %.13lf  %.13lf %.13lf\n",entry->addr,entry->line_number,
	//			round((double)num_of_bits/entry->call_count),entry->no_bits, 
	//			mean,  entry->loss, mem_max);


	  }
	  else{
 		dr_fprintf(logOut, ""PIFX" %d %ld %d %.0lf %.0lf \n",entry->addr,entry->line_number,
				round((double)entry->sum_of_bits/entry->call_count),entry->no_bits, 
				mean*10000000000000,  entry->loss*10000000000000);
	//		dr_fprintf(logOut, ""PIFX" %d %ld %d %.13lf  %.13lf %.13lf \n",entry->addr,entry->line_number,
	//			round((double)num_of_bits/entry->call_count),entry->no_bits, 
	//			mean,  entry->loss);


	  }
	}
return 0;
}


void printFunction(const std::pair<std::string, outer_hash_entry>& pair){
   const outer_hash_entry *entry = &pair.second;
	std::string obj_name = entry->file;
	int found = obj_name.find('.');
	if(found == -1){     
		found = strlen(obj_name.c_str());
	}

	obj_name.resize(found);

	dr_fprintf(logOut, "ob=%s\nfl=%s\nfn=%s\n",obj_name.c_str(), entry->file.c_str(), entry->function_name.c_str());
	printf("Number of fp instrs in function %s is %d:\n", entry->function_name.c_str(), entry->mapAddrs.size());
	dr_fprintf(logF, "%s\n",  entry->function_name.c_str());
        std::for_each(entry->mapAddrs.begin(), entry->mapAddrs.end(), &printAddr);
}





void printMemory(const std::pair<float*, mem_map_entry*>& pair){
	printf("memory: %x ", pair.first);
        const mem_map_entry* entry = pair.second;
	int i;
	const memv_entry* ve;
     
	printf(" with vector %d and pc "PIFX"\n", entry->values.size(), entry->addr);
	printf(" fsum is  %.13f and dsum is %.13lf\n", entry->fsum, entry->dsum);

}


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

        std::for_each(functionmap.begin(), functionmap.end(), &printFunction);
    //    std::for_each(memorymap.begin(), memorymap.end(), &printMemory);
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


}

static void
exit_event(void)
{


printf("In exit event\n");
#ifdef SHOW_RESULTS
    char msg[512];
    int len = dr_snprintf(msg, sizeof(msg),
                      "Instrumentation results:\n"
                      "Processed %d instructions\n"
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
    if(!callgrind_log_created){
	writeCallgrind(thread_id_for_log);
	callgrind_log_created = true;
    }

    printht();
    dr_close_file(logOut);
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
        dr_close_file(logOut);
}


static void
print_address(inner_hash_entry *inVal, int bits, double loss, double lossD, float op1, float op2)
{
	++inVal->call_count;
	++total;
	if(inVal->no_bits < bits){
		inVal->no_bits = bits;
	}

	if(inVal->loss < fabs(lossD)){
		inVal->loss = fabs(lossD);
	}
	if(memory_used < MAX_MEMORY){
	memory_used += 12;
	  vector_entry ve;
	  ve.bits = bits;
	  ve.dvalue = lossD;
          inVal->lost_bits_vec.push_back(ve);
	if(memory_used > 2000000000)
	printf("MEMORY: %lf\n", memory_used);
        }
	else
	{
//	printf("exceded LIMIT: %lf\n", memory_used);
	  inVal->use_vector = false;
	}
        inVal->sum_of_loss += fabs(lossD);
	inVal->sum_of_bits += bits;

   //     inVal->sum_of_squares += lossD*lossD;
   //     inVal->sum_of_cubes += lossD*lossD*lossD;
}


void
writeCallgrind(int thread_id){
	char logname[MAXIMUM_PATH];
    	int len = dr_snprintf(logname, sizeof(logname), 
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
       	dr_fprintf(logOut, "events: Average_bits Max_bits Mean_diff  Max_diff Accumulative_error\n\n\n");
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

static int getMMRegisterID(const reg_id_t r)
{
  if (r >= DR_REG_START_YMM && r <= DR_REG_STOP_YMM)
    return r - DR_REG_START_YMM;
  
  if (r >= DR_REG_START_XMM && r <= DR_REG_STOP_XMM)
    return r - DR_REG_START_XMM;

  DR_ASSERT_MSG(0, "Unable to determine ID of multimedia register");
}


static void 
getRegReg(reg_id_t r1, reg_id_t r2, int opcode, inner_hash_entry *entry , reg_id_t mem_reg, int dd, float * rel_addr){
//		printf("In getRegReg\n");

//r2 dest, r1 source	

	const int s1        = getMMRegisterID(r1);
	const int s2        = getMMRegisterID(r2);
	dr_mcontext_t mcontext;
   	memset(&mcontext, 0, sizeof(dr_mcontext_t));
   	mcontext.flags = DR_MC_ALL;
   	mcontext.size = sizeof(dr_mcontext_t);
   	bool result = dr_get_mcontext(dr_get_current_drcontext(), &mcontext);
	int r, s;
	int bits = 0;
	double loss = 0;
	double lossD = 0;
	float * mem_var;
	if(mem_reg == NULL && rel_addr != NULL){
		mem_var = rel_addr;
	}
	else{
		reg_t mem_reg1 = reg_get_value(mem_reg, &mcontext);
        	mem_var = (float*)(mem_reg1 + dd);
	}
/*	app_pc ff = (app_pc) 0x401fc6; 
	if(entry->addr ==ff ){
	if(mem_reg == NULL) printf("register is NULL\n");
		printf("!!!!!!!!!!! %x  %d\n", mem_var,  dd);
	}
*/
	float op1, op2;
	if(is_single_precision_instr(opcode)){
		op1 = *((float*) &mcontext.ymm[s1].u32[0]);
//printf("OP1 is %f\n", op1);
		op2 = *((float*) &mcontext.ymm[s2].u32[0]);
		
		int exp1, exp2;
		float mant1, mant2;
		mant1 = frexpf(op1, &exp1);
		mant2 = frexpf(op2, &exp2);
		bits = abs(exp1-exp2);
		if(bits > 24){
			bits = 24;
		}
		double dop1 = op1;
		double dop2 = op2;
		if(opcode == OP_addss){
			double dadd = dop1 + dop2;
			float fadd = op1 + op2;
			lossD = dadd - fadd;
//printf("op1 is %f and op2 is %f\n", op1,op2);
//op2 source, op1 dest
//source can be mem or reg; dest is reg


			//to initialize to the initial sum at the beginning
			if(entry->call_count == 0){
				entry->dsum = dop1;
				entry->fsum = op1;
 //				printf("op1 is %f and op2 is %f\n", op1,op2);
			}

			if(entry->memory){
        			if (entry->mem_map.find(mem_var) == entry->mem_map.end()) {
//		printf("didnt findmemory  "PIFX"\n", mem_var);
					memv_entry * m = new memv_entry();
 					m->dv = dop1;
					m->sv = op1;
				//	m->calls = 0;
			 		entry->mem_map[mem_var] = m;
		//	dr_fprintf(logF,""PIFX": %s %d %x\n", entry->addr, get_register_name(mem_reg), dd, mem_var);
//			printf("CREATNIG NEW MEM ENTRY 	%x\n",mem_var );
				}
				memv_entry * m = entry->mem_map[mem_var];
				m->dv +=dop2;
				m->sv +=op2;
//				m->calls++;
				entry->dsum += dop2;
				entry->fsum += op2;
			}
//		printf("double %.13lf float %.13f\n", dadd, fadd);
		}
		else{
			double dsub = dop2 - dop1;
			float fsub = op2 - op1;	
			lossD = dsub - fsub;


			//to initialize to the initial sum at the beginning
			if(entry->call_count == 0){
				entry->dsum = dop2;
				entry->fsum = op2;
 //				printf("op1 is %f and op2 is %f res is %lf\n", op1,op2, dsub);
			}
			if(entry->memory){
        			if (entry->mem_map.find(mem_var) == entry->mem_map.end()) {
					memv_entry * m = new memv_entry();
 					m->dv = dop2;
					m->sv = op2;
//					m->calls = 0;
			 		entry->mem_map[mem_var] = m;
		//	dr_fprintf(logF,""PIFX": %s %d %x\n", entry->addr, get_register_name(mem_reg), dd, mem_var);
//			printf("CREATNIG NEW MEM ENTRY 	%x\n",mem_var );
				}
				memv_entry * m = entry->mem_map[mem_var];
				m->dv -=dop1;
				m->sv -=op1;
//				m->calls++;
				
				entry->dsum -= dop1;
				entry->fsum -= op1;
			}


//op1 is subtrahend(b); a - b = c 
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
      // 		dr_fprintf(logF, "%d: %.13lf  %.13lf\n",opcode, op1, op2);
		int exp1, exp2;
		double mant1, mant2;
		mant1 = frexp(op1, &exp1);
		mant2 = frexp(op2, &exp2);
		bits = abs(exp1-exp2);
		printf("op1 %.13lf mantissa %.13lf exp %d\n", op1, mant1, exp1);
		printf("op2 %.13lf mantissa %.13lf exp %d\n", op2, mant2, exp2);
	}
	print_address(entry, bits, loss, lossD, op1, op2);
}

inner_hash_entry *get_inner_hash_entry(app_pc addr, bool memory)
{
    char sbuf[sizeof(drsym_info_t) + MAX_SYM_RESULT];
    module_data_t *data = dr_lookup_module(addr);
    if (data == NULL) {
       // dr_fprintf(logF, "%s data is null "PFX" \n", prefix, addr);
        return NULL;
    }

    drsym_info_t *sym = (drsym_info_t *) sbuf;
    sym->struct_size = sizeof(drsym_info_t);
    sym->name_size = MAX_SYM_RESULT;   

    drsym_error_t symres = drsym_lookup_address(data->full_path, 
        addr - data->start, sym, DRSYM_DEFAULT_FLAGS);

    if (symres == DRSYM_SUCCESS || symres == DRSYM_ERROR_LINE_NOT_AVAILABLE) {
        const char *modname = dr_module_preferred_name(data);
        if (modname == NULL)
            modname = "<noname>";
        //dr_fprintf(logF, "%s "PFX" %s, function name is: %s, "PIFX", line off "PFX" \n", prefix, addr,
                   //modname, sym->name, addr - data->start - sym->start_offs, sym->line_offs);


        std::string key(sym->name);

	outer_hash_entry* value;
        if (functionmap.find(key) == functionmap.end()) {
		value = &functionmap[key];
                // Attempting to access the file value if 
                // symres == DRSYM_ERROR_LINE_NOT_AVAILABLE appears to cause
                // a segmentation fault.

                if (symres == DRSYM_ERROR_LINE_NOT_AVAILABLE)
                  value->file = std::string("<unkown file>");
                else
                  value->file = std::string(sym->file);

                value->function_name = std::string(sym->name);
	}
        else
        {
		value = &functionmap[key];
        }

        inner_hash_entry* inVal;
        if (value->mapAddrs.find(addr) == value->mapAddrs.end())
        {
	  memory_used += 44;	
          inVal = new inner_hash_entry();
	  inVal->call_count = 0;
	  inVal->no_bits = 0;
	  inVal->line_number = sym->line;
    
          inVal->sum_of_bits = 0;
	  inVal->sum_of_loss = 0;
	  inVal->use_vector = true;
/*if(memory){
	memv_entry* m = new memv_entry();
	m->dv = 0;
	m->sv = 0;
	inVal->mem_map[mem_var] = m;
}
*/
inVal->fsum = 0;
inVal->dsum = 0;
	  inVal->memory = memory;
          value->mapAddrs[addr] = inVal;
          dr_printf("Instrumenting %s, function %s, line %d, pc %d\n", value->file.c_str(), 
            value->function_name.c_str(), inVal->line_number, addr);
	}
        else
        {
          inVal = value->mapAddrs[addr];
        }

	inVal->addr = addr;        
        return inVal;
    }
    else
    {
      printf("Failed to locate symbol\n");
      return NULL;
    }

    dr_free_module_data(data);
}

static void
callback(reg_id_t reg, int displacement, reg_id_t destReg, int opcode, inner_hash_entry *entry, reg_id_t mr, int dd, reg_id_t tmr, int tdd, float * rel_addr){

	int r, s;
//	printf("In callback\n");
   	int regId = getMMRegisterID(destReg);
   	dr_mcontext_t mcontext;
   	memset(&mcontext, 0, sizeof(dr_mcontext_t));
   	mcontext.flags = DR_MC_ALL;
   	mcontext.size = sizeof(dr_mcontext_t);
   	bool result = dr_get_mcontext(dr_get_current_drcontext(), &mcontext);

	reg_t mem_reg = reg_get_value(reg, &mcontext);
	float * mem_var;
	if(mr == NULL && rel_addr != NULL){
		mem_var = rel_addr;
	}
	else{
	

	        reg_t mem_reg1 = reg_get_value(mr, &mcontext);
        	mem_var = (float*)(mem_reg1 + dd);
//		printf("INSIDE callback memory is %x\n ", mem_var);

//		printf("before "PIFX": %s %d %x\n", entry->addr, get_register_name(mr), dd, mem_var);
//		dr_fprintf(logF,"before "PIFX": %s %d %x\n", entry->addr, get_register_name(mr), dd, mem_var);
	//	if(!entry->memory)
		 reg_t tmem_reg1 = reg_get_value(tmr, &mcontext);
        	float * tmem_var = (float*)(tmem_reg1 + tdd);
//		printf("INSIDE callback memory is %x\n ", mem_var);

//		printf("before "PIFX": %s %d %x\n", entry->addr, get_register_name(mr), dd, mem_var);
//		printf("test %s %d %x\n",  get_register_name(tmr), tdd, tmem_var);
//		dr_fprintf(logF,"before "PIFX": %s %d %x\n", entry->addr, get_register_name(mr), dd, mem_var);
		 if(tmem_var == mem_var && mem_reg1 != NULL){
			entry->memory = true;
//			printf("EQUAL %s %d %x\n",  get_register_name(tmr), tdd, tmem_var);
		}

	}
	int bits = 0;
	double loss = 0;
	double lossD = 0;
   		float op1, op2;
	if(is_single_precision_instr(opcode)){
   		op2 = *(float*)(mem_reg + displacement);
//		for(r=0; r<16; ++r)
//			for(s=0; s<4; ++s)
//		     		printf("reg %i.%i: %f\n", r, s, 
//					*((float*) &mcontext.ymm[r].u32[s]));
		op1 = *((float*) &mcontext.ymm[regId].u32[0]);


		int exp1, exp2;
		float mant1, mant2;
		mant1 = frexpf(op1, &exp1);
		mant2 = frexpf(op2, &exp2);
		bits = abs(exp1-exp2);
		if(bits > 24){
			bits = 24;
		}

//source can be mem or reg; dest is reg
		double dop1 = op1;
		double dop2 = op2;
		if(opcode == OP_addss){
			double dadd = dop1 + dop2;
			float fadd = op1 + op2;
			lossD = dadd - fadd;

			//to initialize to the initial sum at the beginning
//check whether need it for memory?????????????
			if(entry->call_count == 0){
				entry->dsum = op1;
				entry->fsum = op1;
//				printf("op1 is %f and op2 is %f\n", op1,op2);
			}
			if(entry->memory){

			        if (entry->mem_map.find(mem_var) == entry->mem_map.end()) {
//		printf("didnt findmemory  "PIFX"\n", mem_var);
					memv_entry * m = new memv_entry();
 					m->dv = dop1;
					m->sv = op1;
//					m->calls = 0;
					entry->mem_map[mem_var] = m;
////		dr_fprintf(logF,""PIFX": %s %d %x\n", entry->addr, get_register_name(mr), dd, mem_var);
//			printf("CREATNIG NEW MEM ENTRY 	%x\n",mem_var );
				}
				memv_entry * m = entry->mem_map[mem_var];
				m->dv +=dop2;
				m->sv +=op2;
//				m->calls++;

				entry->dsum += dop2;
				entry->fsum += op2;
//	printf("memory true 2\n");
			}

		//printf("double %.13lf float %.13f\n", dadd, fadd);
		}
		else{
			double dsub = dop1 - dop2;
			float fsub = op1 - op2;	
			lossD = dsub - fsub;
//op1 is subtrahend(b); a - b = c 
	 		//to initialize to the initial sum at the beginning
			if(entry->call_count == 0){
				entry->dsum = op1;
				entry->fsum = op1;
//				printf("op1 is %f and op2 is %f\n", op1,op2);
			}

			if(entry->memory){

			        if (entry->mem_map.find(mem_var) == entry->mem_map.end()) {
					memv_entry * m = new memv_entry();
 					m->dv = dop1;
					m->sv = op1;
//					m->calls = 0;
					entry->mem_map[mem_var] = m;
	
				}
				memv_entry * m = entry->mem_map[mem_var];
				m->dv -=dop2;
				m->sv -=op2;

//				m->calls++;
				entry->dsum -= dop2;
				entry->fsum -= op2;
			}


		}
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
	print_address(entry, bits, loss, lossD, op1, op2);
}


bool compare_memory_operands(opnd_t op1, opnd_t op2){
  if(opnd_is_rel_addr(op1) && opnd_is_rel_addr(op2)){
		     // float * aa = (float*)opnd_get_addr(op1);
		     // float * bb = (float*)opnd_get_addr(op2);
          	//	printf("RELATIVE ADDRESS CASE: "PIFX" "PIFX"\n", aa, bb);	
    return opnd_get_addr(op1) == opnd_get_addr(op2);
  }
  else if(opnd_is_memory_reference(op1) && opnd_is_memory_reference(op2) ){
    reg_id_t reg1 = opnd_get_reg_used(op1, 0);
    reg_id_t reg2 = opnd_get_reg_used(op2, 0);
    int disp1 = opnd_get_disp(op1);
    int disp2 = opnd_get_disp(op2);
    return ((reg1 == reg2) && (disp1 == disp2));
  }
  return false; 

}

static dr_emit_flags_t
bb_event(void* drcontext, void *tag, instrlist_t *bb, bool for_trace, bool translating)
{
//printf("BEGINNING OF BB_EVENT\n");
    instr_t *instr, *next_instr;
    int opcode;
    for (instr = instrlist_first(bb); instr != NULL; instr = next_instr) {
        next_instr = instr_get_next(instr);
        opcode = instr_get_opcode(instr);
//printf("each instruction\n");
	mem_map_entry* mem = NULL;
        app_pc pc_addr = instr_get_app_pc(instr);
	
        if(instr_is_floating(instr)){
   	//	dr_fprintf(logF, "Has seen FPU instruction with opcode %d\n",opcode);
	
	}
	else if(is_SIMD_packed(opcode)){
   	//	dr_fprintf(logF, "Has seen SIMD packed instruction with opcode %d\n",opcode);
	}

	else if(is_SIMD_arithm(opcode)){
		int is_single = 0;
//		printf("opcode is   %d\n", opcode);
//    		printf("number of sources  %d\n", instr_num_srcs(instr));  
 //   		printf("number of dests  %d\n", instr_num_dsts(instr));
		//assert(number of sources = 2);
		opnd_t source1 = instr_get_src(instr,0);
		opnd_t source2 = instr_get_src(instr,1);
		opnd_t dest = instr_get_dst(instr,0);
float * addr = NULL;
	opnd_t mem_addr;
	opnd_t checkaddr;
	bool found_mem = false;


		reg_id_t reg_src, reg_dst;
		reg_dst = opnd_get_reg(source2);
		if(opnd_is_reg(source1))
			reg_src = opnd_get_reg(source1);
		else
			reg_src = NULL;
// trying to find movss(store) instruction where source reg is the same as dest reg in addss
	        instr_t* next = instr_get_next(instr);
	        if(next == NULL){
	          printf("SOME null 3\n");	
		  //goto callbacks;
		}
		int next_opcode = instr_get_opcode(next);
//while source of the movss is not the same as destination of the addss 
		while(!(next_opcode == OP_movss && opnd_is_reg(instr_get_src(next,0)) && opnd_get_reg(instr_get_src(next, 0)) == reg_dst) ){
		  next = instr_get_next(next);
		  if(next == NULL)
	            break;
		  next_opcode = instr_get_opcode(next);
		}
		if(next == NULL || !opnd_is_memory_reference(instr_get_dst(next, 0))){
//		  printf("no store or stored in other register\n");//go to end	
		  //goto callbacks;
		}
		else{

		  //found mem_addr where register was copied to, 
		  //looking for previous load instruction which has the same mem_addr and reg
                  mem_addr = instr_get_dst(next, 0);

		  instr_t* prev = instr_get_prev(instr);//NUll case
		  if(prev == NULL){
			printf("SOME null 1\n");	
			//goto callbacks;
		  }
		  int prev_opcode = instr_get_opcode(prev);
		//if src of addss is memory and equal to the stored address
		  if(reg_src == NULL ){
			while(!found_mem){
			    if(prev_opcode == OP_movss && opnd_is_memory_reference(instr_get_src(prev, 0)) && 
				opnd_is_reg(instr_get_dst(prev, 0)) ){

				if(compare_memory_operands(instr_get_src(prev, 0), mem_addr)){

                          		if(opnd_get_reg(instr_get_dst(prev, 0)) == reg_dst){
						found_mem = true;
						break;	
					}
					else{ //copied to some other regiser
						break;
					}
					
				}
				else{
					if(opnd_get_reg(instr_get_dst(prev, 0)) == reg_dst){

						checkaddr = instr_get_src(prev,0);
						break; //mem is diff, but copied to the register 
					}
					else{ //diff mem and reg, not our case
						prev = instr_get_prev(prev);
		          			if(prev == NULL)
			    				break;
		          			prev_opcode = instr_get_opcode(prev);
					}
				}
			    }
			    else{
			    	prev = instr_get_prev(prev);
		            	if(prev == NULL)
			    		break;
		            	prev_opcode = instr_get_opcode(prev);
			    }


			}

		//	if(compare_memory_operands(mem_addr, source1)){
		//		found_mem = true;
		//	}
		//	else{
			//addss operand is memory, but the mov was to another memory address			
			  //goto callbacks;
		//	}


		  }
		  else{//reg_src is register


		  while(!found_mem && !(reg_src == NULL)){
		    if(prev_opcode == OP_movss && opnd_is_memory_reference(instr_get_src(prev, 0)) && 
			opnd_is_reg(instr_get_dst(prev, 0)) ){

//           compare_memory_operands(instr_get_src(prev, 0), mem_addr)){
			if(compare_memory_operands(instr_get_src(prev, 0), mem_addr)){
//if that movss copies to the dest reg of addss	and has the same addr	      
                          if(opnd_is_reg(instr_get_dst(prev, 0)) && (opnd_get_reg(instr_get_dst(prev, 0)) == reg_dst || opnd_get_reg(instr_get_dst(prev, 0)) == reg_src)){
		        	  found_mem = true;
//and if memories are the same

/*printf("222222 FOUND MEMORY IS TRUE\n\n");

printf("reg1 %s   reg2 %s\n", get_register_name(reg_src), get_register_name(reg_dst));
			dr_print_instr(drcontext, logF, prev, "INSTR: ");
			dr_print_instr(drcontext, logF, instr, "ADDSS: ");
			dr_print_instr(drcontext, logF, next, "INSTR: ");
*/

/*		  if(opnd_is_rel_addr(mem_addr) && opnd_is_rel_addr(instr_get_src(prev, 0))){
    		printf("\n\n"PIFX"   "PIFX"\n\n", opnd_get_addr(mem_addr) ,opnd_get_addr(instr_get_src(prev, 0)));
  }
  else if(opnd_is_memory_reference(mem_addr) && opnd_is_memory_reference(instr_get_src(prev, 0)) ){
    reg_id_t reg1 = opnd_get_reg_used(mem_addr, 0);
    reg_id_t reg2 = opnd_get_reg_used(instr_get_src(prev, 0), 0);
    int disp1 = opnd_get_disp(mem_addr);
    int disp2 = opnd_get_disp(instr_get_src(prev, 0));
    printf("\n\nreg1 %s reg2 %s, d1 %d d2 %d\n\n", get_register_name(reg1), get_register_name(reg2),disp1,disp2);
  }
*/


			  }
			  else{
		//if address is the same but was copied to another register
				found_mem = false;
				break;
			  }

	              }
		      else{
		//memory is different
                        if( opnd_get_reg(instr_get_dst(prev, 0)) == reg_dst){ 
 				prev = instr_get_prev(prev);
		          	     if(prev == NULL)
			    		break;
		          	prev_opcode = instr_get_opcode(prev);
				
				//look whether other register was copied from that memory
				while(!found_mem){
				    if(prev_opcode == OP_movss && opnd_is_memory_reference(instr_get_src(prev, 0)) && 
						opnd_is_reg(instr_get_dst(prev, 0)) ){

					if(compare_memory_operands(instr_get_src(prev, 0), mem_addr)){
						if(opnd_get_reg(instr_get_dst(prev, 0)) == reg_src){
							found_mem = true;
						}
						else{
							break;
						}
					}
					else{
						if(opnd_get_reg(instr_get_dst(prev, 0)) == reg_src){
							break;
						}
					}
				     }	
			       	     prev = instr_get_prev(prev);
		          	     if(prev == NULL)
			    		break;
		          	     prev_opcode = instr_get_opcode(prev);
				

				}
				break;
			}
			else if(opnd_get_reg(instr_get_dst(prev, 0)) == reg_src){

 				prev = instr_get_prev(prev);
		          	     if(prev == NULL)
			    		break;
		          	prev_opcode = instr_get_opcode(prev);
				
				//look whether other register was copied from that memory
				while(!found_mem){
				    if(prev_opcode == OP_movss && opnd_is_memory_reference(instr_get_src(prev, 0)) && 
						opnd_is_reg(instr_get_dst(prev, 0)) ){

					if(compare_memory_operands(instr_get_src(prev, 0), mem_addr)){
						if(opnd_get_reg(instr_get_dst(prev, 0)) == reg_dst){
							found_mem = true;
						}
						else{
							break;
						}
					}
					else{
						if(opnd_get_reg(instr_get_dst(prev, 0)) == reg_dst){
							break;// both registeres copied from other mem
						}
					}
				     }	
			       	     prev = instr_get_prev(prev);
		          	     if(prev == NULL)
			    		break;
		          	     prev_opcode = instr_get_opcode(prev);
				

				}
				break;

			}

			else{// other movss, continue 
		          prev = instr_get_prev(prev);
		          if(prev == NULL)
			    break;
		          prev_opcode = instr_get_opcode(prev);
			}
		      }

		    }
		    else{ 
		      prev = instr_get_prev(prev);
		      if(prev == NULL)
			break;
		      prev_opcode = instr_get_opcode(prev);
		    }
		  }

		}//else of reg_src is not a memory reference
		  if(prev == NULL || !found_mem){
//			printf("prev is null or mem moved into other register\n");//go to end
			//goto callbacks;
		  }
		  else{
/*
		//found both load and store 
		    
		    if(opnd_is_rel_addr(mem_addr)){
		      addr = (float*)opnd_get_addr(mem_addr);
          		printf("RELATIVE ADDRESS CASE: "PIFX"\n", addr);	
//			     if(addr == 0)
				printf("0 in relative addr\n\n\n");
		    }
		    else{

   	              dr_mcontext_t mcontext; // = *(dr_mcontext_t*) drcontext;
   		      memset(&mcontext, 0, sizeof(dr_mcontext_t));
   		      mcontext.flags = DR_MC_ALL;
   		      mcontext.size = sizeof(dr_mcontext_t);
     		      bool result = dr_get_mcontext(dr_get_current_drcontext(), &mcontext);
		      reg_id_t reg = opnd_get_reg_used(mem_addr, 0);
		      reg_t mem_reg = reg_get_value(reg, &mcontext);
	              int displacement = opnd_get_disp(mem_addr);
   		      addr = (float*)(mem_reg + displacement);

		    }
*/
		  }//else found mem

	        }
			float* rel_addr = NULL;
			if(opnd_is_rel_addr(mem_addr)){
		      		rel_addr= (float*)opnd_get_addr(mem_addr);
			}

                        if(opnd_is_memory_reference(source1)){

			
	//		dr_print_instr(drcontext, logF, instr, "INSTR: ");
//			dr_print_opnd(drcontext, logF, source1, "OPND1: ");
//			dr_print_opnd(drcontext, logF, source2, "OPND2: ");
			reg_id_t rd = opnd_get_reg(source2);
			reg_id_t rs = opnd_get_reg_used(source1, 0);
                        inner_hash_entry *entry = get_inner_hash_entry(instr_get_app_pc(instr), found_mem);
//			printf("Before callback in bbevent %s %d %x\n", get_register_name(opnd_get_reg_used(mem_addr,0)),opnd_get_disp(mem_addr),entry->addr );
			dr_insert_clean_call(drcontext, bb, instr, 
				(void*) callback, true, 10, 
				OPND_CREATE_INTPTR(rs), OPND_CREATE_INTPTR(opnd_get_disp(source1)),
				OPND_CREATE_INTPTR(rd), OPND_CREATE_INTPTR(opcode), OPND_CREATE_INTPTR(entry)
				,OPND_CREATE_INTPTR(opnd_get_reg_used(mem_addr,0)), OPND_CREATE_INTPTR(opnd_get_disp(mem_addr))
				,OPND_CREATE_INTPTR(opnd_get_reg_used(checkaddr,0)), OPND_CREATE_INTPTR(opnd_get_disp(checkaddr))
				,OPND_CREATE_INTPTR(rel_addr)
			);

		}
		else if(opnd_is_reg(source1) && opnd_is_reg(source2)){ 
			reg_id_t reg1 = opnd_get_reg(source1); //source
			reg_id_t reg2 = opnd_get_reg(source2); //dest
                        inner_hash_entry *entry = get_inner_hash_entry(instr_get_app_pc(instr), found_mem);
//			printf("Before getregreg in bbevent %s %d %x\n", get_register_name(opnd_get_reg_used(mem_addr,0)),opnd_get_disp(mem_addr),entry->addr );
			reg_id_t mr = opnd_get_reg_used(mem_addr, 0);
		    	dr_insert_clean_call(drcontext,bb,instr, (void*)getRegReg, 
				true, 7, 
				OPND_CREATE_INTPTR(reg1), OPND_CREATE_INTPTR(reg2)
				,OPND_CREATE_INTPTR(opcode), OPND_CREATE_INTPTR(entry)
				,OPND_CREATE_INTPTR(mr), OPND_CREATE_INTPTR(opnd_get_disp(mem_addr))
				,OPND_CREATE_INTPTR(rel_addr)
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

