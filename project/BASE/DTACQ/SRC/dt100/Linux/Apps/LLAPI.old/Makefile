#****************************************************************************
#
# Makefile for LLAPI
# This is a GNU make (gmake) makefile
#****************************************************************************

CROSS := NO
# DEBUG can be set to YES to include debugging info, or NO otherwise
DEBUG          := NO

# PROFILE can be set to YES to include profiling info, or NO otherwise
PROFILE        := NO

#****************************************************************************

include ../d-top.mak


#****************************************************************************
# API Library
#****************************************************************************
LIBNAME=libllc216
SONAME=$(LIBNAME).so
LIBFILE=$(SONAME).0.1
LIBCC=$(patsubst lib%,%, $(LIBNAME))

LIBS := -L ./$(LIBDIR) -l$(LIBCC)

LSRCS  := acq216_capi acq216impl
LOBJS := $(patsubst %,$(OBJDIR)/%.o,$(LSRCS))


EXTRA_LIBS=-L../lib/$(ARCH)/lib -ltinyxml -lacq -lpopt



DEBUG_CXXFLAGS   := $(DEBUG_CFLAGS) 
RELEASE_CXXFLAGS := $(RELEASE_CFLAGS)

DEBUG_LDFLAGS    := -g
RELEASE_LDFLAGS  :=

#****************************************************************************
# Preprocessor directives
#****************************************************************************


#****************************************************************************
# Include paths
#****************************************************************************

INCS := -I../include

CFLAGS += $(INCS)
CXXFLAGS += $(INCS)

#****************************************************************************
# Targets of the build
#**************************************************************************

APPS += acq216_test acq216capi_test

XAPPS := $(patsubst %,$(EXEDIR)/%,$(APPS))

OUTPUT := $(XAPPS)

TLIBSRC=acq216_capi acq216impl

all: lib apps

lib: $(LIBDIR)/$(LIBFILE)
	echo lib target $(LIBDIR)/$(LIBFILE)

apps: $(XAPPS)

.PHONY: lib apps


#****************************************************************************
# Output
#****************************************************************************


$(XAPPS) :  $(patsubst %,$(OBJDIR)/%.o,$(notdir $(XAPPS)))
	echo XAPPS $(XAPPS)
	echo $(patsubst %,$(OBJDIR)/%.o,$(notdir $@) $(TLIBSRC))
	$(CC) -o $@ $(patsubst %,$(OBJDIR)/%.o,$(notdir $@))  \
	$(LDFLAGS) $(LIBS) $(EXTRA_LIBS)



$(LIBDIR)/$(LIBFILE) : $(LOBJS)
	ls $(LIBDIR)
	ls $(LIBDIR)
	$(CXX) -shared -Wl,-soname,$(SONAME) -fPIC -o $@ $^ \
	-lstdc++
	-cd $(LIBDIR); \
		$(STRIP) $(LIBFILE); \
		ln -sf $(LIBFILE) $(SONAME); \
		ln -sf $(LIBFILE) $(LIBNAME).so
ifeq (YES, $(CROSS))
	cp -a $(LIBDIR)/* ../lib
endif



clean2:
	-rm -Rf $(OUTDIR)
clean:
	make CROSS=NO clean2
	make CROSS=YES clean2	
	-rm -f core *.o $(OUTPUT) $(LOBJS)

depend:
	makedepend $(INCS) $(SRCS)

release:
ifeq (YES, $(CROSS))
	$(STRIP) $(OUTPUT)
	cp $(OUTPUT) ../../rootfs/extra/local/bin
	cp *.xml ../../rootfs/extra/local/etc/cal/
	./makelinks ../../rootfs/extra/local/bin
	cp -a *.so* ../../rootfs/extra/local/lib
	cp -a *.so* ../lib
else
	echo no release action for x86
endif

include ../d-bot.mak

.PHONY: clean clean2

