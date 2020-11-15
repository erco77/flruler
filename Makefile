# FLRULER
# erco@seriss.com 1.1

SHELL=/bin/sh
FLTK_CONFIG=fltk-config

# FLTK BUILD 
CXX      = $(shell $(FLTK_CONFIG) --cxx)
CXXFLAGS = $(shell $(FLTK_CONFIG) --use-images --cxxflags )
LDLIBS   = $(shell $(FLTK_CONFIG) --use-images --ldstaticflags )
LINK     = $(CXX)
VERSION  = $(shell sed 's/^[^"]*"//;s/".*//' version.H)

# WINDOWS?
ifneq ($(wildcard /c/windows/system32/*),)
	EXE=.exe
else
	EXE=
endif

# DEFAULT BUILD
all: flruler$(EXE) docs

# HOW TO BUILD
.SUFFIXES: .o .C

%.o: %.C %.H
	$(CXX) $(CXXFLAGS) -c $<

%.o: %.C %.H
	$(CXX) $(CXXFLAGS) $(DEBUG) -c $<

flruler: flruler.o
	$(LINK) flruler.o -o flruler$(EXE) $(LDLIBS) 
	$(FLTK_CONFIG) --post flruler$(EXE)
	if [ -d flruler.app ]; then \
	    cp icons/flruler-Info.plist flruler.app/Contents/Info.plist; \
	    cp icons/flruler.icns       flruler.app/Contents/Resources/icon.icns; \
	fi

# MAKE DOCUMENTATION
docs: FORCE
	( cd docs && \
          ( echo 'g/Version [0-9]*\.[0-9]*/s//Version '$(VERSION)'/g'; echo wq) | ex flruler.pod; \
	  pod2man --center="Erco's FLTK Tools" \
	          --name=flruler \
		  --section=1 \
		  --release=$(VERSION) \
		  flruler.pod > flruler.1 && \
	  echo "*** Built docs/flruler.1" && \
	  pod2html --title "flruler $(VERSION) Erco's FLTK Tools" \
	          flruler.pod > flruler.html && \
	  echo "*** Built docs/flruler.html" \
	)

# INSTALL
install: flruler$(EXE) docs
	if [ -d /Applications ]; then \
	    echo -- Installing binary in /Applications; \
	    cp -rp flruler.app /Applications ; \
	elif [ -d /usr/local/bin ]; then \
	    echo -- Installing binary in /usr/local/bin; \
	    cp flruler /usr/local/bin ; \
	else \
	    echo "Can't figure out where to install binary"; \
	    exit 1; \
	fi
	if [ -d /usr/share/man/man1 ]; then \
	    echo "-- Installing man page in /usr/share/man/man1"; \
	    cp docs/flruler.1 /usr/share/man/man1; \
	fi
	if [ -d /usr/share/doc ]; then \
	    echo -- Installing docs in /usr/share/doc/flruler-$(VERSION); \
	    if [ ! -d /usr/share/doc/flruler-$(VERSION) ]; then \
	        mkdir /usr/share/doc/flruler-$(VERSION); \
	    fi; \
	    cp docs/flruler.html /usr/share/doc/flruler-$(VERSION); \
	    cp LICENSE           /usr/share/doc/flruler-$(VERSION); \
	fi

# BUILD CLEAN UP
clean:
	-rm -f flruler
	-rm -f *.o
	-rm -f core
	-rm -f core.*
	-rm -f *.idb
	-rm -f *.pdb
	-rm -f *.obj
	-rm -f *.exe
	-rm -f *.rc
	-rm -f *.res
	-rm -f *.RES
	-rm -f docs/pod2htm*
	-rm -f docs/*.1
	-rm -f docs/*.html
	-rm -rf docs/*.z
	-rm -rf docs/*.Z
	-rm -rf ._[A-z]*
	-rm -rf flruler.app 2> /dev/null

# DISTRO CLEANUP
distclean: clean
	-rm -rf bin.*
	-rm -rf reldir

# MAKE VERBOSITY
#.SILENT:

FORCE:
	
