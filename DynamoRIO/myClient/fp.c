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
static int fp_count = 0;
static void *count_mutex; /* for multithread support */
static client_id_t client_id;
DR_EXPORT void
dr_init(client_id_t id)
{
    dr_register_exit_event(exit_event);
    dr_register_bb_event(bb_event);
    count_mutex = dr_mutex_create();
    client_id = id;
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
}
/*
static void
callback(app_pc addr, uint divisor)
{
  //   * instead of a lock could use atomic operations to
  //   * increment the counters 
    dr_mutex_lock(count_mutex);

    div_count++;

  //   check for power of 2 
    if ((divisor & (divisor - 1)) != 0)
        div_p2_count++;

    dr_mutex_unlock(count_mutex);
}*/

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
        	dr_fprintf(STDERR, "<floating point instruction operands for thread %d in %s>\n",
                   dr_get_thread_id(drcontext), logname);
    	}
	#endif	


}

static void 
getRegReg(reg_id_t r1, reg_id_t r2, uint opcode){
	
	char * r1Name = get_register_name(r1);
	char * r2Name = get_register_name(r2);
	int s1        = atoi(r1Name + 3 * sizeof(char));
	int s2        = atoi(r2Name + 3 * sizeof(char));

	dr_mcontext_t mcontext;
   	memset(&mcontext, 0, sizeof(dr_mcontext_t));
   	mcontext.flags = DR_MC_MULTIMEDIA;
   	mcontext.size = sizeof(dr_mcontext_t);
   	bool result = dr_get_mcontext(dr_get_current_drcontext(), &mcontext);
	int r, s;
	float op1, op2;
	if(opcode == OP_addss){
		for(r=0; r<16; ++r)
			for(s=0; s<4; ++s)
		     		printf("reg %i.%i: %f\n", r, s, 
					*((float*) &mcontext.ymm[r].u32[s]));
		op1 = *((float*) &mcontext.ymm[s1].u32[0]);
		op2 = *((float*) &mcontext.ymm[s2].u32[0]);
	}
	else{
		for(r=0; r<16; ++r)
    			for(s=0; s<2; ++s)
	     			printf("reg %i.%i: %f\n", r, s, 
					*((double*) &mcontext.ymm[r].u64[s]));
		op1 = *((double*) &mcontext.ymm[s1].u64[0]);
		op2 = *((double*) &mcontext.ymm[s2].u64[0]);
	}
       	dr_fprintf(logF, "%d: %f  %f\n",opcode, op1, op2);
}

static void
fpRegs(){
  	dr_mcontext_t mcontext;
   	memset(&mcontext, 0, sizeof(dr_mcontext_t));
   	mcontext.flags = DR_MC_ALL;
   	mcontext.size = sizeof(dr_mcontext_t);
   	bool result = dr_get_mcontext(dr_get_current_drcontext(), &mcontext);
//	printf("displacement is %d\n", displacement);
   	printf("RBP contents: %f\n", *(float*)(mcontext.rbp - 48));
//   	op2 = *(float*)(mcontext.rbp -32);
	

}

static void
callback(reg_id_t reg, int displacement, reg_id_t destReg, uint opcode){
	int r, s;
   	float op1, op2;
   	char * destRegName = get_register_name(destReg);
   	int regId = atoi(destRegName + 3 * sizeof(char));


   	dr_mcontext_t mcontext;
   	memset(&mcontext, 0, sizeof(dr_mcontext_t));
   	mcontext.flags = DR_MC_ALL;
   	mcontext.size = sizeof(dr_mcontext_t);
   	bool result = dr_get_mcontext(dr_get_current_drcontext(), &mcontext);
	printf("displacement is %d\n", displacement);
   	printf("RBP contents: %f\n", *(float*)(mcontext.rbp + displacement));
   	op2 = *(float*)(mcontext.rbp + displacement);
	if(opcode == OP_addss){
		for(r=0; r<16; ++r)
			for(s=0; s<4; ++s)
		     		printf("reg %i.%i: %f\n", r, s, 
					*((float*) &mcontext.ymm[r].u32[s]));
		op1 = *((float*) &mcontext.ymm[regId].u32[0]);
	}
	else{
		for(r=0; r<16; ++r)
    			for(s=0; s<2; ++s)
	     			printf("reg %i.%i: %f\n", r, s, 
					*((double*) &mcontext.ymm[r].u64[s]));
		op1 = *((double*) &mcontext.ymm[regId].u64[0]);
	}

   	dr_fprintf(logF, "%d: %f  %f\n",opcode, op1, op2);
}



static dr_emit_flags_t
bb_event(void* drcontext, void *tag, instrlist_t *bb, bool for_trace, bool translating)
{
    instr_t *instr, *next_instr;
    int opcode;

    for (instr = instrlist_first(bb); instr != NULL; instr = next_instr) {
        next_instr = instr_get_next(instr);
        opcode = instr_get_opcode(instr);
//	if(instr_is_floating(instr)){
//	    	printf("Floating point instruction \n");  
//	}

//padded? addpd? addps? AVX? FPU instruction fadd??? 
//rcpps?


	if(opcode == OP_faddp){
		
			dr_insert_clean_call(drcontext, bb, instr, 
				(void*) fpRegs, true, 0); 
	}

	if (opcode == OP_addss || opcode == OP_addsd || opcode == OP_mulss || opcode == OP_mulsd
	    opcode == OP_subss || opcode == OP_subsd || opcode == OP_divss || opcode == OP_divsd	
	    opcode == OP_sqrtss || opcode == OP_sqrtsd || opcode == OP_rsqrtss || opcode == OP_rsqrtsd) { 
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
	    		writeLog(drcontext);
			reg_id_t rd = opnd_get_reg(source2);
			reg_id_t rs = opnd_get_reg_used(source1, 0);
			dr_insert_clean_call(drcontext, bb, instr, 
				(void*) callback, true, 4, 
				OPND_CREATE_INTPTR(rs), OPND_CREATE_INTPTR(opnd_get_disp(source1)),
				OPND_CREATE_INTPTR(rd), OPND_CREATE_INTPTR(opcode));
		}
		else if(opnd_is_reg(source1) && opnd_is_reg(source2)){
			reg_id_t reg1 = opnd_get_reg(source1);
			reg_id_t reg2 = opnd_get_reg(source2);
			printf("register1 is %s\n", get_register_name(reg1));
			printf("register2 is %s\n", get_register_name(reg2));

			dr_insert_clean_call(drcontext,bb,instr, (void*)getRegReg, 
				true, 3, 
				OPND_CREATE_INTPTR(reg1), OPND_CREATE_INTPTR(reg2)
				,OPND_CREATE_INTPTR(opcode) 
			); 
		
			writeLog(drcontext);
		}
		else{
		//should not be the case, throw an exception
		}
	        fp_count++; 
	//  dr_insert_clean_call(drcontext, bb, instr, (void *)callback,
           //                      false /*no fp save*/, 2,
            //                     OPND_CREATE_INTPTR(instr_get_app_pc(instr)),
             //                    instr_get_src(instr, 0) /*divisor is 1st src*/);
        }
    }
    return DR_EMIT_DEFAULT;
}

