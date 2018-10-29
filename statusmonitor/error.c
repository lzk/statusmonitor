#include "error.h"
//#include "statusmonitor.h"
#include <string.h>
#ifdef __cplusplus
extern "C" {
#endif

enum{
	MSG_10001,
	MSG_10002,
	MSG_10009,
	MSG_02001,
	MSG_02004,
	MSG_02005,
	MSG_02008,
	MSG_02009,
	MSG_02010,
	MSG_02011,
	MSG_02013,
	MSG_03003,
	MSG_03004,
	MSG_03005,
	MSG_03008,
	MSG_03009,
	MSG_03010,
	MSG_03016,
	MSG_03017,
	MSG_03018,
	MSG_03019,
	MSG_03020,
	MSG_05002,
	MSG_05003,
	MSG_05004,
	MSG_05005,
	MSG_05006,
	MSG_05007,
	MSG_05008,
	MSG_05009,
	MSG_05010,
	MSG_05011,
	MSG_05012,
	MSG_05013,
	MSG_05014,
	MSG_05015,
	MSG_05016,
	MSG_05017,
	MSG_05018,
	MSG_05019,
	MSG_05020,
	MSG_05021,
	MSG_05022,
	MSG_05023,
	MSG_05024,
	MSG_05025,
	MSG_05071,
	MSG_05027,
	MSG_05028,
	MSG_05029,
	MSG_05030,
	MSG_05031,
	MSG_05032,
	MSG_05033,
	MSG_05034,
	MSG_05035,
	MSG_05036,
	MSG_05037,
	MSG_05038,
	MSG_05039,
	MSG_05040,
	MSG_05041,
	MSG_05042,
	MSG_05066,
	MSG_05043,
	MSG_05044,
	MSG_05045,
	MSG_05046,
	MSG_05047,
	MSG_05048,
	MSG_05070,
	MSG_05049,
	MSG_05050,
	MSG_05051,
	MSG_05052,
	MSG_05057,
	MSG_05058,
	MSG_05059,
	MSG_05060,
	MSG_05061,
	MSG_05062,
	MSG_05063,
	MSG_05064,
	MSG_05072,
	MSG_04003,
	MSG_04004,
	MSG_04008,
	MSG_04009,
	MSG_04010,
	MSG_04011,
	MSG_04016,
	MSG_04019,
	MSG_07001,
	MSG_07006,
	MSG_07008,
	MSG_07009,
	MSG_01001,
	MSG_01002,
	MSG_01003,
	MSG_01004,
	MSG_01005,
	MSG_01006,
	MSG_01007,
	MSG_01008,
	MSG_01009,
	MSG_01030,
	MSG_01031,
	MSG_01032,
	MSG_01033,
	MSG_01050,
	MSG_01051,
	MSG_01052,
	MSG_01053,
	MSG_01054,
	MSG_01055,
	MSG_01056,
	MSG_01057,
	MSG_01058,
	MSG_01059,
	MSG_01060,
	MSG_01061,
	MSG_01062,
	MSG_01063,
	MSG_01064,
	MSG_01065,
	MSG_01066,
	MSG_01067,
	MSG_01068,
	MSG_01069,
	MSG_01070,
	MSG_01071,
	MSG_01072,
	MSG_12002,
	MSG_12004,

};

static const Error_struct error_table[] =
{
	{
		.group = 10
		,.id = 1
	,.code = "016-372"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_10001
	}
	,{
		.group = 10
		,.id = 2
	,.code = "117-348"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_10002
	}
	,{
		.group = 10
		,.id = 9
	,.code = "117-346"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_10009
	}
	,{
		.group = 2
		,.id = 1
	,.code = "116-314"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_02001
	}
	,{
		.group = 2
		,.id = 4
	,.code = "116-314"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_02004
	}
	,{
		.group = 2
		,.id = 5
	,.code = "018-338"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_02005
	}
	,{
		.group = 2
		,.id = 8
	,.code = "116-355"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_02008
	}
	,{
		.group = 2
		,.id = 9
	,.code = "116-355"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_02009
	}
	,{
		.group = 2
		,.id = 10
	,.code = "116-355"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_02010
	}
	,{
		.group = 2
		,.id = 11
	,.code = "116-355"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_02011
	}
	,{
		.group = 2
		,.id = 13
	,.code = "116-355"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_02013
	}
	,{
		.group = 3
		,.id = 3
	,.code = "016-749"
	,.showMeHowFileName = "smh_print_cancell.pdf"
	,.string_index = MSG_03003
	}
	,{
		.group = 3
		,.id = 4
	,.code = "016-749"
	,.showMeHowFileName = "smh_print_cancell.pdf"
	,.string_index = MSG_03004
	}
	,{
		.group = 3
		,.id = 5
	,.code = "016-720"
	,.showMeHowFileName = "smh_print_cancell.pdf"
	,.string_index = MSG_03005
	}
	,{
		.group = 3
		,.id = 8
	,.code = "016-749"
	,.showMeHowFileName = "smh_print_cancell.pdf"
	,.string_index = MSG_03008
	}
	,{
		.group = 3
		,.id = 9
	,.code = "016-720"
	,.showMeHowFileName = "smh_print_cancell.pdf"
	,.string_index = MSG_03009
	}
	,{
		.group = 3
		,.id = 10
	,.code = "016-720"
	,.showMeHowFileName = "smh_print_cancell.pdf"
	,.string_index = MSG_03010
	}
	,{
		.group = 3
		,.id = 16
	,.code = "016-720"
	// ,.showMeHowFileName = "smh_print_cancell.pdf"
	,.string_index = MSG_03016
	}
	,{
		.group = 3
		,.id = 17
	,.code = "016-753"
	// ,.showMeHowFileName = "smh_print_cancell.pdf"
	,.string_index = MSG_03017
	}
	,{
		.group = 3
		,.id = 18
	,.code = "016-755"
	// ,.showMeHowFileName = "smh_print_cancell.pdf"
	,.string_index = MSG_03018
	}
	,{
		.group = 3
		,.id = 19
	,.code = "016-703"
	// ,.showMeHowFileName = "smh_print_cancell.pdf"
	,.string_index = MSG_03019
	}
	,{
		.group = 3
		,.id = 20
	,.code = "016-982"
	// ,.showMeHowFileName = "smh_print_cancell.pdf"
	,.string_index = MSG_03020
	}
	,{
		.group = 5
		,.id = 2
	,.code = "093-973"
	,.showMeHowFileName = "smh_detach_toner.pdf"
	,.string_index = MSG_05002
	}
	,{
		.group = 5
		,.id = 3
	,.code = "093-972"
	,.showMeHowFileName = "smh_detach_toner.pdf"
	,.string_index = MSG_05003
	}
	,{
		.group = 5
		,.id = 4
	,.code = "093-971"
	,.showMeHowFileName = "smh_detach_toner.pdf"
	,.string_index = MSG_05004
	}
	,{
		.group = 5
		,.id = 5
	,.code = "093-970"
	,.showMeHowFileName = "smh_detach_toner.pdf"
	,.string_index = MSG_05005
	}
	,{
		.group = 5
		,.id = 6
	,.code = "093-953"
	,.showMeHowFileName = "smh_detach_toner.pdf"
	,.string_index = MSG_05006
	}
	,{
		.group = 5
		,.id = 7
	,.code = "093-952"
	,.showMeHowFileName = "smh_detach_toner.pdf"
	,.string_index = MSG_05007
	}
	,{
		.group = 5
		,.id = 8
	,.code = "093-951"
	,.showMeHowFileName = "smh_detach_toner.pdf"
	,.string_index = MSG_05008
	}
	,{
		.group = 5
		,.id = 9
	,.code = "093-950"
	,.showMeHowFileName = "smh_detach_toner.pdf"
	,.string_index = MSG_05009
	}
	,{
		.group = 5
		,.id = 10
	,.code = "077-108"
	,.showMeHowFileName = "smh_jam_rear.pdf"
	,.string_index = MSG_05010
	}
	,{
		.group = 5
		,.id = 11
	,.code = "077-109"
	,.showMeHowFileName = "smh_jam_rear.pdf"
	,.string_index = MSG_05011
	}
	,{
		.group = 5
		,.id = 12
	,.code = "077-106"
	,.showMeHowFileName = "smh_jam_output.pdf"
	,.string_index = MSG_05012
	}
	,{
		.group = 5
		,.id = 13
	,.code = "077-104"
	,.showMeHowFileName = "smh_jam_rear.pdf"
	,.string_index = MSG_05013
	}
	,{
		.group = 5
		,.id = 14
	,.code = "077-100"
	,.showMeHowFileName = "smh_jam_rear.pdf"
	,.string_index = MSG_05014
	}
	,{
		.group = 5
		,.id = 15
	,.code = "075-100"
	,.showMeHowFileName = "smh_jam_front.pdf"
	,.string_index = MSG_05015
	}
	,{
		.group = 5
		,.id = 16
	,.code = "077-900"
	,.showMeHowFileName = "smh_jam_output.pdf"
	,.string_index = MSG_05016
	}
	,{
		.group = 5
		,.id = 17
	,.code = "077-901"
	,.showMeHowFileName = "smh_jam_rear.pdf"
	,.string_index = MSG_05017
	}
	,{
		.group = 5
		,.id = 18
	,.code = "092-661"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_05018
	}
	,{
		.group = 5
		,.id = 19
	,.code = "024-340"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_05019
	}
	,{
		.group = 5
		,.id = 20
	,.code = "124-333"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_05020
	}
	,{
		.group = 5
		,.id = 21
	,.code = "042-358"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_05021
	}
	,{
		.group = 5
		,.id = 22
	,.code = "092-651"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_05022
	}
	,{
		.group = 5
		,.id = 23
	,.code = "041-340"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_05023
	}
	,{
		.group = 5
		,.id = 24
	,.code = "010-397"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_05024
	}
	,{
		.group = 5
		,.id = 25
	,.code = "061-370"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_05025
	}
	,{
		.group = 5
		,.id = 71
	,.code = "091-313"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_05071
	}
	,{
		.group = 5
		,.id = 27
	,.code = "042-372"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_05027
	}
	,{
		.group = 5
		,.id = 28
	,.code = "191-311"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_05028
	}
	,{
		.group = 5
		,.id = 29
	,.code = "093-933"
	,.showMeHowFileName = "smh_replace_toner.pdf"
	,.string_index = MSG_05029
	}
	,{
		.group = 5
		,.id = 30
	,.code = "093-932"
	,.showMeHowFileName = "smh_replace_toner.pdf"
	,.string_index = MSG_05030
	}
	,{
		.group = 5
		,.id = 31
	,.code = "093-931"
	,.showMeHowFileName = "smh_replace_toner.pdf"
	,.string_index = MSG_05031
	}
	,{
		.group = 5
		,.id = 32
	,.code = "093-930"
	,.showMeHowFileName = "smh_replace_toner.pdf"
	,.string_index = MSG_05032
	}
	,{
		.group = 5
		,.id = 33
	,.code = "093-933"
	,.showMeHowFileName = "smh_replace_toner.pdf"
	,.string_index = MSG_05033
	}
	,{
		.group = 5
		,.id = 34
	,.code = "093-932"
	,.showMeHowFileName = "smh_replace_toner.pdf"
	,.string_index = MSG_05034
	}
	,{
		.group = 5
		,.id = 35
	,.code = "093-931"
	,.showMeHowFileName = "smh_replace_toner.pdf"
	,.string_index = MSG_05035
	}
	,{
		.group = 5
		,.id = 36
	,.code = "093-930"
	,.showMeHowFileName = "smh_maintin_ctd.pdf"
	,.string_index = MSG_05036
	}
	,{
		.group = 5
		,.id = 37
	,.code = "077-304"
	,.showMeHowFileName = "smh_replace_toner.pdf"
	,.string_index = MSG_05037
	}
	,{
		.group = 5
		,.id = 38
	,.code = "092-310"
	,.showMeHowFileName = "smh_replace_toner.pdf"
	,.string_index = MSG_05038
	}
	,{
		.group = 5
		,.id = 39
	,.code = "093-922"
	,.showMeHowFileName = "smh_replace_toner.pdf"
	,.string_index = MSG_05039
	}
	,{
		.group = 5
		,.id = 40
	,.code = "093-921"
	,.showMeHowFileName = "smh_replace_toner.pdf"
	,.string_index = MSG_05040
	}
	,{
		.group = 5
		,.id = 41
	,.code = "093-920"
	,.showMeHowFileName = "smh_replace_toner.pdf"
	,.string_index = MSG_05041
	}
	,{
		.group = 5
		,.id = 42
	,.code = "093-919"
	,.showMeHowFileName = "smh_replace_toner.pdf"
	,.string_index = MSG_05042
	}
	,{
		.group = 5
		,.id = 66
	,.code = "092-910"
	,.showMeHowFileName = "smh_maintin_ctd.pdf"
	,.string_index = MSG_05066
	}
	,{
		.group = 5
		,.id = 43
	,.code = "091-402"
	//        ,.showMeHowFileName = "smh_maintin_ctd.pdf"
	,.string_index = MSG_05043
	}
	,{
		.group = 5
		,.id = 44
	,.code = "093-426"
	//        ,.showMeHowFileName = "smh_maintin_ctd.pdf"
	,.string_index = MSG_05044
	}
	,{
		.group = 5
		,.id = 45
	,.code = "093-425"
	//        ,.showMeHowFileName = "smh_maintin_ctd.pdf"
	,.string_index = MSG_05045
	}
	,{
		.group = 5
		,.id = 46
	,.code = "093-424"
	//        ,.showMeHowFileName = "smh_maintin_ctd.pdf"
	,.string_index = MSG_05046
	}
	,{
		.group = 5
		,.id = 47
	,.code = "093-423"
	//        ,.showMeHowFileName = "smh_maintin_ctd.pdf"
	,.string_index = MSG_05047
	}
	,{
		.group = 5
		,.id = 48
	,.code = "191-310"
	//        ,.showMeHowFileName = "smh_maintin_ctd.pdf"
	,.string_index = MSG_05048
	}
	,{
		.group = 5
		,.id = 70
	,.code = "091-441"
	//        ,.showMeHowFileName = "smh_maintin_ctd.pdf"
	,.string_index = MSG_05070
	}
	,{
		.group = 5
		,.id = 49
	,.code = "093-426"
	//        ,.showMeHowFileName = "smh_maintin_ctd.pdf"
	,.string_index = MSG_05049
	}
	,{
		.group = 5
		,.id = 50
	,.code = "093-425"
	//        ,.showMeHowFileName = "smh_maintin_ctd.pdf"
	,.string_index = MSG_05050
	}
	,{
		.group = 5
		,.id = 51
	,.code = "093-424"
	//        ,.showMeHowFileName = "smh_maintin_ctd.pdf"
	,.string_index = MSG_05051
	}
	,{
		.group = 5
		,.id = 52
	,.code = "093-423"
	//        ,.showMeHowFileName = "smh_maintin_ctd.pdf"
	,.string_index = MSG_05052
	}
	,{
		.group = 5
		,.id = 57
	,.code = "093-926"
	,.showMeHowFileName = "smh_replace_toner.pdf"
	,.string_index = MSG_05057
	}
	,{
		.group = 5
		,.id = 58
	,.code = "093-962"
	,.showMeHowFileName = "smh_replace_toner.pdf"
	,.string_index = MSG_05058
	}
	,{
		.group = 5
		,.id = 59
	,.code = "093-961"
	,.showMeHowFileName = "smh_replace_toner.pdf"
	,.string_index = MSG_05059
	}
	,{
		.group = 5
		,.id = 60
	,.code = "093-960"
	,.showMeHowFileName = "smh_replace_toner.pdf"
	,.string_index = MSG_05060
	}
	,{
		.group = 5
		,.id = 61
	,.code = "093-933"
	,.showMeHowFileName = "smh_replace_toner.pdf"
	,.string_index = MSG_05061
	}
	,{
		.group = 5
		,.id = 62
	,.code = "093-932"
	,.showMeHowFileName = "smh_replace_toner.pdf"
	,.string_index = MSG_05062
	}
	,{
		.group = 5
		,.id = 63
	,.code = "093-931"
	,.showMeHowFileName = "smh_replace_toner.pdf"
	,.string_index = MSG_05063
	}
	,{
		.group = 5
		,.id = 64
	,.code = "093-930"
	,.showMeHowFileName = "smh_replace_toner.pdf"
	,.string_index = MSG_05064
	}
	,{
		.group = 5
		,.id = 72
	,.code = "024-946"
	//        ,.showMeHowFileName = "smh_replace_toner.pdf"
	,.string_index = MSG_05072
	}
	,{
		.group = 4
		,.id = 3
	,.code = "117-350"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_04003
	}
	,{
		.group = 4
		,.id = 4
	,.code = "024-963"
	,.showMeHowFileName = "smh_loadppr.pdf"
	,.string_index = MSG_04004
	}
	,{
		.group = 4
		,.id = 8
	,.code = "016-718"
	//      ,.showMeHowFileName ="smh_loadppr.pdf"
	,.string_index = MSG_04008
	}
	,{
		.group = 4
		,.id = 9
	,.code = "016-719"
	,.showMeHowFileName = "smh_print_cancell.pdf"
	,.string_index = MSG_04009
	}
	,{
		.group = 4
		,.id = 10
	,.code = "024-963"
	,.showMeHowFileName = "smh_loadppr.pdf"
	,.string_index = MSG_04010
	}
	,{
		.group = 4
		,.id = 11
	,.code = "024-963"
	,.showMeHowFileName = "smh_loadppr.pdf"
	,.string_index = MSG_04011
	}
	,{
		.group = 4
		,.id = 16
	,.code = "024-958"
	,.showMeHowFileName = "smh_loadppr.pdf"
	,.string_index = MSG_04016
	}
	,{
		.group = 4
		,.id = 19
	,.code = "024-963"
	,.showMeHowFileName = "smh_loadppr.pdf"
	,.string_index = MSG_04019
	}
	,{
		.group = 7
		,.id = 1
	,.code = "062-321"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_07001
	}
	,{
		.group = 7
		,.id = 6
	,.code = "005-301"
	,.showMeHowFileName = "smh_adf_open.pdf"
	,.string_index = MSG_07006
	}
	,{
		.group = 7
		,.id = 8
	,.code = "005-121"
	,.showMeHowFileName = "smh_jam_adf.pdf"
	,.string_index = MSG_07008
	}
	,{
		.group = 7
		,.id = 9
	,.code = "005-121"
	,.showMeHowFileName = "smh_jam_adf.pdf"
	,.string_index = MSG_07009
	}
	,{
		.group = 1
		,.id = 1
	,.code = "116-395"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_01001
	}
	,{
		.group = 1
		,.id = 2
	,.code = "116-210"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_01002
	}
	,{
		.group = 1
		,.id = 3
	,.code = "024-371"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_01003
	}
	,{
		.group = 1
		,.id = 4
	,.code = "134-211"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_01004
	}
	,{
		.group = 1
		,.id = 5
	,.code = "117-366"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_01005
	}
	,{
		.group = 1
		,.id = 6
	,.code = "016-317"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_01006
	}
	,{
		.group = 1
		,.id = 7
	,.code = "016-315"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_01007
	}
	,{
		.group = 1
		,.id = 8
	,.code = "062-360"
	//        ,.showMeHowFileName ="smh_app_contact.pdf"
	,.string_index = MSG_01008
	}
	,{
		.group = 1
		,.id = 9
	,.code = "024-360"
	//        ,.showMeHowFileName ="smh_app_contact.pdf"
	,.string_index = MSG_01009
	}
	,{
		.group = 1
		,.id = 30
	,.code = "016-501"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_01030
	}
	,{
		.group = 1
		,.id = 31
	,.code = "016-502"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_01031
	}
	,{
		.group = 1
		,.id = 32
	,.code = "016-744"
	//        ,.showMeHowFileName ="smh_app_contact.pdf"
	,.string_index = MSG_01032
	}
	,{
		.group = 1
		,.id = 33
	,.code = "016-745"
	//        ,.showMeHowFileName ="smh_app_contact.pdf"
	,.string_index = MSG_01033
	}
	,{
		.group = 1
		,.id = 50
	,.code = "117-331"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_01050
	}
	,{
		.group = 1
		,.id = 51
	,.code = "117-331"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_01051
	}
	,{
		.group = 1
		,.id = 52
	,.code = "117-331"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_01052
	}
	,{
		.group = 1
		,.id = 53
	,.code = "117-331"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_01053
	}
	,{
		.group = 1
		,.id = 54
	,.code = "117-331"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_01054
	}
	,{
		.group = 1
		,.id = 55
	,.code = "117-331"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_01055
	}
	,{
		.group = 1
		,.id = 56
	,.code = "117-331"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_01056
	}
	,{
		.group = 1
		,.id = 57
	,.code = "117-331"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_01057
	}
	,{
		.group = 1
		,.id = 58
	,.code = "117-332"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_01058
	}
	,{
		.group = 1
		,.id = 59
	,.code = "117-332"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_01059
	}
	,{
		.group = 1
		,.id = 60
	,.code = "117-332"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_01060
	}
	,{
		.group = 1
		,.id = 61
	,.code = "117-332"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_01061
	}
	,{
		.group = 1
		,.id = 62
	,.code = "117-333"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_01062
	}
	,{
		.group = 1
		,.id = 63
	,.code = "117-333"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_01063
	}
	,{
		.group = 1
		,.id = 64
	,.code = "117-333"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_01064
	}
	,{
		.group = 1
		,.id = 65
	,.code = "117-333"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_01065
	}
	,{
		.group = 1
		,.id = 66
	,.code = "117-334"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_01066
	}
	,{
		.group = 1
		,.id = 67
	,.code = "117-334"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_01067
	}
	,{
		.group = 1
		,.id = 68
	,.code = "117-334"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_01068
	}
	,{
		.group = 1
		,.id = 69
	,.code = "117-334"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_01069
	}
	,{
		.group = 1
		,.id = 70
	,.code = "116-323"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_01070
	}
	,{
		.group = 1
		,.id = 71
	,.code = "116-326"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_01071
	}
	,{
		.group = 1
		,.id = 72
	,.code = "116-325"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_01072
	}
	,{
		.group = 12
		,.id = 2
	,.code = "024-963"
	,.showMeHowFileName = "smh_loadppr_MPF.pdf"
	,.string_index = MSG_12002
	}
	,{
		.group = 12
		,.id = 4
	,.code = "024-963"
	,.showMeHowFileName = "smh_loadppr_MPF.pdf"
	,.string_index = MSG_12004
	}

};

static const int error_table_size = sizeof(error_table) / sizeof(error_table[0]);

static const Error_String_struct error_string_table[] =
{
	{ .string_index = MSG_10001
	,.title = "打印机错误。"
	,.mediaInfo = 0
	,.lines = 2
	,.line0 = "请关闭打印机，然后再次开机。"
	,.line1 = "如果再次出现此故障，请联系客户支持。"
    },{.string_index = MSG_10002
	,.title = "打印机错误。"
	,.mediaInfo = 0
	,.lines = 2
	,.line0 = "请关闭打印机，然后再次开机。"
    ,.line1 = "如果再次出现此故障，请联系客户支持。"
    },{.string_index = MSG_10009
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请关闭打印机，然后再次开机。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_02001
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请关闭打印机，然后再次开机。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_02004
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请关闭打印机，然后再次开机。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_02005
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请关闭打印机，然后再次开机。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_02008
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请关闭打印机，然后再次开机。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_02009
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请关闭打印机，然后再次开机。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_02010
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请关闭打印机，然后再次开机。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_02011
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请关闭打印机，然后再次开机。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_02013
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请关闭打印机，然后再次开机。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_03003
      ,.title = "发生PJL错误。"
      ,.mediaInfo = 0
      ,.lines = 1
      ,.line0 = "请按[确定按钮，以清除消息并取消当前打印作业。"
      },{.string_index = MSG_03004
      ,.title = "发生PDL仿真错误。"
      ,.mediaInfo = 0
      ,.lines = 1
      ,.line0 = "请按[确定按钮，以清除消息并取消当前打印作业。"
      },{.string_index = MSG_03005
      ,.title = "发生PDL仿真错误。"
      ,.mediaInfo = 0
      ,.lines = 1
      ,.line0 = "请按[确定按钮，以清除消息并取消当前打印作业。"
      },{.string_index = MSG_03008
      ,.title = "发生PJL错误。"
      ,.mediaInfo = 0
      ,.lines = 1
      ,.line0 = "请按[确定按钮，以清除消息并取消当前打印作业。"
      },{.string_index = MSG_03009
      ,.title = "发生PDL仿真错误。"
      ,.mediaInfo = 0
      ,.lines = 1
      ,.line0 = "请按[确定按钮，以清除消息并取消当前打印作业。"
      },{.string_index = MSG_03010
      ,.title = "发生PDL仿真错误。"
      ,.mediaInfo = 0
      ,.lines = 1
      ,.line0 = "请按[确定按钮，以清除消息并取消当前打印作业。"
      },{.string_index = MSG_03016
      ,.title = "发生PDL仿真错误。"
      ,.mediaInfo = 0
      ,.lines = 1
      ,.line0 = "请按[确定按钮，以清除消息并取消当前打印作业。"
      },{.string_index = MSG_03017
      ,.title = "发生PDL仿真错误。"
      ,.mediaInfo = 0
      ,.lines = 1
      ,.line0 = "请按[确定按钮，以清除消息并取消当前打印作业。"
      },{.string_index = MSG_03018
      ,.title = "发生PDL仿真错误。"
      ,.mediaInfo = 0
      ,.lines = 1
      ,.line0 = "请按[确定按钮，以清除消息并取消当前打印作业。"
      },{.string_index = MSG_03019
      ,.title = "发生PDL仿真错误。"
      ,.mediaInfo = 0
      ,.lines = 1
      ,.line0 = "请按[确定按钮，以清除消息并取消当前打印作业。"
      },{.string_index = MSG_03020
      ,.title = "打印机内存已满，无法继续处理当前作业。"
      ,.mediaInfo = 0
      ,.lines = 1
      ,.line0 = "请按[确定按钮，以清除消息并取消当前打印作业。"
      },{.string_index = MSG_05002
      ,.title = "黑色墨粉筒缺失，或未完全插入打印机中。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请打开墨粉存取盖并确保黑色墨粉筒已安装好。"
      ,.line1 = "有关详细信息，请单击[显示操作方式]按钮。"
      },{.string_index = MSG_05003
      ,.title = "青色墨粉筒缺失，或未完全插入打印机中。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请打开墨粉存取盖并确保青色墨粉筒已安装好。"
      ,.line1 = "有关详细信息，请单击[显示操作方式]按钮。"
      },{.string_index = MSG_05004
      ,.title = "洋红色墨粉筒缺失，或未完全插入打印机中。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请打开墨粉存取盖并确保洋红色墨粉筒已安装好。"
      ,.line1 = "有关详细信息，请单击[显示操作方式]按钮。"
      },{.string_index = MSG_05005
      ,.title = "黄色墨粉筒缺失，或未完全插入打印机中。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请打开墨粉存取盖并确保黄色墨粉筒已安装好。"
      ,.line1 = "有关详细信息，请单击[显示操作方式]按钮。"
      },{.string_index = MSG_05006
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 3
      ,.line0 = "请关闭打印机，并确认黑色墨粉筒安装正确。"
      ,.line1 = "启动打印机。"
      ,.line2 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_05007
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 3
      ,.line0 = "请关闭打印机，并确认青色墨粉筒安装正确。"
      ,.line1 = "启动打印机。"
      ,.line2 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_05008
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 3
      ,.line0 = "请关闭打印机，并确认洋红色墨粉筒安装正确。"
      ,.line1 = "启动打印机。"
      ,.line2 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_05009
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 3
      ,.line0 = "请关闭打印机，并确认黄色墨粉筒安装正确。"
      ,.line1 = "启动打印机。"
      ,.line2 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_05010
      ,.title = "出现卡纸。"
      ,.mediaInfo = 0
      ,.lines = 3
      ,.line0 = "拉出纸盘，取出卡纸。"
      ,.line1 = "打开后盖，提起定影组件释放杆，然后取出卡纸。请关闭后盖。"
      ,.line2 = "有关详细信息，请单击[显示操作方式]按钮。"
      },{.string_index = MSG_05011
      ,.title = "出现卡纸。"
      ,.mediaInfo = 0
      ,.lines = 3
      ,.line0 = "拉出纸盘，取出卡纸。"
      ,.line1 = "打开后盖，提起定影组件释放杆，然后取出卡纸。请关闭后盖。"
      ,.line2 = "有关详细信息，请单击[显示操作方式]按钮。"
      },{.string_index = MSG_05012
      ,.title = "出现卡纸。"
      ,.mediaInfo = 0
      ,.lines = 3
      ,.line0 = "拉出纸盘，取出卡纸。"
      ,.line1 = "打开后盖，提起定影组件释放杆，然后取出卡纸。请关闭后盖。"
      ,.line2 = "有关详细信息，请单击[显示操作方式]按钮。"
      },{.string_index = MSG_05013
      ,.title = "出现卡纸。"
      ,.mediaInfo = 0
      ,.lines = 3
      ,.line0 = "拉出纸盘，取出卡纸。"
      ,.line1 = "打开后盖，提起定影组件释放杆，然后取出卡纸。请关闭后盖。"
      ,.line2 = "有关详细信息，请单击[显示操作方式]按钮。"
      },{.string_index = MSG_05014
      ,.title = "出现卡纸。"
      ,.mediaInfo = 0
      ,.lines = 3
      ,.line0 = "拉出纸盘，取出卡纸。"
      ,.line1 = "打开后盖，提起定影组件释放杆，然后取出卡纸。请关闭后盖。"
      ,.line2 = "有关详细信息，请单击[显示操作方式]按钮。"
      },{.string_index = MSG_05015
      ,.title = "出现卡纸。"
      ,.mediaInfo = 1
      ,.lines = 5
      ,.line0 = "拉出纸盘。"
      ,.line1 = "取出发生送纸错误的纸张。"
      ,.line2 = "有关详细信息，请单击[显示操作方式]按钮。"
      ,.line3 = "纸张尺寸： "
      ,.line4 = "纸张类型： "
      },{.string_index = MSG_05016
      ,.title = "出现卡纸。"
      ,.mediaInfo = 0
      ,.lines = 3
      ,.line0 = "拉出纸盘，取出卡纸。"
      ,.line1 = "打开后盖，提起定影组件释放杆，然后取出卡纸。请关闭后盖。"
      ,.line2 = "有关详细信息，请单击[显示操作方式]按钮。"
      },{.string_index = MSG_05017
      ,.title = "出现卡纸。"
      ,.mediaInfo = 0
      ,.lines = 3
      ,.line0 = "拉出纸盘，取出卡纸。"
      ,.line1 = "打开后盖，提起定影组件释放杆，然后取出卡纸。请关闭后盖。"
      ,.line2 = "有关详细信息，请单击[显示操作方式]按钮。"
      },{.string_index = MSG_05018
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请关闭打印机，然后再次开机。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_05019
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请关闭打印机，然后再次开机。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_05020
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请关闭打印机，然后再次开机。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_05021
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请关闭打印机，然后再次开机。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_05022
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请关闭打印机，然后再次开机。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_05023
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请关闭打印机，然后再次开机。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_05024
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请关闭打印机，然后再次开机。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_05025
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请关闭打印机，然后再次开机。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_05071
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请关闭打印机，然后再次开机。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_05027
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请关闭打印机，然后再次开机。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_05028
      ,.title = "打印机使用寿命到期。"
      ,.mediaInfo = 0
      ,.lines = 1
      ,.line0 = "无法再使用。"
      },{.string_index = MSG_05029
      ,.title = "更换黑色墨粉筒。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请打开墨粉存取盖。接着，取出旧的黑色墨粉筒并安装新的。"
      ,.line1 = "有关详细信息，请单击[显示操作方式]按钮。"
      },{.string_index = MSG_05030
      ,.title = "更换青色墨粉筒。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请打开墨粉存取盖。接着，取出旧的青色墨粉筒并安装新的。"
      ,.line1 = "有关详细信息，请单击[显示操作方式]按钮。"
      },{.string_index = MSG_05031
      ,.title = "更换洋红色墨粉筒。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请打开墨粉存取盖。接着，取出旧的洋红色墨粉筒并安装新的。"
      ,.line1 = "有关详细信息，请单击[显示操作方式]按钮。"
      },{.string_index = MSG_05032
      ,.title = "更换黄色墨粉筒。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请打开墨粉存取盖。接着，取出旧的黄色墨粉筒并安装新的。"
      ,.line1 = "有关详细信息，请单击[显示操作方式]按钮。"
      },{.string_index = MSG_05033
      ,.title = "更换黑色墨粉筒。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请打开墨粉存取盖。接着，取出旧的黑色墨粉筒并安装新的。"
      ,.line1 = "有关详细信息，请单击[显示操作方式]按钮。"
      },{.string_index = MSG_05034
      ,.title = "更换青色墨粉筒。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请打开墨粉存取盖。接着，取出旧的青色墨粉筒并安装新的。"
      ,.line1 = "有关详细信息，请单击[显示操作方式]按钮。"
      },{.string_index = MSG_05035
      ,.title = "更换洋红色墨粉筒。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请打开墨粉存取盖。接着，取出旧的洋红色墨粉筒并安装新的。"
      ,.line1 = "有关详细信息，请单击[显示操作方式]按钮。"
      },{.string_index = MSG_05036
      ,.title = "更换黄色墨粉筒。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请打开墨粉存取盖。接着，取出旧的黄色墨粉筒并安装新的。"
      ,.line1 = "有关详细信息，请单击[显示操作方式]按钮。"
      },{.string_index = MSG_05037
      ,.title = "后盖开着。"
      ,.mediaInfo = 0
      ,.lines = 1
      ,.line0 = "请关闭后盖。"
      },{.string_index = MSG_05038
      ,.title = "CTD传感器已脏。"
      ,.mediaInfo = 0
      ,.lines = 1
      ,.line0 = "请清洁CTD传感器。有关详细信息，请单击[显示操作方式]按钮。"
      },{.string_index = MSG_05039
      ,.title = "黑色墨粉低浓度"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请打开墨粉存取盖。取出黑色墨粉筒并摇晃。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_05040
      ,.title = "青色墨粉低浓度"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请打开墨粉存取盖。取出青色墨粉筒并摇晃。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_05041
      ,.title = "洋红色墨粉低浓度"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请打开墨粉存取盖。取出洋红色墨粉筒并摇晃。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_05042
      ,.title = "黄色墨粉低浓度"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请打开墨粉存取盖。取出黄色墨粉筒并摇晃。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_05066
      ,.title = "CTD传感器已脏。请清洁CTD传感器。"
      ,.mediaInfo = 0
      ,.lines = 0
      },{.string_index = MSG_05043
      ,.title = "打印机使用寿命即将到期。请参考使用手册。"
      ,.mediaInfo = 0
      ,.lines = 0
      },{.string_index = MSG_05044
      ,.title = "黑色墨粉筒需要尽快更换。"
      ,.mediaInfo = 0
      ,.lines = 0
      },{.string_index = MSG_05045
      ,.title = "青色墨粉筒需要尽快更换。"
      ,.mediaInfo = 0
      ,.lines = 0
      },{.string_index = MSG_05046
      ,.title = "洋红色墨粉筒需要尽快更换。"
      ,.mediaInfo = 0
      ,.lines = 0
      },{.string_index = MSG_05047
      ,.title = "黄色墨粉筒需要尽快更换。"
      ,.mediaInfo = 0
      ,.lines = 0
      },{.string_index = MSG_05048
      ,.title = "打印机使用寿命到期。"
      ,.mediaInfo = 0
      ,.lines = 1
      ,.line0 = "要继续打印，请参考使用手册。"
      },{.string_index = MSG_05070
      ,.title = "打印就绪。可以使用打印机。"
      ,.mediaInfo = 0
      ,.lines = 0
      },{.string_index = MSG_05049
      ,.title = "黑色墨粉筒需要尽快更换。"
      ,.mediaInfo = 0
      ,.lines = 0
      },{.string_index = MSG_05050
      ,.title = "青色墨粉筒需要尽快更换。"
      ,.mediaInfo = 0
      ,.lines = 0
      },{.string_index = MSG_05051
      ,.title = "洋红色墨粉筒需要尽快更换。"
      ,.mediaInfo = 0
      ,.lines = 0
      },{.string_index = MSG_05052
      ,.title = "黄色墨粉筒需要尽快更换。"
      ,.mediaInfo = 0
      ,.lines = 0
      },{.string_index = MSG_05057
      ,.title = "安装的黑色墨粉筒不受支持。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请打开墨粉存取盖。取出不支持的黑色墨粉筒，再安装受支持的墨粉筒。"
      ,.line1 = "有关详细信息，请单击[显示操作方式]按钮。"
      },{.string_index = MSG_05058
      ,.title = "安装的青色墨粉筒不受支持。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请打开墨粉存取盖。取出不支持的青色墨粉筒，再安装受支持的墨粉筒。"
      ,.line1 = "有关详细信息，请单击[显示操作方式]按钮。"
      },{.string_index = MSG_05059
      ,.title = "安装的洋红色墨粉筒不受支持。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请打开墨粉存取盖。取出不支持的洋红色墨粉筒，再安装受支持的墨粉筒。"
      ,.line1 = "有关详细信息，请单击[显示操作方式]按钮。"
      },{.string_index = MSG_05060
      ,.title = "安装的黄色墨粉筒不受支持。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请打开墨粉存取盖。取出不支持的黄色墨粉筒，再安装受支持的墨粉筒。"
      ,.line1 = "有关详细信息，请单击[显示操作方式]按钮。"
      },{.string_index = MSG_05061
      ,.title = "更换黑色墨粉筒。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请打开墨粉存取盖。接着，取出旧的黑色墨粉筒并安装新的。"
      ,.line1 = "有关详细信息，请单击[显示操作方式]按钮。"
      },{.string_index = MSG_05062
      ,.title = "更换青色墨粉筒。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请打开墨粉存取盖。接着，取出旧的青色墨粉筒并安装新的。"
      ,.line1 = "有关详细信息，请单击[显示操作方式]按钮。"
      },{.string_index = MSG_05063
      ,.title = "更换洋红色墨粉筒。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请打开墨粉存取盖。接着，取出旧的洋红色墨粉筒并安装新的。"
      ,.line1 = "有关详细信息，请单击[显示操作方式]按钮。"
      },{.string_index = MSG_05064
      ,.title = "更换黄色墨粉筒。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请打开墨粉存取盖。接着，取出旧的黄色墨粉筒并安装新的。"
      ,.line1 = "有关详细信息，请单击[显示操作方式]按钮。"
      },{.string_index = MSG_05072
      ,.title = "未插入纸盘。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "拉出纸盘。"
      ,.line1 = "检查设备中发生送纸错误的纸张。 取出纸张然后插入纸盘。"
      },{.string_index = MSG_04003
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "打开后盖，然后关闭以继续。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_04004
      ,.title = "未检测到纸张。"
      ,.mediaInfo = 1
      ,.lines = 4
      ,.line0 = "拉出纸盘。"
      ,.line1 = "在纸盘中装入纸张。"
      ,.line2 = "纸张尺寸： "
      ,.line3 = "纸张类型： "
      },{.string_index = MSG_04008
      ,.title = "打印机内存已满，无法继续处理当前作业。"
      ,.mediaInfo = 0
      ,.lines = 1
      ,.line0 = "请按[确定按钮，以清除消息并取消当前打印作业。"
      },{.string_index = MSG_04009
      ,.title = "发生打印机错误。无法继续处理当前的打印作业。"
      ,.mediaInfo = 0
      ,.lines = 1
      ,.line0 = "请按[确定按钮，以清除消息并取消当前打印作业。"
      },{.string_index = MSG_04010
      ,.title = "指定的纸张不可用。"
      ,.mediaInfo = 1
      ,.lines = 3
      ,.line0 = "请在纸盘中装入指定的纸张。"
      ,.line1 = "纸张尺寸： "
      ,.line2 = "纸张类型： "
      },{.string_index = MSG_04011
      ,.title = "指定的纸张不可用。"
      ,.mediaInfo = 1
      ,.lines = 3
      ,.line0 = "请在纸盘中装入指定的纸张。"
      ,.line1 = "纸张尺寸： "
      ,.line2 = "纸张类型： "
      },{.string_index = MSG_04016
      ,.title = "纸盘中的实际纸张尺寸与指定的纸张尺寸不同。"
      ,.mediaInfo = 1
      ,.lines = 3
      ,.line0 = "请在纸盘中装入指定的纸张。"
      ,.line1 = "纸张尺寸： "
      ,.line2 = "纸张类型： "
      },{.string_index = MSG_04019
      ,.title = "指定的纸张不可用。"
      ,.mediaInfo = 0
      ,.lines = 1
      ,.line0 = "请在纸盘中装入A4、Letter或Legal尺寸的纸张。"
      },{.string_index = MSG_07001
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请关闭打印机，然后再次开机。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_07006
      ,.title = "ADF盖子开着。"
      ,.mediaInfo = 0
      ,.lines = 1
      ,.line0 = "请关闭ADF盖子。"
      },{.string_index = MSG_07008
      ,.title = "ADF中发生卡纸。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请从ADF中取出剩余原稿。"
      ,.line1 = "有关详细信息，请单击[显示操作方式]按钮。"
      },{.string_index = MSG_07009
      ,.title = "ADF中发生卡纸。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请从ADF中取出剩余原稿。"
      ,.line1 = "有关详细信息，请单击[显示操作方式]按钮。"
      },{.string_index = MSG_01001
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请关闭打印机，然后再次开机。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_01002
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请关闭打印机，然后再次开机。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_01003
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请关闭打印机，然后再次开机。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_01004
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请关闭打印机，然后再次开机。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_01005
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请关闭打印机，然后再次开机。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_01006
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请关闭打印机，然后再次开机。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_01007
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请关闭打印机，然后再次开机。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_01008
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请关闭打印机，然后再次开机。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_01009
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请关闭打印机，然后再次开机。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_01030
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请关闭打印机，然后再次开机。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_01031
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请关闭打印机，然后再次开机。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_01032
      ,.title = "发生固件下载格式错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请按[确定按钮。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_01033
      ,.title = "发生固件下载格式错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请按[确定按钮。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_01050
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请关闭打印机，然后再次开机。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_01051
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请关闭打印机，然后再次开机。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_01052
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请关闭打印机，然后再次开机。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_01053
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请关闭打印机，然后再次开机。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_01054
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请关闭打印机，然后再次开机。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_01055
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请关闭打印机，然后再次开机。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_01056
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请关闭打印机，然后再次开机。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_01057
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请关闭打印机，然后再次开机。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_01058
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请关闭打印机，然后再次开机。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_01059
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请关闭打印机，然后再次开机。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_01060
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请关闭打印机，然后再次开机。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_01061
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请关闭打印机，然后再次开机。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_01062
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请关闭打印机，然后再次开机。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_01063
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请关闭打印机，然后再次开机。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_01064
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请关闭打印机，然后再次开机。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_01065
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请关闭打印机，然后再次开机。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_01066
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请关闭打印机，然后再次开机。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_01067
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请关闭打印机，然后再次开机。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_01068
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请关闭打印机，然后再次开机。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_01069
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请关闭打印机，然后再次开机。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_01070
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请关闭打印机，然后再次开机。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_01071
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请关闭打印机，然后再次开机。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_01072
      ,.title = "打印机错误。"
      ,.mediaInfo = 0
      ,.lines = 2
      ,.line0 = "请关闭打印机，然后再次开机。"
      ,.line1 = "如果再次出现此故障，请联系客户支持。"
      },{.string_index = MSG_12002
      ,.title = "指定的纸张不可用。"
      ,.mediaInfo = 0
      ,.lines = 1
      ,.line0 = "请在纸盘中装入A4或Letter尺寸的纸张。"
      },{.string_index = MSG_12004
      ,.title = "指定的纸张不可用。"
      ,.mediaInfo = 0
      ,.lines = 1
      ,.line0 = "请在纸盘中装入A4或Letter尺寸的纸张。"
      }
};

static const int error_string_table_size = sizeof(error_string_table) / sizeof(error_string_table[0]);

void PaperSize2Str(int bPaperSize, char* szPaperSize);
void PaperType2Str(int bPaperType, char* szPaperType);
ErrorInfo_struct getErrorInfo(int group ,int id ,int paperType ,int paperSize)
{
    ErrorInfo_struct errorInfo;
    memset((void*)&errorInfo ,0 ,sizeof(errorInfo));
    const Error_struct* p_error;
    const Error_String_struct* p_errorString;
    int i ,j;
    for(i = 0 ;i < error_table_size ;i++){
        p_error = &error_table[i];
        if(p_error->group == group && p_error->id == id){
            errorInfo.error = p_error;
            for(j = 0 ;j < error_string_table_size ;j++){
                p_errorString = &error_string_table[j];
                if(p_errorString->string_index == p_error->string_index){
                    errorInfo.errorString = p_errorString;
                    if(!paperType)
                        break;
                    if(p_errorString->lines < 3)
                        break;
                    if(p_errorString->mediaInfo){
                        PaperSize2Str(paperSize ,errorInfo.paperSizeString);
                        PaperType2Str(paperType ,errorInfo.paperTypeString);
                    }
                    break;
                }
            }
            break;
        }
    }
    return errorInfo;
}
#ifdef __cplusplus
}
#endif
