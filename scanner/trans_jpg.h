#ifndef TRANS_JPG_H
#define TRANS_JPG_H

#include <stdio.h>
#include <jpeglib.h>
class Trans_jpg
{
public:
    Trans_jpg();

    int read_file_open(const char* filename);
    int read_file_read_lines(char* read_buffer ,int bufsize ,int lines);
    int read_file_close();

private:
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE * infile;		/* source file */
    JSAMPARRAY buffer;		/* Output row buffer */
    int row_stride;		/* physical row width in output buffer */
};

#endif // TRANS_JPG_H
