#include "scannerapi.h"
#include <stdint.h>

typedef  uint64_t UINT64;
typedef  uint32_t UINT32;
typedef  uint16_t UINT16;
typedef  uint8_t UINT8;

typedef  int64_t INT64;
typedef  int32_t INT32;
typedef  int16_t INT16;
typedef  int8_t INT8;

typedef  uint8_t BYTE;
typedef  uint16_t WORD;

//#define    CMDID_SCANVERSION            0x56
//#define    CMDID_DOWNLOADGAMMA        0x4d
//#define    CMDID_MOTODL                    0x54
//#define    CMDID_MOTORDLSEND            0x4f
//#define    CMDID_FLASHDLPARM            0x50
//#define    CMDID_FLASHDOWNLOAD            0x48
//#define    CMDID_FAXADDRESSBOOK        0x58
#define    CMDID_LOCKSCANNER            0x6c//0x4c
//#define    CMDID_TONECALIBRATION        0x43
//#define    CMDID_GETADFSTATUS            0x66//0x46
#define    CMDID_SETSCANNERINFO            0x69//0x49
#define    CMDID_ABORTSCAN                0x61//0x41
#define    CMDID_UNLOCKSCANNER            0x75//0x55
//#define    CMDID_PSCANVERSION            0x52
#define    CMDID_PSTATUS                    0x73//0x53
#define    CMDID_PSCANDATA                0x64//0x44

#define    CMDID_CMDHEAD				0x53

typedef struct scannerCmd
{
    UINT8 cmdHEAD;
    UINT8 cmdID;
    UINT8 reserved[6];
}ScanCmd;

typedef struct statusRsp
{
    UINT8 cmdID;
    UINT8 status;
    UINT8 ADFStatus;
    UINT8 WarmLeftTime;
    UINT8 reserved[4];
}StatusRsp;

typedef struct scanDataRsp
{
    UINT8 cmdID;
    UINT8 flag;
    UINT8 reserved[2];
    UINT32 dataSize;
}ScanDataRsp;

typedef struct tagScanInfo
{

    UINT32 nScanSrc;                            // 0: Flat Bed, 1: ADF scan, 2: MFP Auto selects source

    UINT32 nTPA;                                // 0: no support,  1: supported
    UINT32 nEndorser;                        // reserve

    //UINT32 nOpticalXResolution;                // Optical X Resolution
    //UINT32 nOpticalYResolution;                // Optical Y Resolution
    UINT16 nYear;

    UINT8 nMonth;
    UINT8 nDay;

    UINT8 nHour;
    UINT8 nMinute;

    UINT16 nTimeZone;

    UINT32 nBedWidth;                        // Bed width in 1000's of an inch
    UINT32 nBedHeight;                        // Bed height in 1000's of an inch

    INT32 nIntensityMin;                        // Intensity range minimum value
    INT32 nIntensityMax;                    // Intensity range maximum value
    UINT32 nIntensityStep;                    // Intensity range increment/step value

    INT32 nContrastMin;                        // Contrast range minimum value
    INT32 nContrastMax;                        // Contrast range maximum value
    UINT32 nContrastStep;                        // Contrast range increment/step value

    UINT32 nSupportedCompressionType;        // Supported compression type; 0 - None
    UINT32 nSupportedDataTypes;                // Mask of supported data types, i.e. COLOR | BW

    UINT32 nWidthInPixels;                    // Width of image in pixels
    UINT32 nWidthInBytes;                    // Width of image in bytes
    UINT32 nLines;                            // Height of image in pixels

    UINT32 nDataType;                        // Data Types: 1 - BW, 2- Grayscale, 4 - Color
    UINT32 nBitsPerPixel;                        // Pixel Bits: 1 - BW, 8- Grayscale, 24 - Color

    UINT32 nIntensity;                        // Intensity
    UINT32 nContrast;                            // Contrast

    UINT32 nXResolution;                        // X resolution
    UINT32 nYResoultion;                        // Y resolution

    UINT32 nXPosition;                        // X position (left) of scanner window settings (4x)
    UINT32 nYPosition;                        // Y position (top) of scanner window settings
    UINT32 nXExtent;                            // X Extent (number of pixels in a line) of scanner window (32x)
    UINT32 nYExtent;                            // Y Extent (number of lines) of scanner window (32x)

    UINT32 nDitherPattern;                    // Dither Pattern
    UINT32 nNegative;                        // Negative
    UINT32 nMirror;                            // Mirror
    UINT32 nAutoback;                        // Auto back
    UINT32 nColorDitherPattern;                // Color Dither Pattern
    UINT32 nToneMap;                        // Tone Map
    UINT32 nCompression;                        // Compression: 0 - off, 1 - Compression is on
    UINT32 nRawDataFormat;                    // Raw Data Format: 0 - Packed data, 1 - Planar data
    UINT32 nRawPixelOrder;                    // Raw Pixel Order: 0 - RGB, 1 - BGR, 2 - GRB
    UINT32 nNeedDataAlignment;                // Need Data Alignment: 0 - FALSE, 1 - TRUE
    UINT32 nDelayBetweenRead;                // Delay between read: milliseconds
    UINT32 nMaximumBufferSize;                // Maximum buffer size in scanner

} SCANINFO, *LPSCANINFO;

