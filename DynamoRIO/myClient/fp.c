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

static int fp_count = 0;
static void *count_mutex; /* for multithread support */

DR_EXPORT void
dr_init(client_id_t id)
{
    dr_register_exit_event(exit_event);
    dr_register_bb_event(bb_event);
    count_mutex = dr_mutex_create();
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
static void 
regreg(reg_id_t reg1, reg_id_t reg2, app_pc instr){
	printf("in REGREG reg1 %s\n", get_register_name(reg1));
	printf("in REGREG reg2 %s\n", get_register_name(reg2));
}

static void
callback(){
  int b;
  int r;
  int s;

   dr_mcontext_t mcontext;
   memset(&mcontext, 0, sizeof(dr_mcontext_t));
   mcontext.flags = DR_MC_MULTIMEDIA;
   mcontext.size = sizeof(dr_mcontext_t);
   bool result = dr_get_mcontext(dr_get_current_drcontext(), &mcontext);
//   for(r=0; r<16; ++r)
//     for(s=0; s<4; ++s)
//     printf("reg %i.%i: %f\n", r, s, *((float*) &mcontext.ymm[r].u32[s]));

   for(r=0; r<16; ++r)
     for(s=0; s<2; ++s)
     printf("reg %i.%i: %f\n", r, s, *((double*) &mcontext.ymm[r].u64[s]));
	//printf("IN CALLBACK OP %f\n", op2);
//	reg_id_t r = opnd_get_reg(reg); 
//	printf("in callback reg %s\n", get_register_name(reg));
}

static void
regValue(opnd_t reg){
	reg_id_t r;// = opnd_get_reg(reg);
}

static dr_emit_flags_t
bb_event(void* drcontext, void *tag, instrlist_t *bb, bool for_trace, bool translating)
{
//printf("Begin\n");
    instr_t *instr, *next_instr;
    int opcode;
    for (instr = instrlist_first(bb); instr != NULL; instr = next_instr) {
        next_instr = instr_get_next(instr);
        opcode = instr_get_opcode(instr);
	if(instr_is_floating(instr)){
	    	printf("Floating point instruction \n");  
	}

	if (opcode == OP_fadd || opcode == OP_addss || opcode == OP_addsd) { 
		printf("opcode is   %d\n", opcode);
    		printf("number of sources  %d\n", instr_num_srcs(instr));  
    		printf("number of dests  %d\n", instr_num_dsts(instr));
		opnd_t source1 = instr_get_src(instr,0);
		opnd_t source2 = instr_get_src(instr,1);
		opnd_t dest = instr_get_dst(instr,0);
		if(opnd_is_memory_reference(source1)){
			if(opnd_is_reg(source2))
				printf("2 is reg\n");
			reg_id_t rr = opnd_get_reg(source2);
			printf("reg used %s\n", get_register_name(rr));
			dr_insert_clean_call(drcontext, bb, instr, 
				(void*) callback, true, 0); 
			//	source1,source2,  
				//opnd_create_reg(rr),
			//	OPND_CREATE_INTPTR(instr_get_app_pc(instr))); 

		}
		if(opnd_is_reg(source1) && opnd_is_reg(source2)){
			printf("opnd is register\n");
			reg_id_t reg1 = opnd_get_reg(source1);
			reg_id_t reg2 = opnd_get_reg(source2);
			printf("register1 is %s\n", get_register_name(reg1));
			printf("register2 is %s\n", get_register_name(reg2));

//   for(r=0; r<16; ++r)
 //    for(s=0; s<4; ++s)
  //   printf("reg %i.%i: %f\n", r, s, *((float*) &drcontext.ymm[r].u32[s]));
/*			dr_insert_clean_call(drcontext, bb, instr,
			(void*) callback, false, 2, 
			source1, OPND_CREATE_INTPTR(instr_get_app_pc(instr))
//			(void*) regValue, false, 1, 
//			opnd_create_reg(reg1)
			);*/
			dr_insert_clean_call(drcontext,bb,instr, (void*)callback, 
				true, 0, 
			OPND_CREATE_INT32(reg1), OPND_CREATE_INTPTR(reg2), 
			OPND_CREATE_INT32(instr_get_app_pc(instr))); 
		
		}
/*
	reg_id_t r1 = opnd_get_reg_used(instr_get_dst(instr, 0), 0);
	printf("register in dest is %s\n", get_register_name(r1));
	dr_mcontext_t mc = {sizeof(mc),DR_MC_ALL};
	printf("value in dest reg  is %d\n", reg_get_value(r1,&mc));
	printf("value in src reg  is %d\n", reg_get_value(r,&mc ));
	opnd_t m = OPND_CREATE_MEMPTR(r, disp);
	printf("uint %llu\n", m);
	printf("instr before %d\n", instr);
	printf("source before  %d\n", instr_get_src(instr,0));  
	printf("dest before %d\n", instr_get_dst(instr,0));  
*/

        fp_count++; 
	//  dr_insert_clean_call(drcontext, bb, instr, (void *)callback,
           //                      false /*no fp save*/, 2,
            //                     OPND_CREATE_INTPTR(instr_get_app_pc(instr)),
             //                    instr_get_src(instr, 0) /*divisor is 1st src*/);
        }
    }
    return DR_EMIT_DEFAULT;
}

