//$Id:$ -*- C++ -*-
/*
 *  DirectReader.h - Reader from independent files for ONScripter-EN
 *
 *  Copyright (c) 2001-2010 Ogapee. All rights reserved.
 *  (original ONScripter, of which this is a fork).
 *
 *  ogapee@aqua.dti2.ne.jp
 *
 *  Copyright (c) 2007-2010 "Uncle" Mion Sonozaki
 *
 *  UncleMion@gmail.com
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, see <http://www.gnu.org/licenses/>
 *  or write to the Free Software Foundation, Inc.,
 *  59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __DIRECT_READER_H__
#define __DIRECT_READER_H__

#include "BaseReader.h"
#include "DirPaths.h"
#include <string.h>

#define MAX_FILE_NAME_LENGTH 256

class DirectReader : public BaseReader
{
public:
    DirectReader( PathProvider &provider, const unsigned char *key_table=NULL );
    ~DirectReader();

    int open( const char *name=NULL );
    int close();

    const char *getArchiveName() const;
    int getNumFiles();
    void registerCompressionType( const char *ext, int type );

    struct FileInfo getFileByIndex( unsigned int index );
    //file_name parameter is assumed to use SJIS encoding
    size_t getFileLength( const char *file_name );
    size_t getFile( const char *file_name, unsigned char *buffer, int *location=NULL );

    static void convertFromSJISToEUC( char *buf );
    static void convertFromSJISToUTF8( char *dst_buf, const char *src_buf );
    
protected:
    char *file_full_path;
    char *file_sub_path;
    size_t file_path_len;
    char *capital_name;
    char *capital_name_tmp;

    PathProvider *archive_path;
    unsigned char key_table[256];
    bool key_table_flag;
    int  getbit_mask;
    size_t getbit_len, getbit_count;
    unsigned char *read_buf;
    unsigned char *decomp_buffer;
    size_t decomp_buffer_len;
    
    struct RegisteredCompressionType{
        RegisteredCompressionType *next;
        char *ext;
        int type;
        RegisteredCompressionType(){
            ext = NULL;
            next = NULL;
        };
        RegisteredCompressionType( const char *ext, int type ){
            this->ext = new char[ strlen(ext)+1 ];
            for ( unsigned int i=0 ; i<strlen(ext)+1 ; i++ ){
                this->ext[i] = ext[i];
                if ( this->ext[i] >= 'a' && this->ext[i] <= 'z' )
                    this->ext[i] += 'A' - 'a';
            }
            this->type = type;
            this->next = NULL;
        };
        ~RegisteredCompressionType(){
            if (ext) delete[] ext;
        };
    } root_registered_compression_type, *last_registered_compression_type;

    FILE *fopen(const char *path, const char *mode);
    unsigned char readChar( FILE *fp );
    unsigned short readShort( FILE *fp );
    unsigned long readLong( FILE *fp );
    void writeChar( FILE *fp, unsigned char ch );
    void writeShort( FILE *fp, unsigned short ch );
    void writeLong( FILE *fp, unsigned long ch );
    static unsigned short swapShort( unsigned short ch );
    static unsigned long swapLong( unsigned long ch );
    size_t decodeNBZ( FILE *fp, size_t offset, unsigned char *buf );
#ifdef TOOLS_BUILD
    size_t encodeNBZ( FILE *fp, size_t length, unsigned char *buf );
#endif
    int getbit( FILE *fp, int n );
    size_t decodeSPB( FILE *fp, size_t offset, unsigned char *buf );
    size_t decodeLZSS( struct ArchiveInfo *ai, int no, unsigned char *buf );
    int getRegisteredCompressionType( const char *file_name );
    size_t getDecompressedFileLength( int type, FILE *fp, size_t offset );
    
private:
    //file_name parameter is assumed to use SJIS encoding
    FILE *getFileHandle( const char *file_name, int &compression_type, size_t *length );
};

#endif // __DIRECT_READER_H__
