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

#define MAX_PATH_LEN (int)128
#define PATH_COUNT (int)1
#define FNAME_MAX (int)256

class SarFile : public ::testing::Test
{
  protected:
    MockPathProvider provider;
    char *tmp;
    char *fName;
    FILE *file;

  void SetUp() {
    char *saved_cwd = new char[PATH_MAX];
    ::getcwd(saved_cwd, PATH_MAX - 1);
    char *tmpl = new char[256];
    tmpl[0] = '\0'; // ensure 'tmpl' is an empty string
    tmp = ::getenv("TMPDIR");
    if (tmp != NULL) {
      ::strcpy(tmpl, tmp);
      ::strncat(tmpl, "/", 1);
    } else {
      ::strcpy(tmpl, "/var/tmp/");
      tmp = new char[strlen(tmpl) + 1];
      tmp[0] = '\0'; // ensure 'tmp' is an empty string
      ::strncpy(tmp, tmpl, strlen(tmpl));
    }
    fName = new char[FNAME_MAX];
    ::memset(fName, 0, FNAME_MAX);
    ::strncpy(fName, "SarReader_test_tmpXXXXXX", 28);
    delete[] tmpl;

    ::chdir(tmp);
    int fd = ::mkstemp(fName);
    if (fd) {
      file = ::fdopen(fd, "w");
      if (file) {
        ::fprintf(file, "%s\n", "abc123");
      } else {
        std::cerr << "File descriptor" << fd << " could not be converted to a FILE* : errno=" << errno << std::endl;
        ::close(fd);
      }
    } else {
      std::cerr << "File " << tmp << "/" << fName << " was not created : errno=" << errno << std::endl;
    }
    ::chdir(saved_cwd);
  }
    
  void TearDown() {
    if (file) {
      if (::fclose(file) != 0) {
        std::cerr << "File " << tmp << "/" << fName << " was not closed : errno=" << errno << std::endl;
      }
    }
    
    if (fName) {
      char *saved_cwd = new char[PATH_MAX];
      ::getcwd(saved_cwd, PATH_MAX - 1);
      ::chdir(tmp);
      if (::unlink(fName) != 0) {
        std::cerr << "File " << tmp << "/" << fName << " was not removed : errno=" << errno << std::endl;
      }
      delete[] fName;
      ::chdir(saved_cwd);
    }
  }
};

TEST (SarReaderTest, OpenAndClose) {
  MockPathProvider provider;
  EXPECT_CALL(provider, max_path_len()).Times(testing::AtLeast(1)).WillRepeatedly(testing::Return(MAX_PATH_LEN));
  EXPECT_CALL(provider, get_num_paths()).Times(testing::AtLeast(1)).WillRepeatedly(testing::Return(PATH_COUNT));
  EXPECT_CALL(provider, get_path(testing::Le(PATH_COUNT))).Times(testing::AtLeast(1)).WillRepeatedly(testing::Return(::getenv("TMPDIR")));
  SarReader sr(provider, NULL);
  ASSERT_EQ(-1, sr.open()); // no filename should return -1
  ASSERT_EQ(-1, sr.open("")); // empty filename should return -1
  ASSERT_EQ(-1, sr.open("BLAH")); // non-existent filename should return -1
  ASSERT_EQ(0u, sr.close());
}

TEST_F (SarFile, OpenAndCloseInvalidSar) {
  EXPECT_CALL(provider, max_path_len()).Times(testing::AtLeast(1)).WillRepeatedly(testing::Return(MAX_PATH_LEN));
  EXPECT_CALL(provider, get_num_paths()).Times(testing::AtLeast(1)).WillRepeatedly(testing::Return(PATH_COUNT));
  EXPECT_CALL(provider, get_path(testing::Le(PATH_COUNT))).Times(testing::AtLeast(1)).WillRepeatedly(testing::Return(tmp));
  SarReader sr(provider, NULL);
  ASSERT_EQ(-1, sr.open(fName)); // existing but invalid SAR file should return -1
  ASSERT_EQ(0u, sr.close());
}

