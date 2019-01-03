#ifndef IMAGETRANS_H
#define IMAGETRANS_H

#include "imageinfo.h"
enum{
    ImageTransMode_rawToBmp = 0x01,
    ImageTransMode_bmpToRaw = 1 << 1,
};

enum{
    ImageTransFormat_Raw,
    ImageTransFormat_Bmp,
};

typedef struct
{
    ImageInfo target_image_info;
    int source_dpi_x;
    int source_dpi_y;
    int source_total_lines;
    int source_pixelsOfWidth;

    int format;
    int mode;

    char* resume_buffer;
    int resume_buf_size;
    int scanned_buf_size;
    int resume_bytes;
    char* source_buffer;
    int source_buf_size;
    int source_line_buf_size;
    char* target_buffer;
    int target_buf_size;
    int target_line_buf_size;
    int source_lines_per_line;
    int lines_per_source_line;

//    char* real_target_buf;
//    const char* real_source_buf;
    int real_line;
//    int ready_line;
//    int header_size;
}
    ImageTransInfo;

class ImageTrans
{
public:
    ImageTrans();
    static int init(ImageTransInfo* source_info);
    static int trans(ImageTransInfo* source_info);

private:
    static int trans_header(ImageTransInfo* source_info);
};

#endif // IMAGETRANS_H
