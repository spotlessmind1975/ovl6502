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
    #pragma code-name ("OVERLAY4");
    #pragma rodata-name ("OVERLAY4");

#endif

/****************************************************************************
 ** RESIDENT VARIABLES SECTION
 ****************************************************************************/

extern int language;
extern int canto;

/****************************************************************************
 ** OVERLAYED FUNCTIONS SECTION
 ****************************************************************************/

// The functions defined at this level can only be called up if the current
// module has been loaded into memory. On the other hand, they can call any 
// function declared at the resident module level.

/**
 * This function allow the user to choose the "canto" to be displayed
 */
void choose_canto(void)
{
    int c;

    do {
        puts("");
        if (language == 0) {
            puts("Quale canto?");
        }
        else {
            puts("Which canto?");
        }
        puts("  1) CANTO I");
        puts("  2) CANTO II");
        if (language == 0) {
            puts("  Q) Esci");
        }
        else {
            puts("  Q) Quit");
        }
        c = getchar();
        if (c == '1') {
            canto = 1;
            break;
        }
        else if (c == '2') {
            canto = 2;
            break;
        }
        else if (c == 'Q' || c == 'q') {
            canto = 0;
            break;
        }
        else {
            puts("");
            if (language == 0) {
                puts("Digitare 1 per CANTO I o 2 per CANTO II.");
            }
            else {
                puts("Please digit 1 for CANTO I or 2 for CANTO II.");
            }
            puts("");
        }
    } while (1); // Repeat until a choice has been taken
}