TEST_F (SarFile, OpenAndCloseRealSar) {
  char *path = new char[PATH_MAX];
  ::getcwd(path, PATH_MAX - 1);
  EXPECT_CALL(provider, max_path_len()).Times(testing::AtLeast(1)).WillRepeatedly(testing::Return(MAX_PATH_LEN));
  EXPECT_CALL(provider, get_num_paths()).Times(testing::AtLeast(1)).WillRepeatedly(testing::Return(PATH_COUNT));
  EXPECT_CALL(provider, get_path(testing::Le(PATH_COUNT))).Times(testing::AtLeast(1)).WillRepeatedly(testing::Return(tmp));
  SarReader sr(provider, NULL);
  sprintf(path, "%s%s%s", path, "/", "arc.sar"); /* NOTE path separator of '/' is incorrect on Windows */
  ASSERT_EQ(1u, sr.open(path));
  ASSERT_EQ(0u, sr.close());
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

#define MAX_PATH_LEN (int)128
#define PATH_COUNT (int)1

TEST (SarReaderTest, getFileLengthWithNonexistentFile) {
  const unsigned char* str = NULL;
  MockPathProvider provider;
  SarReader dr(provider, str);

  size_t length = dr.getFileLength("");

  ASSERT_EQ(0u, length);
}

TEST (SarReaderTest, getFileLengthWithNull) {
  const unsigned char* str = NULL;
  MockPathProvider provider;
  SarReader dr(provider, str);

  size_t length = dr.getFileLength(NULL);

  ASSERT_EQ(0u, length);
}

TEST_F (SarFile, getFileLengthWithRealSar) {
  EXPECT_CALL(provider, max_path_len()).Times(testing::AtLeast(1)).WillRepeatedly(testing::Return(MAX_PATH_LEN));
  EXPECT_CALL(provider, get_num_paths()).Times(testing::AtLeast(1)).WillRepeatedly(testing::Return(PATH_COUNT));
  EXPECT_CALL(provider, get_path(testing::Le(PATH_COUNT))).Times(testing::AtLeast(1)).WillRepeatedly(testing::Return("test/"));
  SarReader dr(provider, NULL);
  ASSERT_EQ(7u, dr.getFileLength("test/arc.sar"));
}

TEST_F (SarFile, getFileWithRealSar) {
  EXPECT_CALL(provider, max_path_len()).Times(testing::AtLeast(1)).WillRepeatedly(testing::Return(MAX_PATH_LEN));
  EXPECT_CALL(provider, get_num_paths()).Times(testing::AtLeast(1)).WillRepeatedly(testing::Return(PATH_COUNT));
  EXPECT_CALL(provider, get_path(testing::Le(PATH_COUNT))).Times(testing::AtLeast(1)).WillRepeatedly(testing::Return(tmp));
  SarReader dr(provider, NULL);
  unsigned char* buf = new unsigned char[256];
  int loc = 0;
  ASSERT_EQ(0u, dr.getFile(fName, buf, &loc));
  delete[] buf;
  ASSERT_EQ(0u, loc);
}

TEST (SarReaderTest, getFileWithEmptyFileName) {
  const unsigned char* str = NULL;
  MockPathProvider provider;
  SarReader dr(provider, str);
  unsigned char* buf = new unsigned char[256];
  int loc;

  size_t size = dr.getFile("", buf, &loc);

  delete[] buf;

  ASSERT_EQ(0u, size);
  ASSERT_EQ(BaseReader::ARCHIVE_TYPE_NONE, loc);
}

TEST (SarReaderTest, getFileWithNull) {
  const unsigned char* str = NULL;
  MockPathProvider provider;
  SarReader dr(provider, str);
  ASSERT_EQ(0u, dr.getFile(NULL, NULL, NULL));
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
  ASSERT_FALSE(::hasTwoByteChar(str));
  delete[] str;
}

TEST (SarReaderTest, hasTwoByteCharWithNull) {
  const unsigned char* str = NULL;
  ASSERT_FALSE(::hasTwoByteChar(str));
}

} // namespace
