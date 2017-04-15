/* vga_console.c - contains the implementation of
 * of all vga_console driver functions.
 * TODO:
 * - '\n' is treated as '\r'
 * - Test scroll()
 * - Test VGA_display_char() cases
 */
#include "vga_console.h"
#include "../lib/so_string.h"
#define VGA_BASE 0xb8000
#define WIDTH 80  /* Pixels? */
#define HEIGHT 25
#define FONT 10 << 8

/* Screen Size Specs */
static unsigned short *vgaBuff = (unsigned short*)VGA_BASE;
const unsigned short SCREEN_MAX = WIDTH * HEIGHT;

/* Cursor Position in vgaBuff */
static int cursor = 0;

/* Prototypes */
void VGA_clear(void);
void VGA_display_char(char c);
void VGA_display_str(const char * str);
void scroll();

/* Clears entire screen */
void VGA_clear(void) {
   memset((void*)VGA_BASE, 0, SCREEN_MAX * 2);
   cursor = 0;
}

/* Outputs a char to cursor position */
void VGA_display_char(char c) {
   unsigned short char_attrbs = FONT;

   if (c == '\n') {
      cursor += WIDTH - (cursor % WIDTH);
      if (cursor >= SCREEN_MAX) {
         scroll();
         cursor -= WIDTH;
      }
   }
   else {
      if (cursor == SCREEN_MAX) {
         scroll();
         cursor = SCREEN_MAX - WIDTH;
      }

      vgaBuff[cursor] = char_attrbs | c;
      cursor++;
   }
}

/* Outputs a string starting from cursor position */
void VGA_display_str(const char *str) {
   while ('\0' != *str) {
      VGA_display_char(*str);
      str++;
   }
}

/* scoots everything in the window up by width (or to the left) */
void scroll() {

   /* Shift everything up by one row */
   for (unsigned short i = WIDTH; i < SCREEN_MAX; i++)
      vgaBuff[i - WIDTH] = vgaBuff[i];

   /* memset last (2*WIDTH) of SCREEN_MAX */
   memset((void*)(VGA_BASE + (SCREEN_MAX * 2) - (WIDTH * 2)), 0, (WIDTH * 2));
}