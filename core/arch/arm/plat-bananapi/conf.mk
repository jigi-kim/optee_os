PLATFORM_FLAVOR ?= bananapi

include core/arch/arm/cpu/cortex-a7.mk

$(call force,CFG_8250_UART,y)
$(call force,CFG_ARM32_core,y)
$(call force,CFG_GENERIC_BOOT,y)
$(call force,CFG_GIC,y)
$(call force,CFG_PM_STUBS,y)
$(call force,CFG_PSCI_ARM32,y)
$(call force,CFG_SECURE_TIME_SOURCE_CNTPCT,y)

ta-targets = ta_arm32

CFG_TEE_CORE_EMBED_INTERNAL_TESTS ?= y
CFG_WITH_STACK_CANARIES ?= y
CFG_WITH_STATS ?= y

CFG_NUM_THREADS ?= 1

ifeq ($(CFG_NS_HYPERVISOR),y)
    CFG_NS_ENTRY_ADDR ?= 0xb0000000
else
    CFG_NS_ENTRY_ADDR ?= 0x48000000
endif
