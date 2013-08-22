MAKE=make
MAKE_ARG=
SUBDIRS_TEMPLATE= \
src/nldb/BUILD_FOLDER \
src/gtest/BUILD_FOLDER \
src/tests/BUILD_FOLDER 

all: release

# By default, build in release mode. No MAKE_ARG is required for the release mode.
release: SUBDIRS = $(SUBDIRS_TEMPLATE:BUILD_FOLDER=GccRelease)
release: compile-release

# Run the debug build
debug: SUBDIRS = $(SUBDIRS_TEMPLATE:BUILD_FOLDER=GccDebug)
debug: compile-debug

clean: clean-debug clean-release

# Clean the release build
clean-release: SUBDIRS = $(SUBDIRS_TEMPLATE:BUILD_FOLDER=GccRelease) 
clean-release: MAKE_ARG=clean
clean-release: compile-release

# Clean the debug build
clean-debug: SUBDIRS = $(SUBDIRS_TEMPLATE:BUILD_FOLDER=GccDebug)
clean-debug: MAKE_ARG=clean
clean-debug: compile-debug

prepare-build:
	./prepare-build.sh

# Why having compile-debug and compile-release running the same script?
# I separated these to clean both debug and release build when 'make clean' runs.
# If I merge these into one, make cleans up either debug or release, not both of them.
compile-debug:
	for dir in $(SUBDIRS); do \
	    $(MAKE) $(MAKE_ARG) -C $$dir; \
	done

compile-release:
	for dir in $(SUBDIRS); do \
	    $(MAKE) $(MAKE_ARG) -C $$dir; \
	done

