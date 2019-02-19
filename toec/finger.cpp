#include "finger.h"


Finger::Finger()
{

}
bool Finger::finger_isEnabled(char* uri)
{
    //if(fingercomm.IsDeviceConnectForPrint(uri))
    {
        if(fingercomm.IsDeviceFingerOpenForPrint(uri))
        {
            return true;
        }

    }
    return false;
}

bool Finger::finger_check(char* uri)
{
    char userName[33] = {0};
    short index = 0;
    if(fingercomm.IsFingerPrint(uri,userName, &index) == 0)
    {
//        qDebug()<<QString::fromUtf8("指纹识别成功,可以打印!");
//        qDebug()<<"userName="<<userName<<" index="<<index;
        return true;
    }
    else
    {
//        qDebug()<<QString::fromUtf8("指纹识别失败,再试一次!");
        return false;
    }
    return false;
}

void Finger::finger_cancel(char* uri)
{
    fingercomm.CancelFingerPrint(uri);
}
