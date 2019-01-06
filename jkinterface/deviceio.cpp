#include "deviceio.h"
#include "log.h"
#include <unistd.h>
#define delay100ms(x ,y) {if(y) usleep((x)  * 100 * 1000);}

int DeviceIO::open(const char* url ,int port)
{
    int ret;
    ret = resolveUrl(url);
    if(!ret)
        ret = open(port);
    return ret;
}

int DeviceIO::resolveUrl(const char* url)
{
    int ret = -1;
    if(url){
        strcpy(device_uri ,url);
        ret = 0;
    }
    return ret;
}

int DeviceIO::writeThenRead(char* wrBuffer ,int wrSize ,char* rdBuffer ,int rdSize)
{
    int err = write(wrBuffer ,wrSize);
    if(err == wrSize){
//    if(!err){
        int j;
        int _read_size = 0;
        int nocheck=0;

        delay100ms(9 ,ifdelay);
        for(j = 0 ;j < 3 ;j++){
            if(!nocheck){
                if(1 == read(rdBuffer,1)){
                    if(0x4d != rdBuffer[0]){
//                        LOGLOG("waiting for 0x4d:%#.2x" ,rdBuffer[0]);
                        delay100ms (1 ,ifdelay);
                        continue;
                    }
                }else{
//                    LOGLOG("cannot read now,wait 100 ms read again");
                    delay100ms (1 ,ifdelay);
                    continue;
                }
            }
            nocheck = 0;
            delay100ms (1 ,ifdelay);
            if(1 == read(rdBuffer+1,1)){
                if(0x3c == rdBuffer[1]){
                    delay100ms (1 ,ifdelay);
                    _read_size = read(rdBuffer+2 ,rdSize-2);
//                    LOGLOG("read size:%d" ,_read_size == -1 ?-1 : _read_size + 2);
                    j++;
                    break;
                }else if(0x4d == rdBuffer[1]){
                    nocheck = 1;
                }
            }
            delay100ms (1 ,ifdelay);
        }

        LOGLOG("try %d times" ,j);
        if(_read_size == rdSize -2){
            err = 0;
        }else{
            LOGLOG("read wrong");
            err = -1;
        }
    }
    return err;
}