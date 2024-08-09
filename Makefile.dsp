# -*- makefile-gmake -*-
#
# Makefile.dsp - General makefile rules for ONScripter DSP acceleration
#

#if USE_X86_GFX

graphics_sse2.o: graphics_sse2.cpp graphics_sse2.h graphics_common.h graphics_sum.h graphics_blend.h
	$(CXX) $(CXXSTD) $(OSCFLAGS) $(INCS) $(DEPFLAGS) $(DEFS) $GFX_SSE2_FLAGS -c $< -o $@

graphics_mmx.o: graphics_mmx.cpp graphics_mmx.h graphics_common.h graphics_sum.h
	$(CXX) $(CXXSTD) $(OSCFLAGS) $(INCS) $(DEPFLAGS) $(DEFS) $GFX_MMX_FLAGS -c $< -o $@

#endif

#if USE_PPC_GFX

graphics_altivec.o: graphics_altivec.cpp graphics_altivec.h graphics_common.h graphics_sum.h
	$(CXX) $(CXXSTD) $(OSCFLAGS) $(INCS) $(DEPFLAGS) $(DEFS) $GFX_MALTIVEC_FLAGS -c $< -o $@

#endif
