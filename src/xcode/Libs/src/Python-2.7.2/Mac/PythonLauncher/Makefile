CC=gcc-4.2
LD=gcc-4.2
BASECFLAGS= -fno-strict-aliasing -fno-common -dynamic
OPT=-DNDEBUG -g -fwrapv -O3 -Wall -Wstrict-prototypes
CFLAGS=-isysroot /Developer/SDKs/MacOSX10.6.sdk/ -arch i386 -arch ppc -arch x86_64  $(BASECFLAGS) $(OPT)
LDFLAGS=-arch i386 -arch ppc -arch x86_64 -isysroot /Developer/SDKs/MacOSX10.6.sdk/ -isysroot /Developer/SDKs/MacOSX10.6.sdk/  -L/opt/mapc/lib -L/opt/cuscus/lib -L/usr/local/lib -L/opt/local/lib -L/Developer/usr/lib -L/lib -L/usr/lib -L/opt/X11/lib -L/usr/X11/lib -L/opt/sw/lib
srcdir=         .
VERSION=	2.7
UNIVERSALSDK=/Developer/SDKs/MacOSX10.6.sdk/
builddir=	../..

RUNSHARED=      DYLD_FRAMEWORK_PATH=/Developer/Projects/cube2/sauerbomber/trunk/src/xcode/Libs/src/Python-2.7.2:
BUILDEXE=       .exe
BUILDPYTHON=    $(builddir)/python$(BUILDEXE)
PYTHONFRAMEWORK=Python

# Deployment target selected during configure, to be checked
# by distutils  
MACOSX_DEPLOYMENT_TARGET=10.5
export MACOSX_DEPLOYMENT_TARGET

BUNDLEBULDER=$(srcdir)/../../Lib/plat-mac/bundlebuilder.py

PYTHONAPPSDIR=/Applications/$(PYTHONFRAMEWORK) $(VERSION)
OBJECTS=FileSettings.o MyAppDelegate.o MyDocument.o PreferencesWindowController.o doscript.o main.o

all: Python\ Launcher.app

install: Python\ Launcher.app
	test -d "$(DESTDIR)$(PYTHONAPPSDIR)" || mkdir -p "$(DESTDIR)$(PYTHONAPPSDIR)"
	-test -d "$(DESTDIR)$(PYTHONAPPSDIR)/Python Launcher.app" && rm -r "$(DESTDIR)$(PYTHONAPPSDIR)/Python Launcher.app"
	/bin/cp -r "Python Launcher.app" "$(DESTDIR)$(PYTHONAPPSDIR)"
	touch "$(DESTDIR)$(PYTHONAPPSDIR)/Python Launcher.app"

clean:
	rm -f *.o "Python Launcher"
	rm -rf "Python Launcher.app"

Python\ Launcher.app:  Info.plist \
		Python\ Launcher $(srcdir)/../Icons/PythonLauncher.icns \
		$(srcdir)/../Icons/PythonSource.icns \
		$(srcdir)/../Icons/PythonCompiled.icns \
		$(srcdir)/factorySettings.plist
	rm -fr "Python Launcher.app"
	$(RUNSHARED) $(BUILDPYTHON) $(BUNDLEBULDER) \
		--builddir=. \
		--name="Python Launcher" \
		--executable="Python Launcher" \
		--iconfile=$(srcdir)/../Icons/PythonLauncher.icns \
		--bundle-id=org.python.PythonLauncher \
		--resource=$(srcdir)/../Icons/PythonSource.icns \
		--resource=$(srcdir)/../Icons/PythonCompiled.icns \
		--resource=$(srcdir)/English.lproj \
		--resource=$(srcdir)/factorySettings.plist \
		--plist Info.plist \
		build
	find "Python Launcher.app" -name '.svn' -print0 | xargs -0 rm -r
		

FileSettings.o: $(srcdir)/FileSettings.m
	$(CC) $(CFLAGS) -o $@ -c $(srcdir)/FileSettings.m

MyAppDelegate.o: $(srcdir)/MyAppDelegate.m
	$(CC) $(CFLAGS) -o $@ -c $(srcdir)/MyAppDelegate.m

MyDocument.o: $(srcdir)/MyDocument.m
	$(CC) $(CFLAGS) -o $@ -c $(srcdir)/MyDocument.m

PreferencesWindowController.o: $(srcdir)/PreferencesWindowController.m
	$(CC) $(CFLAGS) -o $@ -c $(srcdir)/PreferencesWindowController.m

doscript.o: $(srcdir)/doscript.m
	$(CC) $(CFLAGS) -o $@ -c $(srcdir)/doscript.m

main.o: $(srcdir)/main.m
	$(CC) $(CFLAGS) -o $@ -c $(srcdir)/main.m

Python\ Launcher: $(OBJECTS)
	$(CC) $(LDFLAGS) -o "Python Launcher" $(OBJECTS) -framework AppKit -framework Carbon

Info.plist: $(srcdir)/Info.plist.in
	sed 's/%VERSION%/'"`$(RUNSHARED) $(BUILDPYTHON) -c 'import platform; print platform.python_version()'`"'/g' < $(srcdir)/Info.plist.in > Info.plist
