#include "imagetrans.h"

#include "rawtobmp.h"
#include <string.h>
#include "log.h"
ImageTrans::ImageTrans()
{

}

int ImageTrans::trans_header(ImageTransInfo* info)
{
    char* buffer = info->target_buffer;
    int buf_size = info->target_buf_size;
    int index = 0;

    if(!buffer)
        return -1;

    if(info->mode & ImageTransMode_rawToBmp){
        index = RawToBmp::trans_header(buffer ,buf_size ,&info->target_image_info);
    }

    return index;
}


int ImageTrans::init(ImageTransInfo* info)
{
    int index = trans_header(info);
    info->real_line = 0;
    info->resume_bytes = 0;
    info->scanned_buf_size = 0;
    return index;
}

bool if_need_add_line(int line ,int source_dpi ,int target_dpi)
{
    bool add = false;
    if(source_dpi == 600 && target_dpi == 1200){
        if(line % 2){
            add = true;
        }
    }

    return add;
}

int choudian(char* buffer ,int buf_size ,const char* source_buf ,ImageTransInfo* info)
{
    ImageInfo* target = &info->target_image_info;
    int factor_x = info->source_dpi_x / target->Resolution;
    if(factor_x < 1)
        factor_x = 1;
//    int source_pixels_per_line = info->source_pixelsOfWidth;
    int factor_y = info->source_dpi_y / target->Resolution;
    if(factor_y < 1)
        factor_y = 1;
    if(factor_x == 1 && factor_y == 1){
        memcpy(buffer ,source_buf ,buf_size);
    }else{
        for(int i = 0 ;i < buf_size ;i++){
            //use the first byte first
            buffer[i] = source_buf[i * factor_x];
        }
    }
}

int ImageTrans::trans(ImageTransInfo* info)
{
    ImageInfo* target = &info->target_image_info;

    int line_buf_size = info->target_line_buf_size;
    int source_line_buf_size = info->source_line_buf_size;
    int source_size = info->scanned_buf_size + info->resume_bytes;
    int source_buf_size_per_line = info->source_line_buf_size * info->source_lines_per_line;

    int new_source_lines = source_size / source_line_buf_size;
    int new_target_lines = new_source_lines * info->lines_per_source_line;
    if(new_source_lines + info->real_line > info->source_total_lines)
        new_source_lines = info->source_total_lines - info->real_line;

    memcpy(info->resume_buffer + info->resume_bytes ,info->source_buffer ,info->scanned_buf_size);
    const char* source_line_buf = info->resume_buffer;
    char* target_line_buffer = info->target_buffer;

    for(int i = 0 ;i < new_target_lines ;i++){//every line
        if(if_need_add_line(i ,info->source_dpi_y ,target->Resolution)){//if add line
            LOGLOG("add one lines");
            memcpy(target_line_buffer ,target_line_buffer - line_buf_size ,line_buf_size);
        }else{
            if(info->mode & ImageTransMode_rawToBmp){
                choudian(target_line_buffer ,line_buf_size ,source_line_buf ,info);
                if(info->target_image_info.BitsPerPixel == 24){
                    RawToBmp::rgb2bgr(target_line_buffer ,line_buf_size ,target_line_buffer ,line_buf_size);
                }
            }
            source_line_buf += source_buf_size_per_line;
        }
        target_line_buffer += line_buf_size;
    }

    info->real_line += new_target_lines;

    info->target_buf_size = new_target_lines * line_buf_size;//tell target buf size

    //resume rest buf less than a line
    info->resume_bytes = source_size % source_buf_size_per_line;

    if(info->resume_bytes){
        memmove(info->resume_buffer ,source_line_buf ,info->resume_bytes);
    }

    return 0;
}
