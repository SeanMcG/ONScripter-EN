# -*- makefile-gmake -*-
#
# test/Makefile
# borrowed and slightly modified from VLC
#

GTEST_VERSION=1.8.1
GTEST_URL=https://github.com/google/googletest/archive/release-$(GTEST_VERSION).zip

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

googletest-release-$(GTEST_VERSION).zip:
	$(call download_pkg,$(GTEST_URL),googletest)

googletest: googletest-release-$(GTEST_VERSION).zip
	$(RM) -R $(@) && unzip $<
	(cd $(UNPACK_DIR) && autoreconf -fiv)
	$(MOVE)

CLEAN_PKG += googletest
DISTCLEAN_PKG += googletest-release-$(GTEST_VERSION).zip

all: googletest

clean::
	$(RM) -R $(CLEAN_PKG)

distclean:: clean
	$(RM) $(DISTCLEAN_PKG)

.PHONY: all clean distclean

.DELETE_ON_ERROR:
