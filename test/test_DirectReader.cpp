#include <iostream>

#include "DirectReader.h"

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

TEST (DirectReaderTest, Constructor) {
  const unsigned char* str = NULL;
  MockPathProvider provider;
  DirectReader dr(provider, str);
}

TEST (DirectReaderTest, ConstructorWithKeyTable) {
  const unsigned char* str = (const unsigned char *)"";
  MockPathProvider provider;
  DirectReader dr(provider, str);
}

TEST (DirectReaderTest, OpenAndClose) {
  const unsigned char* str = NULL;
  MockPathProvider provider;
  DirectReader dr(provider, str);
  ASSERT_EQ(0u, dr.open());
  ASSERT_EQ(0u, dr.close());
}

TEST (DirectReaderTest, getArchiveName) {
  const unsigned char* str = NULL;
  MockPathProvider provider;
  DirectReader dr(provider, str);
  ASSERT_STREQ("direct", dr.getArchiveName());
}

TEST (DirectReaderTest, getNumFiles) {
  const unsigned char* str = NULL;
  MockPathProvider provider;
  DirectReader dr(provider, str);
  ASSERT_EQ(0u, dr.getNumFiles());
}

TEST (DirectReaderTest, registerCompressionType) {
  const unsigned char* str = NULL;
  MockPathProvider provider;
  DirectReader dr(provider, str);
  dr.registerCompressionType("", 0);
  dr.registerCompressionType("nsa", 0);
}

TEST (DirectReaderTest, getFileByIndex) {
  const unsigned char* str = NULL;
  MockPathProvider provider;
  DirectReader dr(provider, str);
  struct DirectReader::FileInfo fInfo = dr.getFileByIndex(0);
  ASSERT_EQ(0u, fInfo.compression_type);
  ASSERT_EQ(0u, fInfo.length);
  ASSERT_EQ(0u, fInfo.offset);
  ASSERT_EQ(0u, fInfo.original_length);
}

TEST (DirectReaderTest, getFileLength) {
  const unsigned char* str = NULL;
  MockPathProvider provider;
  EXPECT_CALL(provider, max_path_len()).Times(1);
  EXPECT_CALL(provider, get_num_paths()).Times(2);
  DirectReader dr(provider, str);
  ASSERT_EQ(0u, dr.getFileLength(""));
}

TEST (DirectReaderTest, DISABLED_getFileLengthWithTempFile) {
  const unsigned char* str = NULL;
  MockPathProvider provider;
  EXPECT_CALL(provider, max_path_len()).Times(1);
  EXPECT_CALL(provider, get_num_paths()).Times(2);
  DirectReader dr(provider, str);
  ASSERT_EQ(1264u, dr.getFileLength("./test/README")); // TODO fix
}

TEST (DirectReaderTest, getFile) {
  const unsigned char* str = new unsigned char[256];
//  const char *tmpdir = ::getenv("TEMP");
  char *tmpl = new char[256];
  ::strcpy(tmpl, "/tmp/directreader_test.XXXXXX");
  char *fName = new char[256];
  ::memset(fName, 0, ::strlen(fName));
  ::strncpy(fName, tmpl, ::strlen(tmpl));
  delete[] tmpl;
  MockPathProvider provider;
  EXPECT_CALL(provider, max_path_len()).Times(1);
  EXPECT_CALL(provider, get_num_paths()).Times(2);
  DirectReader dr(provider, str);
  unsigned char* buf = new unsigned char[256];
  int loc = 0;
  int fd = ::mkstemp(fName);
  if (!fd) {
    std::cerr << "File " << fName << " was not created : errno=" << errno << std::endl;
  }
  ASSERT_EQ(0u, dr.getFile(fName, buf, &loc));
  ASSERT_EQ(0u, loc);
  int result = ::close(fd);
  if (result != 0) {
    std::cerr << "File " << fName << " was not closed : errno=" << errno << std::endl;
  }
  result = ::unlink(fName);
  if (result != 0) {
    std::cerr << "File " << fName << " was not removed : errno=" << errno << std::endl;
  }
  delete[] buf;
  delete[] fName;
  delete[] str;
}

TEST (DirectReaderTest, getFileWithEmptyFileName) {
  const unsigned char* str = NULL;
  MockPathProvider provider;
  EXPECT_CALL(provider, max_path_len()).Times(1);
  EXPECT_CALL(provider, get_num_paths()).Times(2);
  DirectReader dr(provider, str);
  unsigned char* buf = new unsigned char[256];
  int loc;
  ASSERT_EQ(0u, dr.getFile("", buf, &loc));
  ASSERT_EQ(0u, loc);
  delete[] buf;
}

TEST (DirectReaderTest, getFileWithNull) {
  const unsigned char* str = NULL;
  MockPathProvider provider;
  DirectReader dr(provider, str);
  ASSERT_EQ(0u, dr.getFile(NULL, NULL, NULL));
}

TEST (DirectReaderTest, convertFromSJISToEUC) {
  const unsigned char* str = NULL;
  MockPathProvider provider;
  DirectReader dr(provider, str);
  char *sjis_no = new char[3];
  sjis_no[0] = 0x82;
  sjis_no[1] = 0xCC;
  sjis_no[2] = 0x0;
  dr.convertFromSJISToEUC(sjis_no);
  ASSERT_STREQ("\xA4\xCE", sjis_no);
  delete[] sjis_no;
}

TEST (DirectReaderTest, convertFromSJISToUTF8) {
  const unsigned char* str = NULL;
  MockPathProvider provider;
  DirectReader dr(provider, str);
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

TEST (DirectReaderTest, hasTwoByteChar) {
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

TEST (DirectReaderTest, hasTwoByteCharWithEmpty) {
  unsigned char* str = new unsigned char[1];
  str[0] = 0x0;
  ASSERT_FALSE(::hasTwoByteChar(str));
  delete[] str;
}

TEST (DirectReaderTest, hasTwoByteCharWithNull) {
  const unsigned char* str = NULL;
  ASSERT_FALSE(::hasTwoByteChar(str));
}

} // namespace
