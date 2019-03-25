#ifndef TRANS_JPG_H
#define TRANS_JPG_H

typedef int (*func_jpec_save_scanline)(char* ,int);
int read_JPEG_file (char * filename ,func_jpec_save_scanline put_scanline_someplace);

class Trans_jpg
{
public:
    Trans_jpg();
};

#endif // TRANS_JPG_H
