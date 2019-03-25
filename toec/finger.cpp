#include "finger.h"


Finger::Finger()
{

}
int Finger::finger_isEnabled(char* uri)
{
    int nRet = fingercomm.IsDeviceFingerOpenForPrint(uri);
    if(nRet == 1)
    {
        LOGLOG("\r\n#finger_isEnabled: IsDeviceFingerOpenForPrint: open!");
        return 1;
    }
    else
    {
        if(nRet == 3)
        {
            LOGLOG("\r\n#finger_isEnabled: IsDeviceFingerOpenForPrint: open but no finger!");
            return 3;
        }
        else if(nRet == 4)
        {
            LOGLOG("\r\n#finger_isEnabled: IsDeviceFingerOpenForPrint: busy!");
        }
        else
        {
            LOGLOG("\r\n#finger_isEnabled: IsDeviceFingerOpenForPrint: close!");
        }
    }


    return 0;
}

int Finger::finger_check(char* uri, int mTimeout)
{
    char userName[33] = {0};
    short index = 0;
    int ret = 0;

    if((ret=fingercomm.IsFingerPrint(uri,userName, &index, mTimeout)) == 0)
    {
//        qDebug()<<QString::fromUtf8("指纹识别成功,可以打印!");
//        qDebug()<<"userName="<<userName<<" index="<<index;
        return ret;
    }
    else
    {
//        qDebug()<<QString::fromUtf8("指纹识别失败,再试一次!");
        return ret;
    }
    return ret;
}

void Finger::finger_cancel(char* uri)
{
    fingercomm.CancelFingerPrint(uri);
}
