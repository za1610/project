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

/* Counts the number of dynamic div instruction for which the
 * divisor is a power of 2 (these are cases where div could be
 * strength reduced to a simple shift).  Demonstrates callout
 * based profiling with live operand values. */

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

static int div_count = 0, div_p2_count = 0;
static void *count_mutex; /* for multithread support */

DR_EXPORT void
dr_init(client_id_t id)
{
    dr_register_exit_event(exit_event);
    dr_register_bb_event(bb_event);
    count_mutex = dr_mutex_create();
	printf("Client path is  %s !!!\n", dr_get_client_path(id));
}

static void
exit_event(void)
{
#ifdef SHOW_RESULTS
    char msg[512];
    int len;
    len = dr_snprintf(msg, sizeof(msg)/sizeof(msg[0]),
                      "Instrumentation results:\n"
                      "  saw %d div instructions\n"
                      "  of which %d were powers of 2\n",
                      div_count, div_p2_count);
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
static void callback(instr_t* instr, int opcode){
printf("IN callback \n");
        if (opcode == OP_addss) { 
		printf("opcode is addss  %d\n", opcode);
	    	printf("number of sources  %d\n", instr_num_srcs(instr));  
	    	printf("number of dests  %d\n", instr_num_dsts(instr));
if(instr_reads_memory(instr)){
	    	printf("some of the  sources is memory \n");  
} 
if(opnd_is_memory_reference(instr_get_src(instr,0))){
	    	printf("source is opnd  memory and num of regs is %d\n", opnd_num_regs_used(instr_get_src(instr, 0))); 
reg_id_t r = opnd_get_reg_used(instr_get_src(instr, 0), 0);
printf("register used is %s\n", get_register_name(r));
}
 
	    	printf("source  %d\n", instr_get_src(instr,0));  
		printf("dest  %d\n", instr_get_dst(instr,0));  
}
}

static void
callback1(app_pc addr, instr_t* instr){
int op = 5;
printf("in callback %d\n", op);
}

static dr_emit_flags_t
bb_event(void* drcontext, void *tag, instrlist_t *bb, bool for_trace, bool translating)
{
    instr_t *instr, *next_instr;
    int opcode;
printf("here\n");
    for (instr = instrlist_first(bb); instr != NULL; instr = next_instr) {
        next_instr = instr_get_next(instr);
        opcode = instr_get_opcode(instr);
	//if(opcode >= 300)
//		printf("opcode is  %d\n", opcode);
        /* if find div, insert a clean call to our instrumentation routine */
if(instr_is_floating(instr)){
	    	printf("Floating point instruction \n");  
}
if(instr_is_mmx(instr)){
	    	printf("MMX instruction \n");  
}
        if (opcode == OP_addss) { 
		printf("opcode is addss  %d\n", opcode);
	    	printf("number of sources  %d\n", instr_num_srcs(instr));  
	    	printf("number of dests  %d\n", instr_num_dsts(instr));
if(instr_reads_memory(instr)){
	    	printf("some of the  sources is memory \n");  
} 
if(opnd_is_memory_reference(instr_get_src(instr,0))){
	    	printf("source is opnd  memory and num of regs is %d\n", opnd_num_regs_used(instr_get_src(instr, 0))); 
if(opnd_is_memory_reference(instr_get_dst(instr,0)))
	    	printf("dest is opnd  memory and num of regs is %d\n", opnd_num_regs_used(instr_get_dst(instr, 0)));
 
reg_id_t r1 = opnd_get_reg_used(instr_get_dst(instr, 0), 0);
printf("register in dest is %s\n", get_register_name(r1));
dr_mcontext_t mc = {sizeof(mc),DR_MC_ALL};
printf("value in dest reg  is %d\n", reg_get_value(r1,&mc ));
reg_id_t r = opnd_get_reg_used(instr_get_src(instr, 0), 0);
printf("register used is %s\n", get_register_name(r));

printf("displacement in src is %d\n", opnd_get_disp(instr_get_src(instr,0)));
printf("value in src reg  is %d\n", reg_get_value(r,&mc ));
}
 
	    	printf("source  %d\n", instr_get_src(instr,0));  
		printf("dest  %d\n", instr_get_dst(instr,0));  
        div_count++; 
	//dr_insert_clean_call(drcontext, bb, instr, (void*) callback, false, 2, instr, opcode); 
//	dr_insert_clean_call(drcontext, bb, instr, (void*) callback1, false, 2, OPND_CREATE_INTPTR(instr_get_app_pc(instr)), instr); 
	//  dr_insert_clean_call(drcontext, bb, instr, (void *)callback,
           //                      false /*no fp save*/, 2,
            //                     OPND_CREATE_INTPTR(instr_get_app_pc(instr)),
             //                    instr_get_src(instr, 0) /*divisor is 1st src*/);
        }
    }
    return DR_EMIT_DEFAULT;
}

