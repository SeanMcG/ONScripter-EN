#include "ScriptHandler.h"

#include "Reporter.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

class MockReporter : public Reporter {
public:
  MOCK_CONST_METHOD4(reportError, bool(const char *, const char *, bool, bool));
};

namespace {

TEST (ScriptHandlerTest, Constructor) {
  MockReporter reporter;
  EXPECT_CALL(reporter, reportError(testing::NotNull(), testing::NotNull(), testing::IsFalse(), testing::IsFalse())).Times(0);
  ScriptHandler handler = ScriptHandler();
  handler.setReporter(&reporter);
  ASSERT_EQ(ScriptHandler::CMD_NONE, handler.current_cmd_type);
  ASSERT_EQ(ScriptHandler::CMD_NONE, handler.gosub_cmd_type);
  handler.setReporter(NULL);
}

TEST (ScriptHandlerTest, reset) {
  MockReporter reporter;
  EXPECT_CALL(reporter, reportError(testing::NotNull(), testing::NotNull(), testing::IsFalse(), testing::IsFalse())).Times(0);
  ScriptHandler handler = ScriptHandler();
  handler.setReporter(&reporter);
  handler.reset();
  ASSERT_EQ(ScriptHandler::CMD_NONE, handler.current_cmd_type);
  ASSERT_EQ(ScriptHandler::CMD_NONE, handler.gosub_cmd_type);
  handler.setReporter(NULL);
}

TEST (ScriptHandlerTest, fopenWithNullsAndBooleans) {
  MockReporter reporter;
  FILE *fp = NULL;
  EXPECT_CALL(reporter, reportError(testing::NotNull(), testing::NotNull(), testing::IsFalse(), testing::IsFalse())).Times(0);
  ScriptHandler handler = ScriptHandler();
  handler.setReporter(&reporter);
  fp = handler.fopen((char *)NULL, (char *) NULL, false, false);
  ASSERT_EQ((FILE *)NULL, fp);
  handler.setReporter(NULL);
}

TEST (ScriptHandlerTest, fopenWithTempFileAndBooleans) {
  MockReporter reporter;
  FILE *fp = NULL;
  const char *path = ""; // TODO fix
  const char *mode = "";
  EXPECT_CALL(reporter, reportError(testing::NotNull(), testing::NotNull(), testing::IsFalse(), testing::IsFalse())).Times(0);
  ScriptHandler handler = ScriptHandler();
  handler.setReporter(&reporter);
  fp = handler.fopen(path, mode, false, false);
//  ASSERT_NE((FILE *)NULL, fp);
  handler.setReporter(NULL);
}

// TEST (ScriptHandlerTest, fopen) {
//   MockReporter reporter;
//   EXPECT_CALL(reporter, reportError(testing::NotNull(), testing::NotNull(), testing::IsFalse(), testing::IsFalse())).Times(0);
//   ScriptHandler handler = ScriptHandler();
//   handler.setReporter(&reporter);
//   FILE *fp = handler.fopen((char *)NULL, (char *) NULL, (char *) NULL);
// TODO: assert 'fp' == NULL
//   handler.setReporter(NULL);
// }

// TEST (ScriptHandlerTest, readToken) {
//   MockReporter reporter;
//   EXPECT_CALL(reporter, reportError(testing::NotNull(), testing::NotNull(), testing::IsFalse(), testing::IsFalse())).Times(0);
//   ScriptHandler handler = ScriptHandler();
//   handler.setReporter(&reporter);
//   const char *token = handler.readToken(false);
//   // assert non null 'token'
//   handler.setReporter(NULL);
// }

} // namespace
