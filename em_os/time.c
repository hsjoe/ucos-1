

#define TIME_GLOBAL
#include "emos.h"


/*
 * 目的:
 */
void time_dly(int ticks)
{
#if CRITICAL_METHOD == 3
	OS_CPU_SR cpu_sr = sigset_mask;
#endif

	if (ticks > 0)
	{
		ENTER_CRITICAL();

		tcb_exit_rdy(cur_tcb->prio);
		cur_tcb->delay = ticks;

		//list_add(&cur_tcb->list, tcb_dly_head);

		debug("prio: %d, ticks: %d, rdy_grp: %d, rdy_tbl[0]: %d\n", cur_tcb->prio, ticks, rdy_grp, rdy_tbl[0]);

		EXIT_CRITICAL();

		schedule();
	}
}



/*
 * 取消 延时
 */
u8 time_dly_resume(u8 prio)
{
#if CRITICAL_METHOD == 3
	OS_CPU_SR cpu_sr = sigset_mask;
#endif

	struct tcb *ptcb;
	u8 err = NO_ERR;

	err = general_check_prio(prio);
	if (NO_ERR != err)
	{
		goto fail;
	}

	ENTER_CRITICAL();

	ptcb = tcb_prio_tbl[prio];

	// check
	
	ptcb->delay = 0;

	EXIT_CRITICAL();

	schedule();

fail:
	return err;
}


/*
 * ticks
 */
void time_set(u32 ticks)
{
#if CRITICAL_METHOD == 3
	OS_CPU_SR cpu_sr = sigset_mask;
#endif

	ENTER_CRITICAL();

	jiffies = ticks;

	EXIT_CRITICAL();
}


u32 time_get(void)
{
#if CRITICAL_METHOD == 3
	OS_CPU_SR cpu_sr = sigset_mask;
#endif

	u32 ticks;

	ENTER_CRITICAL();

	ticks = jiffies;

	EXIT_CRITICAL();

	return (ticks);
}
