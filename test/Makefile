# -*- makefile-gmake -*-
#
# test/Makefile
# borrowed and slightly modified from VLC
#

TOPSRC := $(abspath ..)
OBJSUFFIX = .o
LIBSUFFIX = .a

CXX ?= g++
CXXSTD = -std=c++98
CXXFLAGS = -g -Wall -Wextra -DGTEST_LANG_CXX11=0 -pthread

# TODO -- only on Linux
ifndef NOT_LINUX
CXXFLAGS += -DLINUX -DUSE_OGG_VORBIS
endif

ifeq ($(COVERAGE),1)
CXXFLAGS += -fprofile-arcs -ftest-coverage -O0
endif

GTEST_VERSION=1.8.1
GTEST_URL=https://github.com/google/googletest/archive/release-$(GTEST_VERSION).zip
GTEST_DIR=googletest/googletest
GTEST_INCDIR=$(GTEST_DIR)/include
GMOCK_DIR=googletest/googlemock
GMOCK_INCDIR=$(GMOCK_DIR)/include

ifeq ($(shell command -v curl >/dev/null 2>&1 || echo FAIL),)
download = curl -f -L -- "$(1)" > "$@.tmp" && touch $@.tmp && mv $@.tmp $@
else ifeq ($(shell command -v wget >/dev/null 2>&1 || echo FAIL),)
download = rm -f $@.tmp && \
	wget --passive -c -p -O $@.tmp "$(1)" && \
	touch $@.tmp && \
	mv $@.tmp $@
else
download = $(error Neither curl nor wget found!)
endif

ifeq ($(shell command -v sha512sum >/dev/null 2>&1 || echo FAIL),)
SHA512SUM = sha512sum -c
else ifeq ($(shell command -v shasum >/dev/null 2>&1 || echo FAIL),)
SHA512SUM = shasum -a 512 --check
else ifeq ($(shell command -v openssl >/dev/null 2>&1 || echo FAIL),)
SHA512SUM = openssl dgst -sha512
else
SHA512SUM = $(error SHA-512 checksumming not found!)
endif

download_pkg = \
	$(call download,$(1)) \
	&& grep $(@) SHA512SUMS| $(SHA512SUM)

UNPACK_DIR = $(patsubst %.tar,%,$(basename $(notdir $<)))
MOVE = mv $(UNPACK_DIR) $(@) && touch $(@)

all: test

googletest-release-$(GTEST_VERSION).zip:
	$(call download_pkg,$(GTEST_URL),googletest)

googletest: googletest-release-$(GTEST_VERSION).zip
	$(RM) -R $(@) && unzip $<
	(cd $(UNPACK_DIR) && autoreconf -fiv)
	$(MOVE)

include Makefile.testlibs

.cpp$(OBJSUFFIX):
	$(CXX) $(CXXSTD) -isystem $(GTEST_INCDIR) -I$(GTEST_DIR) $(CXXFLAGS) -c $< -o $@

.cc$(OBJSUFFIX):
	$(CXX) $(CXXSTD) -isystem $(GTEST_INCDIR) -I$(GTEST_DIR) $(CXXFLAGS) -c $< -o $@

test_%: test_%.cpp $(TOPSRC)/%.cpp libgtest$(LIBSUFFIX)
	@$(CXX) $(CXXSTD) -isystem $(GTEST_INCDIR) -I$(TOPSRC) $(CXXFLAGS) $^ -o $@
	./$@

test_BaseReader: test_BaseReader.cpp libgtest$(LIBSUFFIX)
	@$(CXX) $(CXXSTD) -isystem $(GTEST_INCDIR) -I$(TOPSRC) $(CXXFLAGS) $^ -o $@
	./$@

# TODO only when using internal bzlib?
EXTLIB_INCDIR = $(TOPSRC)/extlib/include

# Debian does not supply pkg-config data in libbz2-dev (what the hell?!)
LIBS_bz2 = -lbz2
	
LIBS_gmock = libgmock$(LIBSUFFIX) libgtest$(LIBSUFFIX)

test_DirectReader: test_DirectReader.cpp $(TOPSRC)/DirectReader.cpp $(TOPSRC)/sjis2utf16.cpp $(LIBS_gmock)
	@$(CXX) $(CXXSTD) -isystem $(GTEST_INCDIR) -isystem $(GMOCK_INCDIR) -I$(TOPSRC) -I$(EXTLIB_INCDIR) $(CXXFLAGS) $^ $(LIBS_bz2) -o $@
	./$@

test_NsaReader: test_NsaReader.cpp $(TOPSRC)/NsaReader.cpp $(TOPSRC)/SarReader.cpp $(TOPSRC)/DirectReader.cpp $(TOPSRC)/DirPaths.cpp $(TOPSRC)/sjis2utf16.cpp $(LIBS_gmock)
	@$(CXX) $(CXXSTD) -isystem $(GTEST_INCDIR) -isystem $(GMOCK_INCDIR) -I$(TOPSRC) -I$(EXTLIB_INCDIR) $(CXXFLAGS) $^ $(LIBS_bz2) -o $@
	./$@

test_SarReader: test_SarReader.cpp $(TOPSRC)/SarReader.cpp $(TOPSRC)/DirectReader.cpp $(TOPSRC)/sjis2utf16.cpp $(LIBS_gmock)
	@$(CXX) $(CXXSTD) -isystem $(GTEST_INCDIR) -isystem $(GMOCK_INCDIR) -I$(TOPSRC) -I$(EXTLIB_INCDIR) $(CXXFLAGS) $^ $(LIBS_bz2) -o $@
	./$@

# TODO abstract out the dependency on 'sjis2utf16.cpp'
test_Encoding: test_Encoding.cpp $(TOPSRC)/Encoding.cpp $(TOPSRC)/sjis2utf16.cpp libgtest$(LIBSUFFIX)
	@$(CXX) $(CXXSTD) -isystem $(GTEST_INCDIR) -I$(TOPSRC) $(CXXFLAGS) $^ -o $@
	./$@
# TODO test_graphics_routines{,MMX,SSE2,AltiVec} (requires some fancy footwork & pivots for CPU type)
# TODO test_{DirtyRect,FontInfo,ScriptHandler} needs SDL
TESTEXE := test_BaseReader test_DirPaths test_DirectReader test_Encoding test_NsaReader test_SarReader

test: $(TESTEXE)

CLEAN_PKG += googletest
DISTCLEAN_PKG += googletest-release-$(GTEST_VERSION).zip

clean::
	$(RM) $(CLEAN_OBJ) $(TESTEXE)
	$(RM) -R $(CLEAN_PKG)

distclean:: clean
	$(RM) $(DISTCLEAN_PKG)

.PHONY: all clean distclean test $(TESTEXE)

.DELETE_ON_ERROR:
