#ifndef FINGER_H
#define FINGER_H
#include "FingerCommon.h"

class Finger
{
public:
    Finger();
    int finger_isEnabled(char* uri);
    int finger_check(char* uri, int mTimeout);
    void finger_cancel(char* uri);
private:
   FingerCommon fingercomm;
};

#endif // FINGER_H
