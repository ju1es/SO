#include "init_drivers.h"
#include "interrupts.h"
#include "keyboard.h"
#include "ps2.h"
#include "vga_console.h"
#include "../lib/so_stdio.h"

void init_drivers(void);

void init_drivers() {

/* DEBUGGING */
   CLI

   VGA_clear();
   printk("Initializing.. ");

/*
   init IDT
   init PIC
 */
   ps2_init();
   keyboard_init();
   init_interrupt_environment();

   STI

   printk("\n\n");
}
