#include <hafos/gdt.h>

#include <stdint.h>
#include <stddef.h>

struct gdt_pointer {
    uint16_t limit;
    uintptr_t base;
} __attribute__((packed));

struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed));

static struct gdt_pointer gdt_ptr;
static struct gdt_entry gdt_entries[5];

extern void gdt_flush(uintptr_t);

static void gdt_set_gate(size_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
   gdt_entries[num].base_low = (base & 0xFFFF);
   gdt_entries[num].base_middle = (base >> 16) & 0xFF;
   gdt_entries[num].base_high = (base >> 24) & 0xFF;

   gdt_entries[num].limit_low = (limit & 0xFFFF);
   gdt_entries[num].granularity = (limit >> 16) & 0x0F;

   gdt_entries[num].granularity |= gran & 0xF0;
   gdt_entries[num].access = access;
}

void gdt_install(void) {
    gdt_ptr.limit = sizeof(gdt_entries) - 1;
    gdt_ptr.base = (uintptr_t) &gdt_entries;

    gdt_set_gate(0, 0, 0, 0, 0);
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); //Code
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); //Data
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); //User code
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); //User data

    gdt_flush((uintptr_t) &gdt_ptr);
}
