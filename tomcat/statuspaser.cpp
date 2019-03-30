#include "statuspaser.h"

StatusPaser::StatusPaser()
{

}

bool StatusPaser::AnyTonerReachLevel1(const PrinterStatus_struct& m_PrinterStatus)
{
    if (m_PrinterStatus.PrinterStatus == PS_ERROR_POWER_OFF)
        return false;
    if (m_PrinterStatus.TonelStatusLevelC <= 20 || m_PrinterStatus.TonelStatusLevelM <= 20 || m_PrinterStatus.TonelStatusLevelY <= 20 || m_PrinterStatus.TonelStatusLevelK <= 20)
        return true;
    else	return false;
}

bool StatusPaser::IsNonDellTonerMode(const PrinterStatus_struct& m_PrinterStatus)
{
    if (m_PrinterStatus.NonDellTonerMode)
        return true;
    else	return false;
}

bool StatusPaser::OnlyColorTonerEmpty(const PrinterStatus_struct& m_PrinterStatus)
{
    if (m_PrinterStatus.PrinterStatus == PS_ERROR_POWER_OFF)
        return false;
    if (m_PrinterStatus.TonelStatusLevelK != 0 && (m_PrinterStatus.TonelStatusLevelC == 0 || m_PrinterStatus.TonelStatusLevelM == 0 || m_PrinterStatus.TonelStatusLevelY == 0))
        return true;
    else	return false;
}
