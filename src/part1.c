/****************************************************************************
 * ovl - Overlay Example on unexpanded 6502 retrocomputers                  *
 *                                                                          *
 * Copyright (c) 2020 by Marco Spedaletti. Licensed under CC-BY-NC-SA       *
 *--------------------------------------------------------------------------*
 * OVERLAYED MODULE                                                          *
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
    #pragma code-name ("OVERLAY1");
    #pragma rodata-name ("OVERLAY1");

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
 * This function print the first "canto" of Divine Comedy.
 */
void canto1(void)
{
    write_title("CANTO I");
    if (language == 0) {
        write_verse("Nel mezzo del cammin di nostra vita");
        write_verse("mi ritrovai per una selva oscura");
        write_verse("che' la diritta via era smarrita.");
        write_verse(" ");
        write_verse("Ahi quanto a dir qual era e' cosa dura");
        write_verse("esta selva selvaggia e aspra e forte");
        write_verse("che nel pensier rinova la paura!");
        write_verse(" ");
        press_any_key();
        write_verse(" ");
        write_verse("Tant’e' amara che poco e' piu' morte;");
        write_verse("ma per trattar del ben ch’i’ vi trovai,");
        write_verse("diro' de l’altre cose ch’i’ v’ ho scorte.");
        write_verse(" ");
        write_verse("Io non so ben ridir com’i’ v’intrai,");
        write_verse("tant’era pien di sonno a quel punto");
        write_verse("che la verace via abbandonai.");
        write_verse(" ");
        press_any_key();
        write_verse(" ");
        write_verse("Ma poi ch’i’ fui al pie' d’un colle giunto,");
        write_verse("la' dove terminava quella valle");
        write_verse("che m’avea di paura il cor compunto,");
        write_verse(" ");
        write_verse("guardai in alto e vidi le sue spalle");
        write_verse("vestite gia' de’ raggi del pianeta");
        write_verse("che mena dritto altrui per ogne calle.");
        write_verse(" ");
    } else {
        write_verse("When I had journeyed half of our life's way,");
        write_verse("I found myself within a shadowed forest");
        write_verse("for I had lost the path that does not stray.");
        write_verse(" ");
        write_verse("(more to come)");
    }
}
