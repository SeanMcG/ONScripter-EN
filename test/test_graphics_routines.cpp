#include "graphics_cpu.h"
#include "graphics_sum.h"
#if 0 // requires SDL
#include "graphics_blend.h"
#include "graphics_resize.h"
#endif
#include "resize_image.h"

#include "gtest/gtest.h"

namespace {

TEST(GraphicsRoutinesTest, ons_gfx_imageFilterMean) {
  const int len = 0;
  unsigned char* src1 = NULL;
  unsigned char* src2 = NULL;
  unsigned char* dst = NULL;
  ons_gfx::imageFilterMean(src1, src2, dst, len);
}

} // namespace
