#include "DirtyRect.h"

#include "gtest/gtest.h"

namespace {

TEST (DirtyRectTest, DefaultConstructor) {
  const DirtyRect rect;
  EXPECT_EQ(0u, rect.screen_width);
  EXPECT_EQ(0u, rect.screen_height);
  EXPECT_EQ(0u, rect.bounding_box.w);
  EXPECT_EQ(0u, rect.bounding_box.h);
}

TEST (DirtyRectTest, CopyConstructor) {
  const DirtyRect rect;
  const DirtyRect r2 = rect;
  EXPECT_EQ(rect.screen_width, r2.screen_width);
  EXPECT_EQ(rect.screen_height, r2.screen_height);
  EXPECT_EQ(rect.bounding_box.w, r2.bounding_box.w);
  EXPECT_EQ(rect.bounding_box.h, r2.bounding_box.h);
}

TEST (DirtyRectTest, OperatorEquals) {
  DirtyRect rect;
  DirtyRect const r2;
  rect = r2;
  EXPECT_EQ(rect.screen_width, r2.screen_width);
  EXPECT_EQ(rect.screen_height, r2.screen_height);
  EXPECT_EQ(rect.bounding_box.w, r2.bounding_box.w);
  EXPECT_EQ(rect.bounding_box.h, r2.bounding_box.h);
}

TEST (DirtyRectTest, rectStartsEmpty) {
  const DirtyRect rect;
  EXPECT_EQ(0u, rect.screen_width);
  EXPECT_EQ(0u, rect.screen_height);
}

TEST (DirtyRectTest, SDLRectStartsEmpty) {
  const DirtyRect rect;
  EXPECT_EQ(0u, rect.bounding_box.w);
  EXPECT_EQ(0u, rect.bounding_box.h);
}

TEST (DirtyRectTest, setDimension) {
  DirtyRect rect;
  rect.setDimension(64, 64);
  EXPECT_EQ(64, rect.screen_width);
  EXPECT_EQ(64, rect.screen_height);
}

TEST (DirtyRectTest, add) {
  DirtyRect rect;
  rect.screen_width = 128;
  rect.screen_height = 128;
  SDL_Rect sdlr;
  sdlr.x = 64;
  sdlr.y = 64;
  sdlr.w = 64;
  sdlr.h = 64;
  rect.add(sdlr);
  EXPECT_EQ(128, rect.screen_width);
  EXPECT_EQ(128, rect.screen_height);
  EXPECT_EQ(64, rect.bounding_box.w);
  EXPECT_EQ(64, rect.bounding_box.h);
}

TEST (DirtyRectTest, addWithNegativeXandY) {
  DirtyRect rect;
  rect.screen_width = 128;
  rect.screen_height = 128;
  SDL_Rect sdlr;
  sdlr.x = -64;
  sdlr.y = -64;
  sdlr.w = 64;
  sdlr.h = 64;
  rect.add(sdlr);
  EXPECT_EQ(128, rect.screen_width);
  EXPECT_EQ(128, rect.screen_height);
  EXPECT_EQ(0, rect.bounding_box.w);
  EXPECT_EQ(0, rect.bounding_box.h);
}

TEST (DirtyRectTest, calcBoundingBox) {
  DirtyRect rect;
  SDL_Rect sdlr1;
  sdlr1.x = 64;
  sdlr1.y = 64;
  sdlr1.w = 64;
  sdlr1.h = 64;
  SDL_Rect sdlr2;
  sdlr2.x = 64;
  sdlr2.y = 64;
  sdlr2.w = 64;
  sdlr2.h = 64;
  SDL_Rect sdlr = rect.calcBoundingBox(sdlr1, sdlr2);
  EXPECT_EQ(64, sdlr.x);
  EXPECT_EQ(64, sdlr.y);
  EXPECT_EQ(64, sdlr.w);
  EXPECT_EQ(64, sdlr.h);
}

TEST (DirtyRectTest, clear) {
  DirtyRect rect;
  rect.bounding_box.w = 64;
  rect.bounding_box.h = 64;
  rect.clear();
  EXPECT_EQ(0u, rect.bounding_box.w);
  EXPECT_EQ(0u, rect.bounding_box.h);
}

TEST (DirtyRectTest, fill) {
  DirtyRect rect;
  rect.fill(64, 64);
  EXPECT_EQ(0u, rect.bounding_box.x);
  EXPECT_EQ(0u, rect.bounding_box.y);
  EXPECT_EQ(64, rect.bounding_box.w);
  EXPECT_EQ(64, rect.bounding_box.h);
}

} // namespace
