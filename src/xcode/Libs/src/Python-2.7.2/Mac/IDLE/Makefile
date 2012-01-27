prefix=/Frameworks//Python.framework/Versions/2.7
CC=gcc-4.2
LD=gcc-4.2
BASECFLAGS= -fno-strict-aliasing -fno-common -dynamic
OPT=-DNDEBUG -g -fwrapv -O3 -Wall -Wstrict-prototypes
CFLAGS=$(BASECFLAGS) $(OPT)
LDFLAGS=-arch i386 -arch ppc -arch x86_64 -isysroot /Developer/SDKs/MacOSX10.6.sdk/ -isysroot /Developer/SDKs/MacOSX10.6.sdk/  -L/opt/mapc/lib -L/opt/cuscus/lib -L/usr/local/lib -L/opt/local/lib -L/Developer/usr/lib -L/lib -L/usr/lib -L/opt/X11/lib -L/usr/X11/lib -L/opt/sw/lib
srcdir=         .
VERSION=	2.7
UNIVERSALSDK=/Developer/SDKs/MacOSX10.6.sdk/
builddir=	../..
PYTHONFRAMEWORK=Python
LIPO_32BIT_FLAGS=-extract ppc7400 -extract i386


RUNSHARED=      DYLD_FRAMEWORK_PATH=/Developer/Projects/cube2/sauerbomber/trunk/src/xcode/Libs/src/Python-2.7.2:
BUILDEXE=       .exe
BUILDPYTHON=    $(builddir)/python$(BUILDEXE)

# Deployment target selected during configure, to be checked
# by distutils  
MACOSX_DEPLOYMENT_TARGET=10.5
export MACOSX_DEPLOYMENT_TARGET

BUNDLEBULDER=$(srcdir)/../../Lib/plat-mac/bundlebuilder.py

PYTHONAPPSDIR=/Applications/$(PYTHONFRAMEWORK) $(VERSION)

all: IDLE.app

install: IDLE.app $(srcdir)/config-main.def $(srcdir)/config-extensions.def
	test -d "$(DESTDIR)$(PYTHONAPPSDIR)" || mkdir -p "$(DESTDIR)$(PYTHONAPPSDIR)"
	-test -d "$(DESTDIR)$(PYTHONAPPSDIR)/IDLE.app" && rm -r "$(DESTDIR)$(PYTHONAPPSDIR)/IDLE.app"
	/bin/cp -PR IDLE.app "$(DESTDIR)$(PYTHONAPPSDIR)"
	touch "$(DESTDIR)$(PYTHONAPPSDIR)/IDLE.app"
	/bin/cp $(srcdir)/config-main.def "$(DESTDIR)$(prefix)/lib/python$(VERSION)/idlelib/config-main.def"
	/bin/cp $(srcdir)/config-extensions.def "$(DESTDIR)$(prefix)/lib/python$(VERSION)/idlelib/config-extensions.def"

clean:
	rm -rf IDLE.app

IDLE.app:  \
		$(srcdir)/../Icons/IDLE.icns $(srcdir)/idlemain.py \
		$(srcdir)/../Icons/PythonSource.icns \
		$(srcdir)/../Icons/PythonCompiled.icns Info.plist
	rm -fr IDLE.app
	$(RUNSHARED)  $(BUILDPYTHON) $(BUNDLEBULDER) \
		--builddir=. \
		--name=IDLE \
		--link-exec \
		--plist=Info.plist \
		--mainprogram=$(srcdir)/idlemain.py \
		--iconfile=$(srcdir)/../Icons/IDLE.icns \
		--resource=$(srcdir)/../Icons/PythonSource.icns \
		--resource=$(srcdir)/../Icons/PythonCompiled.icns \
		--python=$(prefix)/Resources/Python.app/Contents/MacOS/Python \
		build
ifneq ($(LIPO_32BIT_FLAGS),)
	rm "IDLE.app/Contents/MacOS/Python"
	lipo $(LIPO_32BIT_FLAGS) -output "IDLE.app/Contents/MacOS/Python" "$(BUILDPYTHON)"
endif

Info.plist: $(srcdir)/Info.plist.in
	sed 's/%VERSION%/'"`$(RUNSHARED) $(BUILDPYTHON) -c 'import platform; print platform.python_version()'`"'/g' < $(srcdir)/Info.plist.in > Info.plist

