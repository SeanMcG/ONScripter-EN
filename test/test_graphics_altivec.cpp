#include "graphics_altivec.h"

#include "gtest/gtest.h"

namespace {

TEST(GraphicsAltivecTest, ons_gfx_imageFilterMean_Altivec_null) {
  const int len = 0;
  unsigned char* src1 = NULL;
  unsigned char* src2 = NULL;
  unsigned char* dst = NULL;
  ons_gfx::imageFilterMean_Altivec(src1, src2, dst, len);
  EXPECT_EQ(NULL, dst);
}

TEST(GraphicsAltivecTest, ons_gfx_imageFilterMean_Altivec) {
  const int len = 256;
  unsigned char* src1 = new unsigned char[len];
  unsigned char* src2 = new unsigned char[len];
  unsigned char* dst = new unsigned char[len];
  ons_gfx::imageFilterMean_Altivec(src1, src2, dst, len);
  delete[] src1;
  delete[] src2;
  delete[] dst;
}

TEST(GraphicsAltivecTest, ons_gfx_imageFilterAddTo_Altivec_null) {
  const int len = 0;
  unsigned char* src = NULL;
  unsigned char* dst = NULL;
  ons_gfx::imageFilterAddTo_Altivec(dst, src, len);
  EXPECT_EQ(NULL, dst);
}

TEST(GraphicsAltivecTest, ons_gfx_imageFilterAddTo_Altivec) {
  const int len = 256;
  unsigned char* src = new unsigned char[len];
  unsigned char* dst = new unsigned char[len];
  ons_gfx::imageFilterAddTo_Altivec(dst, src, len);
  delete[] src;
  delete[] dst;
}

TEST(GraphicsAltivecTest, ons_gfx_imageFilterSubFrom_Altivec_null) {
  const int len = 0;
  unsigned char* src = NULL;
  unsigned char* dst = NULL;
  ons_gfx::imageFilterSubFrom_Altivec(dst, src, len);
  EXPECT_EQ(NULL, dst);
}

TEST(GraphicsAltivecTest, ons_gfx_imageFilterSubFrom_Altivec) {
  const int len = 256;
  unsigned char* src = new unsigned char[len];
  unsigned char* dst = new unsigned char[len];
  ons_gfx::imageFilterSubFrom_Altivec(dst, src, len);
  delete[] src;
  delete[] dst;
}

} // namespace
