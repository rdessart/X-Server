SRC_BASE	:=	.
INCLUDES    := -I ~/vcpkg/installed/arm64-osx/include/
LIBRARIES   := 
BUILDDIR	:=	./build
ARCH 		:= arm64
DEFINES     :=  -DAPL=1
CFLAGS      := $(DEFINES) $(INCLUDES) -fPIC -fvisibility=hidden -std=c++17  -arch $(ARCH)
LINKER      := $(LIBRARIES) -arch $(ARCH)

$(BUILDDIR)/X-Server.Logging.ar : $(BUILDDIR)/Logger.o |  $(BUILDDIR)
	ar r X-Server.Logging.ar $(BUILDDIR)/Logger.o

$(BUILDDIR)/Logger.o : include/Logger.h  src/Logger.cpp | $(BUILDDIR)
	clang $(CFLAGS) src/Logger.cpp -o $(BUILDDIR)/logger.o -c


$(BUILDDIR) :
	mkdir $(BUILDDIR)

clean:
	rm -r $(BUILDDIR)