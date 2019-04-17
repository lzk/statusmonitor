#include "statuspaser.h"
#include "uiconfig.h"
StatusPaser::StatusPaser()
{

}

bool StatusPaser::is_enable_scan(int status)
{
    bool enable = false;
    switch (status) {
    case UIConfig::Ready:
    case UIConfig::PowerSaving:
        //toner end
    case UIConfig::TonerEnd1:
    case UIConfig::TonerEnd2:
    case UIConfig::TonerNearEnd:
    case UIConfig::WasteTonerFull:
        //paper jam
    case UIConfig::InitializeJam:
    case UIConfig::NofeedJam:
    case UIConfig::JamAtRegistStayOn:
    case UIConfig::JamAtExitNotReach:
    case UIConfig::JamAtExitStayOn:
        enable = true;
        break;
    default:
        break;
    }
    return enable;
}

bool StatusPaser::is_enable_copy(int status)
{
    bool enable = false;
    switch (status) {
    case UIConfig::Ready:
    case UIConfig::PowerSaving:
    case UIConfig::TonerEnd1:
    case UIConfig::TonerEnd2:
    case UIConfig::TonerNearEnd:
    case UIConfig::WasteTonerFull:
    case UIConfig::CopyScanNextPage:
        enable = true;
        break;
    default:
        break;
    }
    return enable;
}
