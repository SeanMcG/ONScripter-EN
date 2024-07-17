#include <iostream>

#include "NsaReader.h"

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

TEST (NsaReaderTest, Constructor) {
  const int offset = 0;
  const unsigned char* str = NULL;
  MockPathProvider provider;
  NsaReader dr(provider, offset, str);
}

TEST (NsaReaderTest, ConstructorWithKeyTable) {
  const int offset = 0;
  const unsigned char* str = (const unsigned char *)"";
  MockPathProvider provider;
  NsaReader dr(provider, offset, str);
}

TEST (NsaReaderTest, OpenAndClose) {
  const int offset = 0;
  const unsigned char* str = NULL;
  MockPathProvider provider;
  EXPECT_CALL(provider, max_path_len()).Times(1);
  EXPECT_CALL(provider, get_num_paths()).Times(4);
  NsaReader dr(provider, offset, str);
  ASSERT_EQ(0u, dr.open());
  ASSERT_EQ(0u, dr.close());
}

TEST (NsaReaderTest, getArchiveName) {
  const int offset = 0;
  const unsigned char* str = NULL;
  MockPathProvider provider;
  NsaReader dr(provider, offset, str);
  ASSERT_STREQ("nsa", dr.getArchiveName());
}

TEST (NsaReaderTest, getNumFiles) {
  const int offset = 0;
  const unsigned char* str = NULL;
  MockPathProvider provider;
  NsaReader dr(provider, offset, str);
  ASSERT_EQ(0u, dr.getNumFiles());
}

TEST (NsaReaderTest, registerCompressionType) {
  const int offset = 0;
  const unsigned char* str = NULL;
  MockPathProvider provider;
  NsaReader dr(provider, offset, str);
  dr.registerCompressionType("", 0);
}

TEST (NsaReaderTest, getFileByIndex) {
  const int offset = 0;
  const unsigned char* str = NULL;
  MockPathProvider provider;
  NsaReader dr(provider, offset, str);
  struct NsaReader::FileInfo fInfo = dr.getFileByIndex(0);
  ASSERT_EQ(0u, fInfo.compression_type);
  ASSERT_EQ(0u, fInfo.length);
  ASSERT_EQ(0u, fInfo.offset);
  ASSERT_EQ(0u, fInfo.original_length);
}

TEST (NsaReaderTest, getFileLength) {
  const int offset = 0;
  const unsigned char* str = NULL;
  MockPathProvider provider;
  EXPECT_CALL(provider, max_path_len()).Times(2);
  EXPECT_CALL(provider, get_num_paths()).Times(4);
  NsaReader dr(provider, offset, str);
  ASSERT_EQ(0u, dr.getFileLength(""));
}

TEST (NsaReaderTest, DISABLED_getFileLengthWithTempFile) {
  const int offset = 0;
  const unsigned char* str = NULL;
  MockPathProvider provider;
  EXPECT_CALL(provider, max_path_len()).Times(1);
  EXPECT_CALL(provider, get_num_paths()).Times(2);
  NsaReader dr(provider, offset, str);
  ASSERT_EQ(1264u, dr.getFileLength("./test/README")); // TODO fix
}

TEST (NsaReaderTest, getFile) {
  const int offset = 0;
  const unsigned char* str = NULL;
  MockPathProvider provider;
  EXPECT_CALL(provider, max_path_len()).Times(2);
  EXPECT_CALL(provider, get_num_paths()).Times(4);
  NsaReader dr(provider, offset, str);
  unsigned char* buf = new unsigned char[256];
  int loc;
  ASSERT_EQ(0u, dr.getFile("", buf, &loc));
  ASSERT_EQ(1, loc); // TODO fix
  delete[] buf;
}

TEST (NsaReaderTest, convertFromSJISToEUC) {
  const int offset = 0;
  const unsigned char* str = NULL;
  MockPathProvider provider;
  NsaReader dr(provider, offset, str);
  char *sjis_no = new char[3];
  sjis_no[0] = 0x82;
  sjis_no[1] = 0xCC;
  sjis_no[2] = 0x0;
  dr.convertFromSJISToEUC(sjis_no);
  ASSERT_STREQ("\xA4\xCE", sjis_no);
  delete[] sjis_no;
}

TEST (NsaReaderTest, convertFromSJISToUTF8) {
  const int offset = 0;
  const unsigned char* str = NULL;
  MockPathProvider provider;
  NsaReader dr(provider, offset, str);
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

TEST (NsaReaderTest, hasTwoByteChar) {
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

TEST (NsaReaderTest, hasTwoByteCharWithEmpty) {
  unsigned char* str = new unsigned char[1];
  str[0] = 0x0;
  ASSERT_FALSE(::hasTwoByteChar(str));
  delete[] str;
}

TEST (NsaReaderTest, hasTwoByteCharWithNull) {
  const unsigned char* str = NULL;
  ASSERT_FALSE(::hasTwoByteChar(str));
}

} // namespace
