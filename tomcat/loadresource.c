
#include "ids_string.h"
#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
void PaperSize2Str(int bPaperSize, char* szPaperSize)
{
	//static const UINT grandePaperSizeTabl[] = {
	//	/*IDS_UNKONWN_PAPERSIZE_PAPERTYPE*/IDS_PAPERSIZE_CUSTOM,
	//	IDS_PAPERSIZE_A4,
	//	IDS_PAPERSIZE_B5,//fixed issue:#35425
	//	IDS_PAPERSIZE_A5,
	//	//IDS_PAPERSIZE_B5,//
	//	IDS_PAPERSIZE_LETTER,
	//	IDS_PAPERSIZE_FOILO,
	//	IDS_PAPERSIZE_LEGAL,
	//	IDS_PAPERSIZE_EXECUTIVE,
	//	IDS_PAPERSIZE_COM10,
	//	IDS_PAPERSIZE_MONARCH,
	//	IDS_PAPERSIZE_DL,
	//	IDS_PAPERSIZE_C5,
	//	IDS_PAPERSIZE_CUSTOM,
	//	IDS_PAPERSIZE_POSTCARD_JPN,
	//	IDS_MEDIATYPE_WPOSTCARD_JPN,
	//	IDS_MEDIATYPE_YOUGATA2,
	//	IDS_MEDIATYPE_YOUGATA3,
	//	IDS_MEDIATYPE_YOUGATA4,
	//	IDS_MEDIATYPE_YOUGATA6,
	//	IDS_MEDIATYPE_YOUNAGA3,
	//	IDS_MEDIATYPE_NAGAGATA3,
	//	IDS_MEDIATYPE_NAGAGATA4,
	//	IDS_MEDIATYPE_KAKUGATA3
	//	};	// fixed issue:#35567/#35562
    static const char* grandePaperSizeTabl[] = {
		/* 0x00 */ IDS_PAPERSIZE_CUSTOM,
		IDS_PAPERSIZE_A4,
		IDS_PAPERSIZE_B5,
		IDS_PAPERSIZE_A5,
		IDS_PAPERSIZE_LETTER,
        /* 0x05 */ IDS_PAPERSIZE_A6,//IDS_PAPERSIZE_EXECUTIVE,
        IDS_PAPERSIZE_FOILO,
        IDS_PAPERSIZE_LEGAL,
        IDS_PAPERSIZE_B6,//IDS_PAPERSIZE_STATEMENT,
        IDS_PAPERSIZE_16K,//IDS_PAPERSIZE_COM10,
		/* 0x0a */ IDS_PAPERSIZE_MONARCH,
		IDS_PAPERSIZE_C5,
		IDS_PAPERSIZE_DL,
		IDS_PAPERSIZE_POSTCARD_JPN,
		IDS_MEDIATYPE_WPOSTCARD_JPN,
		/* 0x0f */ IDS_MEDIATYPE_YOUGATA2,
		IDS_MEDIATYPE_YOUGATA3,
		IDS_MEDIATYPE_YOUGATA4,
		IDS_MEDIATYPE_YOUGATA6,
		IDS_MEDIATYPE_YOUNAGA3,
		/* 0x14 */ IDS_MEDIATYPE_NAGAGATA3,
		IDS_MEDIATYPE_NAGAGATA4,
		IDS_MEDIATYPE_KAKUGATA3
	};

    static const char* swiftPaperSizeTabl[] = {
		/* 0x00 */ IDS_PAPERSIZE_CUSTOM,
		IDS_PAPERSIZE_A4,
		IDS_PAPERSIZE_B5,
		IDS_PAPERSIZE_A5,
		IDS_PAPERSIZE_LETTER,
        /* 0x05 */ IDS_PAPERSIZE_A6,//IDS_PAPERSIZE_EXECUTIVE,
        IDS_PAPERSIZE_FOILO,
        IDS_PAPERSIZE_LEGAL,
        IDS_PAPERSIZE_B6,//IDS_PAPERSIZE_STATEMENT,
        IDS_PAPERSIZE_16K,//IDS_PAPERSIZE_COM10,
		/* 0x0a */ IDS_PAPERSIZE_MONARCH,
		IDS_PAPERSIZE_C5,
		IDS_PAPERSIZE_DL,
		IDS_PAPERSIZE_POSTCARD_JPN,
		IDS_MEDIATYPE_WPOSTCARD_JPN,
		/* 0x0f */ IDS_MEDIATYPE_YOUGATA2,
		IDS_MEDIATYPE_YOUGATA3,
		IDS_MEDIATYPE_YOUGATA4,
		IDS_MEDIATYPE_YOUGATA6,
		IDS_MEDIATYPE_YOUNAGA3,
		/* 0x14 */ IDS_MEDIATYPE_NAGAGATA3,
		IDS_MEDIATYPE_NAGAGATA4,
		IDS_MEDIATYPE_KAKUGATA3
	};

#define PaperSizeTabl (grandePaperSizeTabl)
    int uCountPaperSize = sizeof(PaperSizeTabl)/sizeof(PaperSizeTabl[0]);

    if (bPaperSize < uCountPaperSize) {
        strcpy(szPaperSize, PaperSizeTabl[bPaperSize]);
	}
}

