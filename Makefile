MAKE=make
MAKE_ARG=
SUBDIRS_TEMPLATE= \
src/nldb/BUILD_FOLDER \
src/gtest/BUILD_FOLDER \
src/tests/BUILD_FOLDER 

all: release

# By default, build in release mode. No MAKE_ARG is required for the release mode.
release: SUBDIRS = $(SUBDIRS_TEMPLATE:BUILD_FOLDER=GccRelease)
release: compile

# Run the debug build
debug: SUBDIRS = $(SUBDIRS_TEMPLATE:BUILD_FOLDER=GccDebug)
debug: compile

clean: clean-release

# Clean the release build
clean-release: SUBDIRS = $(SUBDIRS_TEMPLATE:BUILD_FOLDER=GccRelease) 
clean-release: MAKE_ARG=clean
clean-release: compile

# Clean the debug build
clean-debug: SUBDIRS = $(SUBDIRS_TEMPLATE:BUILD_FOLDER=GccDebug)
clean-debug: MAKE_ARG=clean
clean-debug: compile

prepare-build:
	./prepare-build.sh

compile: 
	for dir in $(SUBDIRS); do \
	    $(MAKE) $(MAKE_ARG) -C $$dir; \
	done

