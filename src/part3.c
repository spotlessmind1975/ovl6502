/****************************************************************************
 * ovl - Overlay Example on unexpanded 6502 retrocomputers                  *
 *                                                                          *
 * Copyright (c) 2020 by Marco Spedaletti. Licensed under CC-BY-NC-SA       *
 *--------------------------------------------------------------------------*
 * OVERLAYED MODULE                                                         *
 ****************************************************************************/

/****************************************************************************
 ** INCLUDE SECTION
 ****************************************************************************/

#include <stdio.h>
#include <cc65.h>

#include "main.h"

// Overlay management is driven by the definition of the appropriate 
// compilation symbol (__OVERLAY__). In this case, we enable or disable the 
// compilation of the relevant code.

#ifdef __OVERLAY__

    // In general, the compiler places the executable code in the "CODE" 
    // segment, while the non-modifiable data in the "RODATA" segment.
    // In order to save space, both of these segments are moved to the 
    // overlay segment, so that both the code and the data it uses are 
    // removed from the resident module.
    #pragma code-name ("OVERLAY3");
    #pragma rodata-name ("OVERLAY3");

#endif

/****************************************************************************
 ** RESIDENT VARIABLES SECTION
 ****************************************************************************/

extern int language;

/****************************************************************************
 ** OVERLAYED FUNCTIONS SECTION
 ****************************************************************************/

// The functions defined at this level can only be called up if the current
// module has been loaded into memory. On the other hand, they can call any 
// function declared at the resident module level.

/**
 * This function print the title of the Divine Comedy
 */
void presentation(void)
{
    puts(" LA DIVINA COMMEDIA");
    puts("  di Dante Alighieri");
    puts(" ");
}

/**
 * This function allows to choose the language (italian / english)
 */
void choose_language(void)
{
    int c;

    do {
        puts("Which language?");
        puts("  0) italiano");
        puts("  1) inglese");
        c = getchar();
        if (c == '0') {
            language = 0;
            break;
        }
        else if (c == '1') {
            language = 1;
            break;
        }
        puts("");
        puts("Please digit 0 for italian or 1 for english.");
        puts("");
    } while (1); // Repeat until a choice has been taken

}