typedef struct scanInfoCmd
{
    UINT8 cmdHEAD;
    UINT8 cmdID;
    UINT8 reserved[2];
    UINT32 dataLength;
    SCANINFO scanInfo;
}ScanInfoCmd;

typedef struct abortScanCmd
{
    UINT8 cmdHEAD;
    UINT8 cmdID;
    //UINT8 m_nType;
    UINT8 Reserved[6];
}AbortScanCmd;

enum scan_sources
{
    SOURCE_FLATBED=0,
    SOURCE_ADF_SIMPLEX,
    SOURCE_ADF_DUPLEX,

};

#ifdef VARIABLES_UNIT_MM
#define SCAN_RANGE_WIDTH 215.9 //The unit is millimeter
#define SCAN_RANGE_HEIGHT 297.2 //The unit is millimeter
#define SCAN_RANGE_MIN 10.9 //The unit is millimeter

#define    HORIZONTAL_BED_SIZE    (SCAN_RANGE_WIDTH * 1000.0 / 25.4)//8500        // Bed width in thousandths of an inch
#define    VERTICAL_BED_SIZE        (SCAN_RANGE_HEIGHT * 1000.0 / 25.4)//11690        // Bed height in thousandths of an inch

#define PRECISION 0.0001
#else
#define SCAN_RANGE_WIDTH 8.5 //The unit is inch
#define SCAN_RANGE_HEIGHT 11.7008 //The unit is inch
#define SCAN_RANGE_MIN 0.429133 //The unit is inch

#define    HORIZONTAL_BED_SIZE    (SCAN_RANGE_WIDTH * 1000.0)//8500        // Bed width in thousandths of an inch
#define    VERTICAL_BED_SIZE        (SCAN_RANGE_HEIGHT * 1000.0)//11690        // Bed height in thousandths of an inch

#define PRECISION 0.00001
#endif

#define    HORIZONTAL_SHEET_FEED_SIZE    8500    // Sheet feed width in thousandths of an inch
#define    VERTICAL_SHEET_FEED_SIZE        14000    // Sheet feed height in thousandths of an inch

#define    INTENSITY_MAX    127
#define    INTENSITY_MIN    -127
#define    INTENSITY_STEP    1

#define    CONTRAST_MAX    127
#define    CONTRAST_MIN    -127
#define    CONTRAST_STEP    1

#define SIZE_ACCURACY 0.1

#ifdef VARIABLES_UNIT_MM
    #define UNIT_FACTOR 25.4
#else
#define UNIT_FACTOR 1
#endif

typedef struct{
    int index;
    float width;
    float height;
}
    Scan_Size_t;

//const Scan_Size_t scan_size_array[] =
//{
//    {Scan_A4, 210/25.4, 297/25.4},
//    {Scan_A5, 148/25.4, 210/25.4},
//    {Scan_B5, 182/25.4, 257/25.4},
//    {Scan_Letter, 8.5, 11.0},
//    {Scan_4_6, 4, 6},
//};

const Scan_Size_t scan_size_array[] =
{
    {Scan_A4, 8.268, 11.693},
    {Scan_A5, 5.827, 8.268},
    {Scan_B5, 7.165, 10},
    {Scan_Letter, 8.5, 11.0},
    {Scan_4_6, 4, 6},
};

#include <time.h>
#include <sys/timeb.h>

