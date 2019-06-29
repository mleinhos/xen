/*
 * arch/arm/vm_event.c
 *
 * Architecture-specific vm_event handling routines
 *
 * Copyright (c) 2016 Tamas K Lengyel (tamas.lengyel@zentific.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License v2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; If not, see <http://www.gnu.org/licenses/>.
 */

#include <xen/sched.h>
#include <asm/vm_event.h>

void vm_event_fill_regs(vm_event_request_t *req)
{
    const struct cpu_user_regs *regs = guest_cpu_user_regs();
    static unsigned long ttbr0_prev = 0;

    req->data.regs.arm.cpsr = regs->cpsr;
    req->data.regs.arm.pc = regs->pc;
    req->data.regs.arm.ttbcr = READ_SYSREG(TCR_EL1);
    req->data.regs.arm.ttbr0 = READ_SYSREG64(TTBR0_EL1);
    req->data.regs.arm.ttbr1 = READ_SYSREG64(TTBR1_EL1);

    if (1 || ttbr0_prev != req->data.regs.arm.ttbr0)
    {
	printk("DEBUG %s %d d=%d v=%d ttbr0=%lx\n",
               __func__, __LINE__, current->domain->domain_id, current->vcpu_id, req->data.regs.arm.ttbr0);
        ttbr0_prev = req->data.regs.arm.ttbr0;
    }
}

void vm_event_set_registers(struct vcpu *v, vm_event_response_t *rsp)
{
    struct cpu_user_regs *regs = &v->arch.cpu_info->guest_cpu_user_regs;

    /* vCPU should be paused */
    ASSERT(atomic_read(&v->vm_event_pause_count));

    printk("DEBUG %s %d d=%d v=%d ttbr0=%lx\n",
               __func__, __LINE__, current->domain->domain_id, current->vcpu_id, rsp->data.regs.arm.ttbr0);

    regs->pc = rsp->data.regs.arm.pc;
}

void vm_event_monitor_next_interrupt(struct vcpu *v)
{
    /* Not supported on ARM. */
}

/*
 * Local variables:
 * mode: C
 * c-file-style: "BSD"
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * End:
 */
