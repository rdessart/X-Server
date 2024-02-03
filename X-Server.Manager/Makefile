SRC_BASE	:=	.
INCLUDES    := \
	-I ~/vcpkg/installed/arm64-osx/include/ \
	-I ../X-Server.Logging/include/ \
	-I ../X-Server.Networking/include/ \
	-I ../SDK/CHeaders/
LIBRARIES   := 
BUILDDIR	:=	./build
ARCH 		:= arm64
DEFINES     :=  -DAPL=1
CFLAGS      := $(DEFINES) $(INCLUDES) -fPIC -fvisibility=hidden -std=c++20  -arch $(ARCH)
LINKER      := $(LIBRARIES) -arch $(ARCH)

$(BUILDDIR)/X-Server.Manager.ar : $(BUILDDIR)/Dataref.o $(BUILDDIR)/FFA320Dataref.o $(BUILDDIR)/DatarefManager.o $(BUILDDIR)/FlightLoopManager.o $(BUILDDIR)/OperationManager.o $(BUILDDIR)/Utils.o $(BUILDDIR)/MasterCallback.o |  $(BUILDDIR)
	ar r X-Server.Networking.ar $(BUILDDIR)/Dataref.o $(BUILDDIR)/FFA320Dataref.o $(BUILDDIR)/DatarefManager.o $(BUILDDIR)/FlightLoopManager.o $(BUILDDIR)/OperationManager.o $(BUILDDIR)/Utils.o $(BUILDDIR)/MasterCallback.o

$(BUILDDIR)/Dataref.o : include/Datarefs/Dataref.h include/Datarefs/AbstractDataref.h src/Datarefs/Dataref.cpp | $(BUILDDIR)
	clang $(CFLAGS) src/Datarefs/Dataref.cpp -o $(BUILDDIR)/Dataref.o -c

$(BUILDDIR)/FFA320Dataref.o : include/Datarefs/FFA320Dataref.h include/Datarefs/AbstractDataref.h src/Datarefs/FFA320Dataref.cpp | $(BUILDDIR)
	clang $(CFLAGS) src/Datarefs/FFA320Dataref.cpp -o $(BUILDDIR)/FFA320Dataref.o -c

$(BUILDDIR)/DatarefManager.o : include/Managers/DatarefManager.h src/Managers/DatarefManager.cpp | $(BUILDDIR)
	clang $(CFLAGS) src/Managers/DatarefManager.cpp -o $(BUILDDIR)/DatarefManager.o -c

$(BUILDDIR)/FlightLoopManager.o : include/Managers/FlightLoopManager.h src/Managers/FlightLoopManager.cpp | $(BUILDDIR)
	clang $(CFLAGS) src/Managers/FlightLoopManager.cpp -o $(BUILDDIR)/FlightLoopManager.o -c

$(BUILDDIR)/OperationManager.o : include/Managers/OperationManager.h src/Managers/OperationManager.cpp | $(BUILDDIR)
	clang $(CFLAGS) src/Managers/OperationManager.cpp -o $(BUILDDIR)/OperationManager.o -c

$(BUILDDIR)/Utils.o : include/Tools/Utils.h src/Tools/Utils.cpp | $(BUILDDIR)
	clang $(CFLAGS) src/Tools/Utils.cpp -o $(BUILDDIR)/Utils.o -c

$(BUILDDIR)/MasterCallback.o : include/MasterCallback.h src/MasterCallback.cpp | $(BUILDDIR)
	clang $(CFLAGS) src/MasterCallback.cpp -o $(BUILDDIR)/MasterCallback.o -c

$(BUILDDIR) :
	mkdir $(BUILDDIR)

clean:
	rm -r $(BUILDDIR)