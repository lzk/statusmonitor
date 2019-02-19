#ifndef FINGER_H
#define FINGER_H
#include "FingerCommon.h"

class Finger
{
public:
    Finger();
    bool finger_isEnabled(char* uri);
    bool finger_check(char* uri);
    void finger_cancel(char* uri);
private:
   FingerCommon fingercomm;
};

#endif // FINGER_H
