# /****************************************************************************
#  * ovl - Overlay Example on unexpanded 6502 retrocomputers                  *
#  *                                                                          *
#  * Copyright (c) 2020 by Marco Spedaletti. Licensed under CC-BY-NC-SA       *
#  *--------------------------------------------------------------------------*
#  * LINKER DEFINITIONS                                                       *
#  ****************************************************************************/
###############################################################################
## This makefile was created with the aim of generalizing the compilation of 
## a source for different 8-bit environments. It follows that, before you can 
## use it, you must indicate which environments you are interested in obtaining
## an executable for.
###############################################################################

# Enter the name of the program here. This is also the name of any secondary 
# files, such as disk or cassette images.
PROGRAMNAME := demo

# Allowed targets:
# (currently "vic20" target cannot be selected due to memory limits)
#  - c64: single executable for Commodore 64 (named on disk: "demo-single")
#  - c64ovl: overlayed executable for Commodore 64 (named on disk: "demo")
#  - vic20: single executable for VIC 20 (named on disk: "demo-single")
#  - vic20ovl: overlayed executable for Commodore 64 (named on disk: "demo")
TARGETS := c64 c64ovl vic20ovl

###############################################################################
###############################################################################
###############################################################################

###############################################################################
## UNIX / WINDOWS ENVIROMENT
###############################################################################
# The "Native Win32" GNU Make contains quite some workarounds to get along with
# cmd.exe as shell. However it does not provide means to determine that it does
# actually activate those workarounds. Especially $(SHELL) does NOT contain the
# value 'cmd.exe'. So the usual way to determine if cmd.exe is being used is to
# execute the command 'echo' without any parameters. Only cmd.exe will return a
# non-empty string - saying 'ECHO is on/off'.
#
# Many "Native Win32" programs accept '/' as directory delimiter just fine. How-
# ever the internal commands of cmd.exe generally require '\' to be used.
#
# cmd.exe has an internal command 'mkdir' that doesn't understand nor require a
# '-p' to create parent directories as needed.
#
# cmd.exe has an internal command 'del' that reports a syntax error if executed
# without any file so make sure to call it only if there's an actual argument.
ifeq ($(shell echo),)
  MKDIR = mkdir -p $1
  RMDIR = rmdir $1
  RMFILES = $(RM) $1
  COPYFILES = cp $1 $2
else
  MKDIR = mkdir $(subst /,\,$1)
  RMDIR = rmdir $(subst /,\,$1)
  RMFILES = $(if $1,del /f $(subst /,\,$1))
  COPYFILES = copy $(subst /,\,$1) $(subst /,\,$2)
endif
COMMA := ,
SPACE := $(N/A) $(N/A)
define NEWLINE


endef
# Note: Do not remove any of the two empty lines above !

# On Windows it is mandatory to have CC65_HOME set. So do not unnecessarily
# rely on cl65 being added to the PATH in this scenario.
ifdef CC65_HOME
  CC := $(CC65_HOME)/bin/cl65
else
  CC := cl65
endif

# On Windows it is mandatory to have CC1541_HOME set. So do not unnecessarily
# rely on being added to the PATH in this scenario.
ifdef CC1541_HOME
  CC1541 := $(CC1541_HOME)/cc1541
else
  CC1541 := cc1541
endif

###############################################################################
## COMPILATION / LINKING OPTIONS
###############################################################################

CFLAGS := 
LDFLAGS := 
CRT :=
REMOVES :=

# Compiler flags used to tell the compiler to optimise for SPEED
define _optspeed_
  CFLAGS += -Oris
endef

# Compiler flags used to tell the compiler to optimise for SIZE
define _optsize_
  CFLAGS += -Or
endef

# Linker flags for generating map file
define _mapfile_
  LDFLAGS += --mapfile $$@.map
  REMOVES += $(PROGRAM).map
endef

# Linker flags for generating VICE label file
define _labelfile_
  LDFLAGS += -Ln $$@.lbl
  REMOVES += $(PROGRAM).lbl
endef

# Linker flags for generating a debug file
define _debugfile_
  LDFLAGS += -Wl --dbgfile,$$@.dbg
  REMOVES += $(PROGRAM).dbg
