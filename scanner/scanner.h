#ifndef SCANNER_H
#define SCANNER_H

#include "scannerapp.h"
class ScannerApi;
class Scanner
{
public:
    Scanner(ScannerApp* app);
    ~Scanner();

    int flat_scan(ScanSettings* settings);
    int adf_scan(ScanSettings* settings);
    void set_cancel(bool cancel){m_cancel = cancel;}
private:
    ScannerApi* scannner_api;
    ScannerApp* scanner_app;
    bool m_cancel;
};

#endif // SCANNER_H
