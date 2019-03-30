#ifndef STATUSPASER_H
#define STATUSPASER_H

#include "statusmonitor.h"
#include "error.h"

class StatusPaser
{
public:
    StatusPaser();

    static bool AnyTonerReachLevel1(const PrinterStatus_struct& m_PrinterStatus);
    static bool IsNonDellTonerMode(const PrinterStatus_struct& m_PrinterStatus);
    static bool OnlyColorTonerEmpty(const PrinterStatus_struct& m_PrinterStatus);
};

#endif // STATUSPASER_H