void PaperType2Str(int bPaperType, char* szPaperType)
{
    static const char* grande2PaperTypeTabl[] = {
		/* 0 */	 IDS_UNKONWN_PAPERSIZE_PAPERTYPE,
		/* 1 */	 IDS_MEDIATYPE_PLAIN,
		/* 2 */	 IDS_MEDIATYPE_BOND,
		/* 3 */  IDS_MEDIATYPE_LIGHT_CARD,
		/* 4 */  IDS_MEDIATYPE_LIGHT_GLOSSY_CARD,
		/* 5 */  IDS_MEDIATYPE_LABEL,
		/* 6 */  IDS_MEDIATYPE_RECYCLED,
		/* 7 */	 IDS_MEDIATYPE_ENVELOPE,
		/* 8 */	 IDS_MEDIATYPE_PLANE1_SIDE,
		/* 9 */	 IDS_MEDIATYPE_THICK_SIDE,
		/* 10 */ IDS_MEDIATYPE_COVER1_SIDE,
		/* 11 */ IDS_MEDIATYPE_COATED_SIDE,
		/* 12 */ IDS_MEDIATYPE_RECYCLED_SIDE,
		/* 13 */ IDS_UNKONWN_PAPERSIZE_PAPERTYPE,
		/* 14*/IDS_MEDIATYPE_POSTCARD_JPN,
		/*15*/IDS_MEDIATYPE_LIGHTWEIGHT1,
		/*16*/IDS_MEDIATYPE_LIGHTWEIGHT_RELOADED,
		/*17*/IDS_MEDIATYPE_POSTCARD_RELOADED,
		/*18*/IDS_UNKONWN_PAPERSIZE_PAPERTYPE
	};// fixed issue:#35567/#35562

    static const char* swiftPaperTypeTabl[] = {
		IDS_UNKONWN_PAPERSIZE_PAPERTYPE,
		IDS_MEDIATYPE_PLAIN,
		IDS_MEDIATYPE_BOND,
		IDS_MEDIATYPE_LIGHT_CARD,
		IDS_MEDIATYPE_LIGHT_GLOSSY_CARD,
		IDS_MEDIATYPE_LABEL,
		IDS_MEDIATYPE_ENVELOPE,
		IDS_MEDIATYPE_RECYCLED,
		IDS_UNKONWN_PAPERSIZE_PAPERTYPE,
		IDS_UNKONWN_PAPERSIZE_PAPERTYPE,
		IDS_UNKONWN_PAPERSIZE_PAPERTYPE,
		IDS_UNKONWN_PAPERSIZE_PAPERTYPE,
		IDS_UNKONWN_PAPERSIZE_PAPERTYPE,
		IDS_MEDIATYPE_JAP_GLOSSY_CARD,
		IDS_MEDIATYPE_POSTCARD_JPN,
		IDS_UNKONWN_PAPERSIZE_PAPERTYPE
	};

#define PaperTypeTabl (grande2PaperTypeTabl)
    int uCountPaperType = sizeof(PaperTypeTabl)/sizeof(PaperTypeTabl[0]);

    const char* szBuffer = (bPaperType < uCountPaperType) ?
		PaperTypeTabl[bPaperType] :
        PaperTypeTabl[uCountPaperType - 1];

    strcpy(szPaperType, szBuffer);
}

#ifdef __cplusplus
}
#endif
