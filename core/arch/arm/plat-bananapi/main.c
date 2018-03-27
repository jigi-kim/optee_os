// SPDX-License-Identifier: BSD-2-Clause

#include <console.h>
#include <drivers/gic.h>
#include <drivers/serial8250_uart.h>
#include <kernel/generic_boot.h>
#include <kernel/panic.h>
#include <kernel/pm_stubs.h>
#include <platform_config.h>
#include <tee/entry_fast.h>
#include <tee/entry_std.h>

static void main_fiq(void);

static const struct thread_handlers handlers = {
	.std_smc = tee_entry_std,
	.fast_smc = tee_entry_fast,
	.nintr = main_fiq,
	.cpu_on = pm_panic,
	.cpu_off = pm_panic,
	.cpu_suspend = pm_panic,
	.cpu_resume = pm_panic,
	.system_off = pm_panic,
	.system_reset = pm_panic,
};

static struct gic_data gic_data;
static struct serial8250_uart_data console_data;

register_phys_mem(MEM_AREA_IO_SEC, GIC_BASE, GIC_REG_SIZE);
register_phys_mem(MEM_AREA_IO_SEC, CONSOLE_UART_BASE, UART_REG_SIZE);

register_dynamic_shm(DRAM0_BASE_NSEC, DRAM0_SIZE_NSEC);

const struct thread_handlers *generic_boot_get_handlers(void)
{
	return &handlers;
}

static void main_fiq(void)
{
	panic();
}

void console_init(void)
{
	serial8250_uart_init(&console_data, CONSOLE_UART_BASE, 
	                     CONSOLE_UART_CLK_IN_HZ, CONSOLE_BAUDRATE);
	register_serial_console(&console_data.chip);
}

void main_init_gic(void)
{
	vaddr_t gicc_base;
	vaddr_t gicd_base;

	gicc_base = core_mmu_get_va(GIC_BASE + GICC_OFFSET, MEM_AREA_IO_SEC);
	gicd_base = core_mmu_get_va(GIC_BASE + GICD_OFFSET, MEM_AREA_IO_SEC);

	if (!gicc_base || !gicd_base)
		panic();

	/* Initialize GIC */
	gic_init(&gic_data, gicc_base, gicd_base);
	itr_init(&gic_data.chip);
}
