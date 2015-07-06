OUT= pbtktest

include /usr/local/pocketbook/common.mk

ifndef BUILD
BUILD=emu
CFLAGS+=-g
CXXFLAGS+=-g
#BUILD=arm_gnueabi
endif

ifeq ($(BUILD),arm_gnueabi)
CXXFLAGS+=-I/usr/arm-none-linux-gnueabi/include/c++/4.1.2 -I/usr/local/pocketbook_eabi/include \
-I/usr/local/pocketbook_eabi/include/sigc++-2.0 
LDFLAGS+=-L/usr/local/pocketbook_eabi/lib
endif
ifeq ($(BUILD),arm)
CXXFLAGS+=-I/usr/arm-linux/include/c++/3.4.1  -I/usr/arm-linux/include/c++/3.4.1/arm-linux -I/usr/local/pocketbook/arm-linux/include \
-I/usr/local/pocketbook/arm-linux/include/sigc++-2.0 
LDFLAGS+=-L/usr/local/pocketbook/arm-linux/lib
endif
ifeq ($(BUILD),emu)
CXXFLAGS+=-g `pkg-config --cflags sigc++-2.0` -I./../
LDFLAGS+= /usr/lib32/libpng12.so.0
endif


CXXFLAGS+=-Wall \
-DHAS_NO_IV_GET_DEFAULT_FONT `freetype-config --cflags` -fPIC
#LIBS+=-lgif -lpng12

SOURCES=\
    convert.cxx\
    pbwidget.cxx\
    pbdialog.cxx\
    pbinput.cxx\
    pblabel.cxx\
    pbfilechooser.cxx\
    pblistbox.cxx\
    pbbutton.cxx\
    pbcombobox.cxx\
    selector.cxx\
    filename_match.cxx
    
SOURCES_DEPR=\
    pbpagedlistbox.cxx

SOURCES_IMG=\
    PBGIFImage.cxx\
    PBImg.cxx\
    PBJPEGImage.cxx\
    PBPNGImage.cxx

PIXMAPS=
#    images/dir_bm.xpm

PIXMAPS_C=$(PIXMAPS:.xpm=.c)

PIXMAPS_OBJS=$(addprefix $(OBJDIR)/,$(PIXMAPS_C:.c=.o))

OBJS_IMG=$(addprefix $(OBJDIR)/,$(SOURCES_IMG:.cxx=.cxx.o))

LIBPBTK_SHARED_NAME=libpbtk.so
LIBPBTK_SHARED=$(OBJDIR)/libpbtk.so

LBPBTK=$(OBJDIR)/libpbtk.a

LBPBTK_IMG=$(OBJDIR)/libpbtk_img.a

all: $(PROJECT)

$(LIBPBTK_SHARED): $(OBJDIR) $(OBJS)
	$(CXX) -shared -o $@ -fPIC $(OBJS)

$(LBPBTK): $(OBJDIR) $(OBJS)
	$(AR) cr $@ $(OBJS)
	$(AR) s $@

$(LBPBTK_IMG): $(OBJS_IMG)
	$(AR) cr $@ $(OBJS_IMG)

$(OBJDIR):
	mkdir -p $(OBJDIR)
#	mkdir -p $(OBJDIR)/images

$(PROJECT): pbtktest.cpp $(LBPBTK) $(LBPBTK_IMG) #$(LIBPBTK_SHARED)
	$(CXX) $(CXXFLAGS) -o $(PROJECT) pbtktest.cpp $(LDFLAGS) $(LBPBTK) -lsigc-2.0 -linkview

$(OBJDIR)/%.cxx.o: %.cxx
	$(CXX) -c -o $@ $(CXXFLAGS) $(INCLUDES) $(CDEPS) $<
$(OBJDIR)/%.cpp.o: %.cpp
	$(CXX) -c -o $@ $(CXXFLAGS) $(INCLUDES) $(CDEPS) $<

$(OBJDIR)/images/%.o: images/%.c
	$(CC) -c -o $@ $(CFLAGS) $(INCLUDES) $(CDEPS) $<

$(PIXMAPS_C): $(PIXMAPS)

images/%.c: images/%.xpm
	./xpbres -c $@ $<

-include $(OBJDIR)/*.d
