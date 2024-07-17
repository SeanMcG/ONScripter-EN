#include <iostream>

#include "SarReader.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

class MockPathProvider : public PathProvider
{
public:
  MOCK_CONST_METHOD1(get_path, const char *(int));
  MOCK_CONST_METHOD0(get_all_paths, const char *());
  MOCK_CONST_METHOD0(get_num_paths, int());
  MOCK_CONST_METHOD0(max_path_len, size_t());
};

extern bool hasTwoByteChar(const unsigned char *);

namespace {

TEST (SarReaderTest, Constructor) {
  const unsigned char* str = NULL;
  MockPathProvider provider;
  SarReader dr(provider, str);
}

TEST (SarReaderTest, ConstructorWithKeyTable) {
  const unsigned char* str = (const unsigned char *)"";
  MockPathProvider provider;
  SarReader dr(provider, str);
}

TEST (SarReaderTest, OpenAndClose) {
  const unsigned char* str = NULL;
  MockPathProvider provider;
  SarReader dr(provider, str);
  ASSERT_EQ(-1, dr.open());
  ASSERT_EQ(0u, dr.close());
}

TEST (SarReaderTest, getArchiveName) {
  const unsigned char* str = NULL;
  MockPathProvider provider;
  SarReader dr(provider, str);
  ASSERT_STREQ("sar", dr.getArchiveName());
}

TEST (SarReaderTest, getNumFiles) {
  const unsigned char* str = NULL;
  MockPathProvider provider;
  SarReader dr(provider, str);
  ASSERT_EQ(0u, dr.getNumFiles());
}

TEST (SarReaderTest, registerCompressionType) {
  const unsigned char* str = NULL;
  MockPathProvider provider;
  SarReader dr(provider, str);
  dr.registerCompressionType("", 0);
}

TEST (SarReaderTest, getFileByIndex) {
  const unsigned char* str = NULL;
  MockPathProvider provider;
  SarReader dr(provider, str);
  struct SarReader::FileInfo fInfo = dr.getFileByIndex(0);
  ASSERT_EQ(0u, fInfo.compression_type);
  ASSERT_EQ(0u, fInfo.length);
  ASSERT_EQ(0u, fInfo.offset);
  ASSERT_EQ(0u, fInfo.original_length);
}

TEST (SarReaderTest, getFileLength) {
  const unsigned char* str = NULL;
  MockPathProvider provider;
  EXPECT_CALL(provider, max_path_len()).Times(1);
  EXPECT_CALL(provider, get_num_paths()).Times(2);
  SarReader dr(provider, str);
  ASSERT_EQ(0u, dr.getFileLength(""));
}

TEST (SarReaderTest, DISABLED_getFileLengthWithTempFile) {
  const int offset = 0;
  const unsigned char* str = NULL;
  MockPathProvider provider;
  EXPECT_CALL(provider, max_path_len()).Times(1);
  EXPECT_CALL(provider, get_num_paths()).Times(2);
  SarReader dr(provider, str);
  ASSERT_EQ(1264u, dr.getFileLength("./test/README")); // TODO fix
}

TEST (SarReaderTest, getFile) {
  const unsigned char* str = NULL;
  MockPathProvider provider;
  EXPECT_CALL(provider, max_path_len()).Times(1);
  EXPECT_CALL(provider, get_num_paths()).Times(2);
  SarReader dr(provider, str);
  unsigned char* buf = new unsigned char[256];
  int loc;
  ASSERT_EQ(0u, dr.getFile("", buf, &loc));
  ASSERT_EQ(1, loc); // TODO fix
  delete[] buf;
}

TEST (SarReaderTest, convertFromSJISToEUC) {
  const unsigned char* str = NULL;
  MockPathProvider provider;
  SarReader dr(provider, str);
  char *sjis_no = new char[3];
  sjis_no[0] = 0x82;
  sjis_no[1] = 0xCC;
  sjis_no[2] = 0x0;
  dr.convertFromSJISToEUC(sjis_no);
  ASSERT_STREQ("\xA4\xCE", sjis_no);
  delete[] sjis_no;
}

TEST (SarReaderTest, convertFromSJISToUTF8) {
  const unsigned char* str = NULL;
  MockPathProvider provider;
  SarReader dr(provider, str);
  char *dst = new char[2];
  char *sjis_no = new char[3];
  sjis_no[0] = 0x82;
  sjis_no[1] = 0xCC;
  sjis_no[2] = 0x0;
  dr.convertFromSJISToUTF8(dst, sjis_no);
  ASSERT_STREQ("\xE3\x81\xAE", dst);
  delete[] sjis_no;
  delete[] dst;
}

TEST (SarReaderTest, hasTwoByteChar) {
  unsigned char *str = new unsigned char[3];
  str[0] = 0x79;
  str[1] = 0x0;
  str[2] = 0x0; // NOTE 3rd byte MUST be a NULL!
  ASSERT_FALSE(::hasTwoByteChar(str));
  delete[] str;
  str = new unsigned char[3];
  str[0] = 0x80;
  str[1] = 0x0;
  str[2] = 0x0;
  ASSERT_TRUE(::hasTwoByteChar(str));
  delete[] str;
  str = new unsigned char[3];
  str[0] = 0xE0;
  str[1] = 0x0;
  str[2] = 0x0;
  ASSERT_TRUE(::hasTwoByteChar(str));
  delete[] str;
}

TEST (SarReaderTest, hasTwoByteCharWithEmpty) {
  unsigned char* str = new unsigned char[1];
  str[0] = 0x0;
  delete[] str;
}

TEST (SarReaderTest, hasTwoByteCharWithNull) {
  const unsigned char* str = NULL;
  ASSERT_FALSE(::hasTwoByteChar(str));
}

} // namespace