void calculate_parameters(ScanSettings* scan_settings)
{
    Calc_Data *pCalc = &scan_settings->calc_data;
    UiSettings* settings = &scan_settings->settings;

    switch (settings->scan_dpi) {
    case Scan_100DPI:
        pCalc->dpi_x = pCalc->dpi_y = 100;
        break;
    case Scan_200DPI:
        pCalc->dpi_x = pCalc->dpi_y = 200;
        break;
    case Scan_600DPI:
        pCalc->dpi_x = pCalc->dpi_y = 600;
        break;
    case Scan_1200DPI:
        pCalc->dpi_x = pCalc->dpi_y = 1200;
        break;
    case Scan_300DPI:
    default:
        pCalc->dpi_x = pCalc->dpi_y = 300;
        break;
    }

    pCalc->source_pixel_resolution = pCalc->dpi_x;
    pCalc->source_motor_resolution = pCalc->dpi_x;
    if(pCalc->dpi_x < 300){
        pCalc->source_pixel_resolution = 300;
        pCalc->source_motor_resolution = 300;
    }else if(pCalc->dpi_x == 1200){
        pCalc->source_motor_resolution = 600;
    }

    float scan_width = scan_size_array[settings->scan_size].width;
    float scan_height = scan_size_array[settings->scan_size].height;
    int pixel_alignment = 32;

    pCalc->pixels_per_line = scan_width * pCalc->source_pixel_resolution / UNIT_FACTOR;
    pCalc->source_total_lines = scan_height * pCalc->source_motor_resolution / UNIT_FACTOR;

    pCalc->source_pixels_per_line = ((pCalc->pixels_per_line + pixel_alignment -1) / pixel_alignment) * pixel_alignment;

}

void caculate_image_trans_data(ScanSettings* settings)
{
    ImageTransInfo* info = settings->info;
    ImageInfo* target = &info->target_image_info;

    info->format = ImageTransFormat_Raw;
    info->mode = ImageTransMode_rawToBmp;

    info->source_pixelsOfWidth = settings->calc_data.source_pixels_per_line;
    info->source_dpi_x = settings->calc_data.source_pixel_resolution;
    info->source_dpi_y = settings->calc_data.source_motor_resolution;
    info->source_total_lines = settings->calc_data.source_total_lines;


    target->Resolution = settings->calc_data.dpi_x;
    info->source_lines_per_10_lines = info->source_dpi_y * 10 / target->Resolution;
    LOGLOG("source_lines_per_10_lines is:%d" ,info->source_lines_per_10_lines);
    target->PixelsOfWidth = settings->calc_data.pixels_per_line
            * target->Resolution / info->source_dpi_x;
    target->PixelsOfWidth += 7;
    target->PixelsOfWidth /= 8;
    target->PixelsOfWidth *= 8;
    LOGLOG("target->PixelsOfWidth is:%d" ,target->PixelsOfWidth);
    target->TotalScanLines = info->source_total_lines * 10 / info->source_lines_per_10_lines;
    LOGLOG("target->TotalScanLines is:%d" ,target->TotalScanLines);

    info->source_line_buf_size = info->source_pixelsOfWidth;
    info->target_line_buf_size = target->PixelsOfWidth;
    switch (settings->settings.colorModel) {
    case Black_White:
        target->BitsPerPixel = 1;
//        info->source_line_buf_size /= 8;
        info->target_line_buf_size /= 8;
        break;
    case Grayscale:
        target->BitsPerPixel = 8;
        break;
    case Color:
    default:
        target->BitsPerPixel = 24;
        info->source_line_buf_size *= 3;
        info->target_line_buf_size *= 3;
        break;
    }
    LOGLOG("source_pixelsOfWidth is:%d" ,info->source_pixelsOfWidth);
    LOGLOG("target->PixelsOfWidth is:%d" ,target->PixelsOfWidth);

    info->image_trans_parameter.gamma = 1000;
    info->image_trans_parameter.brightness = settings->settings.brightness;
    info->image_trans_parameter.contrast = settings->settings.contrast;
    info->image_trans_parameter.mediaO = settings->settings.scan_doctype;
}


