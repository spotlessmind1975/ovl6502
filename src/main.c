/****************************************************************************
 * ovl - Overlay Example on unexpanded 6502 retrocomputers                  *
 *                                                                          *
 * Copyright (c) 2020 by Marco Spedaletti. Licensed under CC-BY-NC-SA       *
 *--------------------------------------------------------------------------*
 * RESIDENT MODULE                                                          *
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

    /************************************************************************
     ** OVERLAY MANAGEMENT SECTION
     ************************************************************************/

    // We differentiate management if we are with a Commodore system or not. 
    // The difference lies in the fact that, in the case of "commodore" 
    // (__CBM__) targets, we take advantage of the fact that the binaries 
    // produced contain, at the beginning of the file, the starting position 
    // where to load the code.

    #ifndef __CBM__

        //-------------------------------------------------------------------
        // GENERAL OVERLAY MANAGEMENT
        //-------------------------------------------------------------------

        #include <fcntl.h>
        #include <unistd.h>

        /**
         * This function loads a module (code / data) named "module_name" 
         * from the mass storage into memory. The module will be loaded 
         * starting from address "overlay_address" for a length of 
         * overlay_size bytes.
         * It returns 0 if any error occours, and print out an error message.
         */
        unsigned char load_overlay(char* module_name, void* overlay_address, void* overlay_size)
        {
            int f = open(module_name, O_RDONLY);
            if (f == -1) {
                puts("Internal error - errore interno.");
                return 0;
            }
            read(f, overlay_address, (unsigned)overlay_size);
            close(f);
            return 1;
        }

    #else

        //-------------------------------------------------------------------
        // COMMODORE OVERLAY MANAGEMENT
        //-------------------------------------------------------------------

        #include <cbm.h>
        #include <device.h>

        /**
         * This function loads a module (code / data) named "module_name" 
         * from the mass storage into the address present in the header of 
         * the binary file. 
         * It returns 0 if any error occours, and print out an error message.
         */
        unsigned char load_overlay(char* module_name, void* overlay_address, void* overlay_size)
        {
            // Ignore overlay_address and overlay_size parameters
            (void)overlay_address; (void)overlay_size;
            if (cbm_load(module_name, getcurrentdevice(), NULL) == 0) {
                puts("Internal error - errore interno.");
                return 0;
            }
            return 1;
        }

    #endif

#endif

/****************************************************************************
 ** RESIDENT VARIABLES SECTION
 ****************************************************************************/

// All variables defined as global within the resident body of the code are 
// accessible from all modules, both resident and changing ones.

// Chosen language by user (0 = italian, 1 = english)
int language = 0;

// Chosen "canto" by user (1 = CANTO I, 2 = CANTO II)
int canto = 0;

/****************************************************************************
 ** RESIDENT FUNCTIONS SECTION
 ****************************************************************************/

// All the functions defined within the resident body of the code are 
// accessible from all modules, both resident and changing ones.

/**
 * This function print out the title of a "canto".
 */
void write_title(char* title) {
    int i = 0;
    for (i = 0; i < 25; ++i) puts("");
    puts(title);
}

/**
 * This function print out a single verse of a "canto".
 */
void write_verse(char* verse) {
    puts(verse);
}

/**
 * This function wait a keypress.
 */
void press_any_key() {
    if (language == 0) {
        puts("Premi un tasto per continuare");
    } else {
        puts("Press any key to continue");
    }
    getchar();
}

/****************************************************************************
 ** RESIDENT MAIN FUNCTION
 ****************************************************************************/

// This is the main function body. The purpose is to call the various 
// functions present in the modules, taking care to load the relevant 
// code / data into memory(in the "overlay" area).

void main(void) {

    // Main loop

    do {

        // presentation() and choose_language() are both defined into
        // the third module ("demo.3"): so we are going to load it before 
        // calling any of them.

        #ifdef __OVERLAY__
        if (load_overlay("demo.3", _OVERLAY3_LOAD__, _OVERLAY3_SIZE__)) {
        #endif

            // Present the program.
            presentation();

            // Ask the user for the language
            choose_language();

        #ifdef __OVERLAY__
        }
        #endif

        // Canto's loop.
        do {

            // choose_canto() is defined into the fourth module ("demo.4"):
            // so we are going to load it before calling it.

            #ifdef __OVERLAY__
            if (load_overlay("demo.4", _OVERLAY3_LOAD__, _OVERLAY3_SIZE__)) {
            #endif
                choose_canto();
            #ifdef __OVERLAY__
            }
            #endif

            // According to the selected "canto"...

            switch (canto) {

                // CANTO I
                case 1:

                    // canto1() is defined into the first module ("demo.1"):
                    // so we are going to load it before calling it.

                    #ifdef __OVERLAY__
                    if (load_overlay("demo.1", _OVERLAY3_LOAD__, 
                                                        _OVERLAY3_SIZE__)) {
                    #endif
                        canto1();
                    #ifdef __OVERLAY__
                    }
                    #endif
                    break;

                // CANTO II
                case 2:

                    // canto2() is defined into the first module ("demo.1"):
                    // so we are going to load it before calling it.

                    #ifdef __OVERLAY__
                    if (load_overlay("demo.2", _OVERLAY3_LOAD__, 
                                                        _OVERLAY3_SIZE__)) {
                    #endif
                        canto2();
                    #ifdef __OVERLAY__
                    }
                    #endif

                    break;
            }
        } while (canto != 0); // Repeat until a quit is chosen

    } while (1); // Repeat forever

}