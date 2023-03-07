
#=============	GAME-TEXTURE-AUTOMATION-SOFTWARE-BUILD-SYSTEM	=================

#GLOBAL DEFINES
DEFINES = DEBUG DEBUG_UNBUFFERED  MAGICKCORE_QUANTUM_DEPTH=16 MAGICKCORE_HDRI_ENABLE=0 

#GCC FLAGS
FLAGS = -m64 -std=c99

#ROOT INCLUDE DIRECTORY
INCLUDE_ROOT_DIR = ./include/

#GTK LIBRARY
GTK_INCLUDES = gtk atk fribidi cairo gdk-pixbuf glib  pango lzo libpng16 freetype2 pixman harfbuzz glib
GTK_LIBS = -L./lib/ -lgtk-3 -lgdk-3 -lz -latk-1.0 -lgio-2.0 -lpangowin32-1.0 -lpangocairo-1.0 -lgdk_pixbuf-2.0 -lcairo-gobject -lpango-1.0 -lharfbuzz -lcairo -lgobject-2.0 -lglib-2.0 -lintl

#MAGICK CORE LIBRARY
MAGICK_CORE_INCLUDES = MagickCore
MAGICK_CORE_LIBS = -L./lib/MagickCore/coders/ -L./lib/MagickCore/filters/

#STB LIBRARY
STB_INCLUDES = stb

#CWALK
CWALK_LIBS = -lcwalk


#SOURCE FILES
SOURCE_FILES = $(wildcard src/*.c)
#OBJECT FILES
OBJECT_FILES = $(addsuffix .o, $(basename $(addprefix src/, $(notdir $(SOURCE_FILES)))))


__FLAGS = $(FLAGS) $(addprefix -D, $(DEFINES))
SYSTEM_LIBS = -luuid -pthread -ldwmapi -lsetupapi -lcfgmgr32 -lgdi32 -limm32 -lshell32 -lole32 -lwinmm
INCLUDES = $(addprefix -I$(INCLUDE_ROOT_DIR),  $(GTK_INCLUDES) $(MAGICK_CORE) $(STB_INCLUDES)) -I$(INCLUDE_ROOT_DIR)
LIBS = $(SYSTEM_LIBS)  $(GTK_LIBS)  $(MAGICK_CORE_LIBS) $(CWALK_LIBS)

.PHONY: all
all: main

.PHONY: install
install: 
	@echo install is not implemented yet!

%.o: %.c
	gcc $(__FLAGS) $(INCLUDES) -c $< -o $@

main: $(OBJECT_FILES)
	gcc $(__FLAGS) $^  $(LIBS)  -o $@
	@echo build success!

.PHONY: clean
clean:
	cd src & rm -rf $(notdir $(OBJECT_FILES)) main.exe
	@echo cleaned!