SCANINFO get_parameters(ScanSettings* scan_settings)
{
    Calc_Data *pCalc = &scan_settings->calc_data;
    UiSettings* settings = &scan_settings->settings;

    SCANINFO scan_info;
    scan_info.nScanSrc = SOURCE_FLATBED;

    scan_info.nTPA = 0;
    scan_info.nEndorser = 0;

    struct timeb tp;
    ftime(&tp);
    time_t t;
    struct tm *p_time;
    time(&t);
    p_time = gmtime(&t);
    scan_info.nYear = p_time->tm_year + 1900;
    scan_info.nMonth = p_time->tm_mon + 1;
    scan_info.nDay = p_time->tm_mday;
    scan_info.nHour = p_time->tm_hour;
    scan_info.nMinute = p_time->tm_min;
    scan_info.nTimeZone = (UINT16)(tp.timezone);

    if(scan_info.nScanSrc == SOURCE_FLATBED)
    {
        scan_info.nBedWidth = HORIZONTAL_BED_SIZE;                        // Bed width in 1000's of an inch
        scan_info.nBedHeight = VERTICAL_BED_SIZE;                        // Bed height in 1000's of an inch
    }
    else
    {
        // ADF
        scan_info.nBedWidth = HORIZONTAL_SHEET_FEED_SIZE;                    // Bed width in 1000's of an inch
        scan_info.nBedHeight = VERTICAL_SHEET_FEED_SIZE;                        // Bed height in 1000's of an inch
    }
    scan_info.nIntensityMin = INTENSITY_MIN;                        // Intensity range minimum value
    scan_info.nIntensityMax = INTENSITY_MAX;                    // Intensity range maximum value
    scan_info.nIntensityStep = INTENSITY_STEP;                    // Intensity range increment/step value

    scan_info.nContrastMin = CONTRAST_MIN;                        // Contrast range minimum value
    scan_info.nContrastMax = CONTRAST_MAX;                        // Contrast range maximum value
    scan_info.nContrastStep = CONTRAST_STEP;                        // Contrast range increment/step value

    scan_info.nSupportedCompressionType = 0;        // Supported compression type; 0 - None
    scan_info.nSupportedDataTypes = 7;                // Mask of supported data types, i.e. COLOR | BW

    scan_info.nWidthInPixels = 5100;                    // Width of image in pixels
    scan_info.nWidthInBytes = 0xFFFFFC4F;                    // Width of image in bytes
    scan_info.nLines = 1000;                            // Height of image in pixels
/*
    scan_info.nWidthInPixels = source_pixels_per_line;                    // Width of image in pixels
    scan_info.nWidthInBytes = source_total_lines;                    // Width of image in bytes
    scan_info.nLines = source_bytes_per_line;
 */
    switch (settings->colorModel) {
    case Black_White:
    case Grayscale:
        scan_info.nDataType = 2;                        // Data Types: 1 - BW, 2- Grayscale, 4 - Color
        scan_info.nBitsPerPixel = 8;                        // Pixel Bits: 1 - BW, 8- Grayscale, 24 - Color
        break;
    case Color:
    default:
        scan_info.nDataType = 4;                        // Data Types: 1 - BW, 2- Grayscale, 4 - Color
        scan_info.nBitsPerPixel = 24;                        // Pixel Bits: 1 - BW, 8- Grayscale, 24 - Color
        break;
    }

    scan_info.nIntensity = 0;                        // Intensity
    scan_info.nContrast = 0;                            // Contrast

    scan_info.nXResolution = pCalc->source_pixel_resolution;                        // X resolution
    scan_info.nYResoultion = pCalc->source_motor_resolution;                        // Y resolution

    scan_info.nXPosition = 0;                        // X position (left) of scanner window settings (4x)
    scan_info.nYPosition = 0;                        // Y position (top) of scanner window settings

    scan_info.nXExtent = pCalc->source_pixels_per_line;                            // X Extent (number of pixels in a line) of scanner window (32x)
    scan_info.nYExtent = pCalc->source_total_lines;                            // Y Extent (number of lines) of scanner window (32x)

    scan_info.nDitherPattern = 0;                    // Dither Pattern
    scan_info.nNegative = 0;                        // Negative
    scan_info.nMirror = 0;                            // Mirror
    scan_info.nAutoback = 0;                        // Auto back
    scan_info.nColorDitherPattern = 0;                // Color Dither Pattern
    scan_info.nToneMap = 0;                        // Tone Map
    scan_info.nCompression = 0;                        // Compression: 0 - off, 1 - Compression is on
    scan_info.nRawDataFormat = 1;                    // Raw Data Format: 0 - Packed data, 1 - Planar data
    scan_info.nRawPixelOrder = 0;                    // Raw Pixel Order: 0 - RGB, 1 - BGR, 2 - GRB
    scan_info.nNeedDataAlignment = 1;                // Need Data Alignment: 0 - FALSE, 1 - TRUE
    scan_info.nDelayBetweenRead = 0;                // Delay between read: milliseconds
    scan_info.nMaximumBufferSize = 0;                // Maximum buffer size in scanner
    LOGLOG("%s: Res=%d, xPos=%d, yPos=%d, W=%d, H=%d, Mode=%d. exit!\n", __FUNCTION__, scan_info.nXResolution, scan_info.nXPosition, scan_info.nYPosition, scan_info.nXExtent, scan_info.nYExtent, scan_info.nBitsPerPixel);

    return scan_info;
}

