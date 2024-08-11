#include "FontInfo.h"

#include "gtest/gtest.h"

namespace {

TEST (TestFontInfo, DefaultConstructor) {
  Fontinfo info;
  EXPECT_EQ(Fontinfo::YOKO_MODE, info.tateyoko_mode);
}

} // namespace
