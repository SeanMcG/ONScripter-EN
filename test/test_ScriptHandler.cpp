#include "ScriptHandler.h"

#include "gtest/gtest.h"

namespace {

TEST (ScriptHandlerTest, Constructor) {
  ScriptHandler handler = ScriptHandler();
  ASSERT_EQ(ScriptHandler::CMD_NONE, handler.current_cmd_type);
}

} // namespace
