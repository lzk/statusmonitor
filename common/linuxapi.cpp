#include <sys/statfs.h>
#include "log.h"

bool is_disk_no_space(const char* path ,unsigned int prefer_size)
{
    struct statfs tmp_buf;
    unsigned int t_space_size = prefer_size;
    if(!statfs(path ,&tmp_buf)){
        t_space_size = (unsigned int)(tmp_buf.f_bavail * tmp_buf.f_bsize >> 10);
    }
//    LOGLOG("device space:%d K" ,t_space_size);
    return t_space_size < prefer_size;
}