ScannerApi::ScannerApi(DeviceIO** _device)
    :device(_device)
    ,scanner_locked(false)
{

}

int ScannerApi::open()
{
    int ret = (*device)->open(53010);
    return ret;
}
int ScannerApi::close()
{
    (*device)->close();
    return 0;
}

int ScannerApi::get_cmd_status()
{
    int ret;
    StatusRsp statusRsp = {0};
    ret = (*device)->read_bulk((char*)&statusRsp ,sizeof(statusRsp));
    if(ret < 0){
        return ret;
    }
    if(statusRsp.cmdID != CMDID_PSTATUS){
        ret = -1;
    }else{
        ret = statusRsp.status;
    }
    return ret;
}
#include <unistd.h>
int ScannerApi::lock()
{
    if(scanner_locked)
    {
        return 0;
    }
    ScanCmd cmd = {CMDID_CMDHEAD, CMDID_LOCKSCANNER};

    int ret;

    for(int i = 0 ;i < 10 ;i++){
        ret = (*device)->write_bulk((char*)&cmd ,sizeof(cmd));
        if(ret < 0){
            return ret;
        }
        ret = get_cmd_status();
        if(ret == ScannerApp::STATUS_WARMINGUP){
            LOGLOG("warming up,please try again later");
        }else if(ret == ScannerApp::STATUS_USEWITHOUTLOCK){
//            unlock();
            break;
        }else{
            break;
        }
        usleep(1000000);
    }

    if(!ret)
        scanner_locked = true;
    return ret;
}

int ScannerApi::unlock()
{
    if(!scanner_locked)
    {
        return 0;
    }
    ScanCmd cmd = {CMDID_CMDHEAD, CMDID_UNLOCKSCANNER};

    int ret;
    for(int i = 0 ;i < 3 ;i++){
        ret = (*device)->write_bulk((char*)&cmd ,sizeof(cmd));
        if(ret == sizeof(cmd)){
            break;
        }
    }
    if(ret < 0){
        return ret;
    }

    ret = get_cmd_status();
    if(ret > -1){
        usleep(50000);
    }
    if(!ret)
        scanner_locked = false;
    return ret;
}

int ScannerApi::set_parameters(ScanSettings* settings)
{
    ScanInfoCmd cmd = {CMDID_CMDHEAD, CMDID_SETSCANNERINFO};
    cmd.scanInfo = get_parameters(settings);

    int ret;

    ret = (*device)->write_bulk((char*)&cmd ,sizeof(cmd));
    if(ret < 0){
        return ret;
    }
    ret = get_cmd_status();
    return ret;
}

int ScannerApi::start()
{
    return 0;
}

int ScannerApi::stop()
{
    return 0;
}

int ScannerApi::abort()
{
    AbortScanCmd cmd = {CMDID_CMDHEAD, CMDID_ABORTSCAN};
    int ret;

    ret = (*device)->write_bulk((char*)&cmd ,sizeof(cmd));
    if(ret < 0){
        return ret;
    }

    UINT8 imgBuffer[0x8000];
    ScanStatus status;
    while (1) {
        ret = get_scan_status(&status);
        if(ret)
            break;
        if(status.status == ScanStatus_Scanning){
            ret = get_scan_data((char*)imgBuffer ,status.data_size);
            if(ret < 0)
                break;
        }else if(status.status == ScanStatus_End){
            ret = 0;
            break;
        }
    }

    return ret;
}

int ScannerApi::get_scan_status(ScanStatus* status)
{
    int ret;
    ScanDataRsp statusRsp = {0};
    ret = (*device)->read_bulk((char*)&statusRsp ,sizeof(statusRsp));
    if(ret < 0){
        return ret;
    }
    if(statusRsp.cmdID == CMDID_PSCANDATA){
        ret = 0;
        status->status = ScanStatus_Scanning;
        status->data_size = statusRsp.dataSize;
    }else if(statusRsp.cmdID == CMDID_PSTATUS){
        ret = 0;
        status->status = ScanStatus_End;
        status->error_code = statusRsp.flag;
    }
    return ret;
}

int ScannerApi::get_scan_data(char* buffer ,int buffer_size)
{
    int ret;
    ret = (*device)->read_bulk(buffer, buffer_size);
    return ret;
}
