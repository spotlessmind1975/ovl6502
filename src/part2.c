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
    #pragma code-name ("OVERLAY2");
    #pragma rodata-name ("OVERLAY2");

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
 * This function print the second "canto" of Divine Comedy.
 */
void canto2(void)
{
    write_title("CANTO II");
    if (language == 0) {
        write_verse("<Per me si va ne la citta' dolente,");
        write_verse("per me si va ne l'etterno dolore,");
        write_verse("per me si va tra la perduta gente.");
        write_verse(" ");
        write_verse("Giustizia mosse il mio alto fattore;");
        write_verse("fecemi la divina podestate,");
        write_verse("la somma sapïenza e ’l primo amore.");
        write_verse(" ");
        press_any_key();
        write_verse(" ");
        write_verse("Dinanzi a me non fuor cose create");
        write_verse("se non etterne, e io etterna duro.");
        write_verse("Lasciate ogne speranza, voi ch’intrate’.>");
        write_verse(" ");
        write_verse("Queste parole di colore oscuro");
        write_verse("vid’io scritte al sommo d’una porta;");
        write_verse("per ch’io : <Maestro, il senso lor m’e' duro>.");
        write_verse(" ");
        press_any_key();
        write_verse(" ");
        write_verse("Ed elli a me, come persona accorta :");
        write_verse("<Qui si convien lasciare ogne sospetto;");
        write_verse("ogne vilta' convien che qui sia morta.");
        write_verse(" ");
        write_verse("Noi siam venuti al loco ov’i’ t’ ho detto");
        write_verse("che tu vedrai le genti dolorose");
        write_verse("c’ hanno perduto il ben de l’intelletto.>");
        write_verse(" ");
    }
    else {
        write_verse("THROUGH me the way is to the city dolent;");
        write_verse("Through me the way is to eternal dole;");
        write_verse("Through me the way among the people lost.");
        write_verse(" ");
        write_verse("(more to come)");
    }
}
