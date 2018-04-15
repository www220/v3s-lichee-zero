#include <rthw.h>
#include <rtthread.h>
#include "armv7.h"
/**
 * This function will initialize thread stack
 *
 * @param tentry the entry of thread
 * @param parameter the parameter of entry
 * @param stack_addr the beginning stack address
 * @param texit the function will be called when thread exit
 *
 * @return stack address
 */
rt_uint8_t *rt_hw_stack_init(void *tentry, void *parameter,
	rt_uint8_t *stack_addr, void *texit)
{
	rt_uint32_t *stk;

	stk 	 = (rt_uint32_t*)stack_addr;
	*(stk) 	 = (rt_uint32_t)tentry;			/* entry point */
	*(--stk) = (rt_uint32_t)texit;			/* lr */
	*(--stk) = 0;							/* r12 */
	*(--stk) = 0;							/* r11 */
	*(--stk) = 0;							/* r10 */
	*(--stk) = 0;							/* r9 */
	*(--stk) = 0;							/* r8 */
	*(--stk) = 0;							/* r7 */
	*(--stk) = 0;							/* r6 */
	*(--stk) = 0;							/* r5 */
	*(--stk) = 0;							/* r4 */
	*(--stk) = 0;							/* r3 */
	*(--stk) = 0;							/* r2 */
	*(--stk) = 0;							/* r1 */
	*(--stk) = (rt_uint32_t)parameter;		/* r0 : argument */

	/* cpsr */
	if ((rt_uint32_t)tentry & 0x01)
		*(--stk) = SVCMODE | 0x20;			/* thumb mode */
	else
		*(--stk) = SVCMODE;					/* arm mode   */

	/* return task's current stack address */
	return (rt_uint8_t *)stk;
}



/**
 * This function finds the first bit set (beginning with the least significant bit)
 * in value and return the index of that bit.
 *
 * Bits are numbered starting at 1 (the least significant bit).  A return value of
 * zero from any of these functions means that the argument was zero.
 *
 * @return return the index of the first bit set. If value is 0, then this function
 * shall return 0.
 */
// int __rt_ffs(int value)
// {
    // return __builtin_ffs(value);
// }

