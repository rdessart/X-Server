SRC_BASE	:=	.
INCLUDES    := \
	-I ~/vcpkg/installed/arm64-osx/include/ \
	-I ../X-Server.Logging/include/ \
LIBRARIES   := 
BUILDDIR	:=	./build
ARCH 		:= arm64
DEFINES     :=  -DAPL=1
CFLAGS      := $(DEFINES) $(INCLUDES) -fPIC -fvisibility=hidden -std=c++17  -arch $(ARCH)
LINKER      := $(LIBRARIES) -arch $(ARCH)

$(BUILDDIR)/X-Server.Networking.ar : $(BUILDDIR)/NetworkingUtils.o $(BUILDDIR)/UDPBeacon.o $(BUILDDIR)/UDPServer.o |  $(BUILDDIR)
	ar r X-Server.Networking.ar $(BUILDDIR)/NetworkingUtils.o $(BUILDDIR)/UDPBeacon.o $(BUILDDIR)/UDPServer.o 

$(BUILDDIR)/NetworkingUtils.o : include/NetworkUtils.h  src/NetworkUtils.cpp | $(BUILDDIR)
	clang $(CFLAGS) src/NetworkUtils.cpp -o $(BUILDDIR)/NetworkingUtils.o -c

$(BUILDDIR)/UDPBeacon.o : include/UDPBeacon.h include/IServer.h src/UDPBeacon.cpp | $(BUILDDIR)
	clang $(CFLAGS) src/UDPBeacon.cpp -o $(BUILDDIR)/UDPBeacon.o -c

$(BUILDDIR)/UDPServer.o : include/UDPServer.h include/Message.h include/IServer.h  src/UDPServer.cpp | $(BUILDDIR)
	clang $(CFLAGS) src/UDPServer.cpp -o $(BUILDDIR)/UDPServer.o -c


$(BUILDDIR) :
	mkdir $(BUILDDIR)

clean:
	rm -r $(BUILDDIR)