endef

###############################################################################
## MAKEFILE'S "CORE"
###############################################################################

# This is the set of files that will have to be analyzed. Let's consider, at 
# the moment, only the C sources are since he ASM ones are specific for each 
# environment.
SOURCES := $(wildcard src/*.c)

# Let's calculate what the names of the object files could be. Usually, there 
# will be one for each source. Object files are stored in a separate location 
# for each target environment. 
OBJS := $(addsuffix .o,$(basename $(addprefix obj/PLATFORM/,$(SOURCES:src/%=%))))

# Here we expand every single object produced, according to each expected 
# environment. In this way you get the complete list of all object files to be 
# compiled separately, each according to the compiler suitable for that 
# environment.
OBJECTS := $(foreach TARGET,$(TARGETS),$(subst PLATFORM,$(TARGET),$(OBJS)))

# We generate the list of paths where the object files for each target will 
# end, so that we can generate them in advance (as paths).
TARGETOBJDIR := $(foreach TARGET,$(TARGETS),obj/$(TARGET))

# This is the path where all executables will be put.
EXEDIR := exe

# Similarly, we expand the set of executables that are required.
EXES := $(foreach TARGET,$(TARGETS),$(EXEDIR)/$(PROGRAMNAME).$(TARGET))

###############################################################################
## PLATFORMS' RULES
###############################################################################

##-----------------------------------------------------------------------------
## CC65
##-----------------------------------------------------------------------------

## C64 ------------------------------------------------------------------------

# Let's define rules to compile the demo under C=64 as a one and single 
# executable file. This compilation is used as a "functional check", to
# be sure that the source implementation is correct. Moreover, the executable 
# file will be put on a D64 1541 image, along with the overlay version
# generated by other rules.
obj/c64/%.o:	$(SOURCES)
	$(CC) -t c64 -c $(CFLAGS) -D__CBM__ -o $@ $(subst obj/c64/,src/,$(@:.o=.c))

$(EXEDIR)/$(PROGRAMNAME).c64:	$(subst PLATFORM,c64,$(OBJS))
	$(CC) -t c64 $(LDFLAGS) -o $(EXEDIR)/$(PROGRAMNAME).c64 $(subst PLATFORM,c64,$(OBJS))
	$(CC1541) -f $(PROGRAMNAME)-single -w $(EXEDIR)/$(PROGRAMNAME).c64 $(EXEDIR)/$(PROGRAMNAME).c64.d64  

# Let's define rules to compile the demo under C=64 as the overlay version.
# Moreover, all the executable files will be put on a D64 1541 image, 
# along with the single file version.
obj/c64ovl/%.o:	$(SOURCES)
	$(CC) -t c64 -c $(CFLAGS) -D__CBM__ -D__OVERLAY__ -o $@ $(subst obj/c64ovl/,src/,$(@:.o=.c)) 

# This rule will produce the final binary file for C=64 platform.
$(EXEDIR)/$(PROGRAMNAME).c64ovl:	$(subst PLATFORM,c64ovl,$(OBJS))
	$(CC) -t c64 $(LDFLAGS) -C cfg/c64-overlay.cfg  -o $(EXEDIR)/$(PROGRAMNAME).c64ovl $(subst PLATFORM,c64ovl,$(OBJS))
	$(CC1541) -f $(PROGRAMNAME) -w $(EXEDIR)/$(PROGRAMNAME).c64ovl $(EXEDIR)/$(PROGRAMNAME).c64.d64  
	$(CC1541) -f $(PROGRAMNAME).1 -w $(EXEDIR)/$(PROGRAMNAME).c64ovl.1 $(EXEDIR)/$(PROGRAMNAME).c64.d64  
	$(CC1541) -f $(PROGRAMNAME).2 -w $(EXEDIR)/$(PROGRAMNAME).c64ovl.2 $(EXEDIR)/$(PROGRAMNAME).c64.d64  
	$(CC1541) -f $(PROGRAMNAME).3 -w $(EXEDIR)/$(PROGRAMNAME).c64ovl.3 $(EXEDIR)/$(PROGRAMNAME).c64.d64  
	$(CC1541) -f $(PROGRAMNAME).4 -w $(EXEDIR)/$(PROGRAMNAME).c64ovl.4 $(EXEDIR)/$(PROGRAMNAME).c64.d64  

## VIC20 ------------------------------------------------------------------------

# Let's define rules to compile the demo under C=64 as a one and single 
# executable file. This compilation is used as a "functional check", to
# be sure that the source implementation is correct. Moreover, the executable 
# file will be put on a D64 1541 image, along with the overlay version
# generated by other rules.
obj/vic20/%.o:	$(SOURCES)
	$(CC) -t vic20 -c $(CFLAGS) -D__CBM__ -o $@ $(subst obj/vic20/,src/,$(@:.o=.c))

$(EXEDIR)/$(PROGRAMNAME).vic20:	$(subst PLATFORM,vic20,$(OBJS))
	$(CC) -t vic20 $(LDFLAGS) -o $(EXEDIR)/$(PROGRAMNAME).vic20 $(subst PLATFORM,vic20,$(OBJS))
	$(CC1541) -f $(PROGRAMNAME)-single -w $(EXEDIR)/$(PROGRAMNAME).vic20 $(EXEDIR)/$(PROGRAMNAME).vic20.d64  


# Let's define rules to compile the demo under VIC20 as the overlay version.
# This is the only way to compile this program in order to be able to be 
# executed by this platform. All the executable files will be put on a 
# D64 1541 image.
obj/vic20ovl/%.o:	$(SOURCES)
	$(CC) -t vic20 -c $(CFLAGS) -D__CBM__ -D__OVERLAY__ -C cfg/vic20-overlay.cfg -o $@ $(subst obj/vic20ovl/,src/,$(@:.o=.c))

$(EXEDIR)/$(PROGRAMNAME).vic20ovl:	$(subst PLATFORM,vic20ovl,$(OBJS))
	$(CC) -t vic20 $(LDFLAGS) -C cfg/vic20-overlay.cfg  -o $(EXEDIR)/$(PROGRAMNAME).vic20ovl $(subst PLATFORM,vic20ovl,$(OBJS))
	$(CC1541) -f $(PROGRAMNAME) -w $(EXEDIR)/$(PROGRAMNAME).vic20ovl $(EXEDIR)/$(PROGRAMNAME).vic20.d64  
	$(CC1541) -f $(PROGRAMNAME).1 -w $(EXEDIR)/$(PROGRAMNAME).vic20ovl.1 $(EXEDIR)/$(PROGRAMNAME).vic20.d64  
	$(CC1541) -f $(PROGRAMNAME).2 -w $(EXEDIR)/$(PROGRAMNAME).vic20ovl.2 $(EXEDIR)/$(PROGRAMNAME).vic20.d64  
	$(CC1541) -f $(PROGRAMNAME).3 -w $(EXEDIR)/$(PROGRAMNAME).vic20ovl.3 $(EXEDIR)/$(PROGRAMNAME).vic20.d64  
	$(CC1541) -f $(PROGRAMNAME).4 -w $(EXEDIR)/$(PROGRAMNAME).vic20ovl.4 $(EXEDIR)/$(PROGRAMNAME).vic20.d64  

###############################################################################
## FINAL RULES
###############################################################################

$(EXEDIR):
	$(call RMFILES,$(EXEDIR)/$(PROGRAMNAME).c64.d64)
	$(call RMFILES,$(EXEDIR)/$(PROGRAMNAME).vic20.d64)
	$(call MKDIR,$@)

$(TARGETOBJDIR):
	$(call MKDIR,$@)

$(DATADIR):
	$(call MKDIR,$@)

all: $(EXEDIR) $(TARGETOBJDIR) $(EXES)

clean:
	$(call RMFILES,$(EXEDIR)/$(PROGRAMNAME).c64.d64)
	$(call RMFILES,$(EXEDIR)/$(PROGRAMNAME).vic20.d64)
	$(call RMFILES,$(EXES))
	$(call RMFILES,$(OBJECTS))
