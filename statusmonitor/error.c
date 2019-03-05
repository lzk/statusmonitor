#include "error.h"
//#include "statusmonitor.h"
#include <string.h>
#ifdef __cplusplus
extern "C" {
#endif

	enum {
		MSG_01001,
		MSG_01003,
		MSG_01009,
		MSG_01030,
		MSG_01032,
		MSG_02008,
		MSG_02009,
		MSG_02011,
		MSG_02020,
		MSG_03009,
		MSG_03016,
		MSG_04004,
		MSG_04009,
		MSG_04010,
		MSG_04011,
		MSG_04016,
		MSG_04020,
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
        MSG_05026,
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
		MSG_05043,
		MSG_05044,
		MSG_05045,
		MSG_05046,
		MSG_05072,
	};

static const Error_struct error_table[] =
{
	{
		.group = 01
		,.id = 1
	,.code = "541-001"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_01001
	}
	,{
		.group = 01
		,.id = 3
	,.code = "541-003"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_01003
	}
	,{
		.group = 01
		,.id = 9
	,.code = "541-009"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_01009
	}
	,{
		.group = 01
		,.id = 30
	,.code = "541-030"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_01030
	}
	,{
		.group = 01
		,.id = 32
	,.code = "541-032"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_01032
	}
	,{
		.group = 02
		,.id = 8
	,.code = "542-008"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_02008
	}
	,{
		.group = 02
		,.id = 9
	,.code = "542-009"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_02009
	}
	,{
		.group = 02
		,.id = 11
	,.code = "542-011"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_02011
	}
	,{
		.group = 02
		,.id = 20
	,.code = "542-020"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_02020
	}
	,{
		.group = 03
		,.id = 9
	,.code = "543-009"
	,.showMeHowFileName = "smh_print_cancell.pdf"
	,.string_index = MSG_03009
	}
	,{
		.group = 03
		,.id = 16
	,.code = "543-016"
	,.showMeHowFileName = "smh_print_cancell.pdf"
	,.string_index = MSG_03016
	}
	,{
		.group = 04
		,.id = 4
	,.code = "544-004"
	,.showMeHowFileName = "smh_loadppr.pdf"
	,.string_index = MSG_04004
    }
	,{
		.group = 04
		,.id = 9
	,.code = "544-009"
	,.showMeHowFileName = "smh_print_cancell.pdf"
	,.string_index = MSG_04009
	}
	,{
		.group = 04
		,.id = 10
	,.code = "544-010"
	,.showMeHowFileName = "smh_loadppr.pdf"
	,.string_index = MSG_04010
	}
	,{
		.group = 04
		,.id = 11
	,.code = "544-011"
	,.showMeHowFileName = "smh_loadppr.pdf"
	,.string_index = MSG_04011
	}
	,{
		.group = 04
		,.id = 16
	,.code = "544-016"
	,.showMeHowFileName = "smh_loadppr.pdf"
	,.string_index = MSG_04016
	}
	,{
		.group = 04
		,.id = 20
	,.code = "544-020"
	,.showMeHowFileName = "smh_loadppr.pdf"
	,.string_index = MSG_04020
	}
	,{
		.group = 05
		,.id = 2
	,.code = "545-002"
	,.showMeHowFileName = "smh_detach_toner.pdf"
	,.string_index = MSG_05002
	}
	,{
		.group = 05
		,.id = 3
	,.code = "545-003"
	,.showMeHowFileName = "smh_detach_toner.pdf"
	,.string_index = MSG_05003
	}
	,{
		.group = 05
		,.id = 4
	,.code = "545-004"
	,.showMeHowFileName = "smh_detach_toner.pdf"
	,.string_index = MSG_05004
	}
	,{
		.group = 05
		,.id = 5
	,.code = "545-005"
	,.showMeHowFileName = "smh_detach_toner.pdf"
	,.string_index = MSG_05005
	}
	,{
		.group = 05
		,.id = 6
	,.code = "545-006"
	,.showMeHowFileName = "smh_detach_toner.pdf"
	,.string_index = MSG_05006
	}
	,{
		.group = 05
		,.id = 7
	,.code = "545-007"
	,.showMeHowFileName = "smh_detach_toner.pdf"
	,.string_index = MSG_05007
	}
	,{
		.group = 05
		,.id = 8
	,.code = "545-008"
	,.showMeHowFileName = "smh_detach_toner.pdf"
	,.string_index = MSG_05008
	}
	,{
		.group = 05
		,.id = 9
	,.code = "545-009"
	,.showMeHowFileName = "smh_detach_toner.pdf"
	,.string_index = MSG_05009
	}
    ,{
        .group = 05
        ,.id = 10
    ,.code = "545-010"
    ,.showMeHowFileName = "smh_detach_toner.pdf"
    ,.string_index = MSG_05010
    }
	,{
		.group = 05
		,.id = 11
	,.code = "545-011"
	,.showMeHowFileName = "smh_jam_rear.pdf"
	,.string_index = MSG_05011
	}
	,{
		.group = 05
		,.id = 12
	,.code = "545-012"
	,.showMeHowFileName = "smh_jam_output.pdf"
	,.string_index = MSG_05012
	}
	,{
		.group = 05
		,.id = 13
	,.code = "545-013"
	,.showMeHowFileName = "smh_jam_rear.pdf"
	,.string_index = MSG_05013
	}
	,{
		.group = 05
		,.id = 14
	,.code = "545-014"
	,.showMeHowFileName = "smh_jam_rear.pdf"
	,.string_index = MSG_05014
	}
	,{
		.group = 05
		,.id = 15
	,.code = "545-015"
	,.showMeHowFileName = "smh_jam_front.pdf"
	,.string_index = MSG_05015
	}
	,{
		.group = 05
		,.id = 16
	,.code = "545-016"
	,.showMeHowFileName = "smh_jam_output.pdf"
	,.string_index = MSG_05016
	}
	,{
		.group = 05
		,.id = 17
	,.code = "545-017"
	,.showMeHowFileName = "smh_jam_rear.pdf"
	,.string_index = MSG_05017
	}
	,{
		.group = 05
		,.id = 18
	,.code = "545-018"
	,.showMeHowFileName = "smh_app_contact.pdf"
	,.string_index = MSG_05018
	}
    ,{
        .group = 05
        ,.id = 19
    ,.code = "545-019"
    ,.showMeHowFileName = "smh_app_contact.pdf"
    ,.string_index = MSG_05019
    }
    ,{
        .group = 05
        ,.id = 20
    ,.code = "545-020"
    ,.showMeHowFileName = "smh_app_contact.pdf"
    ,.string_index = MSG_05020
    }
    ,{
        .group = 05
        ,.id = 21
    ,.code = "545-021"
    ,.showMeHowFileName = "smh_app_contact.pdf"
    ,.string_index = MSG_05021
    }
    ,{
        .group = 05
        ,.id = 22
    ,.code = "545-022"
    ,.showMeHowFileName = "smh_app_contact.pdf"
    ,.string_index = MSG_05022
    }
    ,{
        .group = 05
        ,.id = 23
    ,.code = "545-023"
    ,.showMeHowFileName = "smh_app_contact.pdf"
    ,.string_index = MSG_05023
    }
    ,{
        .group = 05
        ,.id = 24
    ,.code = "545-024"
    ,.showMeHowFileName = "smh_app_contact.pdf"
    ,.string_index = MSG_05024
    }
    ,{
        .group = 05
        ,.id = 25
    ,.code = "545-025"
    ,.showMeHowFileName = "smh_app_contact.pdf"
    ,.string_index = MSG_05025
    }
    ,{
        .group = 05
        ,.id = 26
    ,.code = "545-026"
    ,.showMeHowFileName = "smh_app_contact.pdf"
    ,.string_index = MSG_05026
    }
    ,{
        .group = 05
        ,.id = 27
    ,.code = "545-027"
    ,.showMeHowFileName = "smh_app_contact.pdf"
    ,.string_index = MSG_05027
    }
    ,{
        .group = 05
        ,.id = 28
    ,.code = "545-028"
    ,.showMeHowFileName = "smh_app_contact.pdf"
    ,.string_index = MSG_05028
    }
    ,{
        .group = 05
        ,.id = 29
    ,.code = "545-029"
    ,.showMeHowFileName = "smh_app_contact.pdf"
    ,.string_index = MSG_05029
    }
    ,{
        .group = 05
        ,.id = 30
    ,.code = "545-030"
    ,.showMeHowFileName = "smh_app_contact.pdf"
    ,.string_index = MSG_05030
    }
    ,{
        .group = 05
        ,.id = 31
    ,.code = "545-031"
    ,.showMeHowFileName = "smh_app_contact.pdf"
    ,.string_index = MSG_05031
    }
    ,{
        .group = 05
        ,.id = 32
    ,.code = "545-032"
    ,.showMeHowFileName = "smh_app_contact.pdf"
    ,.string_index = MSG_05032
    }
	,{
		.group = 05
		,.id = 33
	,.code = "545-033"
	,.showMeHowFileName = "smh_replace_toner.pdf"
	,.string_index = MSG_05033
	}
	,{
		.group = 05
		,.id = 34
	,.code = "545-034"
	,.showMeHowFileName = "smh_replace_toner.pdf"
	,.string_index = MSG_05034
	}
	,{
		.group = 05
		,.id = 35
	,.code = "545-035"
	,.showMeHowFileName = "smh_replace_toner.pdf"
	,.string_index = MSG_05035
	}
	,{
		.group = 05
		,.id = 36
	,.code = "545-036"
	,.showMeHowFileName = "smh_replace_toner.pdf"
	,.string_index = MSG_05036
	}
	,{
		.group = 05
		,.id = 37
	,.code = "545-037"
	,.showMeHowFileName = "smh_replace_toner.pdf"
	,.string_index = MSG_05037
	}
	,{
		.group = 05
		,.id = 43
	,.code = "545-043"
	,.showMeHowFileName = "smh_replace_toner.pdf"
	,.string_index = MSG_05043
	}
	,{
		.group = 05
		,.id = 44
	,.code = "545-044"
	,.showMeHowFileName = "smh_replace_toner.pdf"
	,.string_index = MSG_05044
	}
	,{
		.group = 05
		,.id = 46
	,.code = "545-046"
	,.showMeHowFileName = "smh_replace_toner.pdf"
	,.string_index = MSG_05046
	}
	,{
		.group = 05
		,.id = 72
	,.code = "545-072"
	,.showMeHowFileName = "smh_replace_toner.pdf"
	,.string_index = MSG_05072
	}
    ,{
        .group = 05
        ,.id = 73
    ,.code = "544-004"
    ,.showMeHowFileName = "smh_loadppr.pdf"
    ,.string_index = MSG_04004
    }
};


static const int error_table_size = sizeof(error_table) / sizeof(error_table[0]);

static const Error_String_struct error_string_table[] =
{
	{
		.string_index = MSG_01001
		,.title = "打印机错误。"
	,.mediaInfo = 0
	,.lines = 2
    ,.line0 = "关闭打印机，然后再次打开它。"
	,.line1 = "如果重复此故障, 请与客户支持联系。"
	}
	,{
		.string_index = MSG_01003
		,.title = "打印机错误。"
	,.mediaInfo = 0
    ,.lines = 2
        ,.line0 = "关闭打印机，然后再次打开它。"
	,.line1 = "如果重复此故障, 请与客户支持联系。"
	}
	,{
		.string_index = MSG_01009
		,.title = "打印机错误。"
	,.mediaInfo = 0
    ,.lines = 2
        ,.line0 = "关闭打印机，然后再次打开它。"
	,.line1 = "如果重复此故障, 请与客户支持联系。"
	}
	,{
		.string_index = MSG_01030
		,.title = "打印机错误。"
	,.mediaInfo = 0
    ,.lines = 2
        ,.line0 = "关闭打印机，然后再次打开它。"
	,.line1 = "如果重复此故障, 请与客户支持联系。"
	}
	,{
		.string_index = MSG_01032
        ,.title = "出现固件下载格式错误。"
	,.mediaInfo = 0
	,.lines = 2
	,.line0 = "按 [确定] 按钮。"
	,.line1 = "如果重复此故障, 请与客户支持联系。"
	}
	,{
		.string_index = MSG_02008
		,.title = "打印机错误。"
	,.mediaInfo = 0
    ,.lines = 2
        ,.line0 = "关闭打印机，然后再次打开它。"
	,.line1 = "如果重复此故障, 请与客户支持联系。"
	}
	,{
		.string_index = MSG_02009
		,.title = "打印机错误。"
	,.mediaInfo = 0
    ,.lines = 2
        ,.line0 = "关闭打印机，然后再次打开它。"
	,.line1 = "如果重复此故障, 请与客户支持联系。"
	}
	,{
		.string_index = MSG_02011
		,.title = "打印机错误。"
	,.mediaInfo = 0
    ,.lines = 2
        ,.line0 = "关闭打印机，然后再次打开它。"
	,.line1 = "如果重复此故障, 请与客户支持联系。"
	}
	,{
		.string_index = MSG_02020
		,.title = "打印机错误。"
	,.mediaInfo = 0
    ,.lines = 2
        ,.line0 = "关闭打印机，然后再次打开它。"
	,.line1 = "如果重复此故障, 请与客户支持联系。"
	}
	,{
		.string_index = MSG_03009
		,.title = "发生PDL仿真错误。"
	,.mediaInfo = 0
	,.lines = 1
    ,.line0 = "按 [确定] 按钮清除错误并取消当前打印作业。"
	}
	,{
		.string_index = MSG_03016
		,.title = "发生PDL仿真错误。"
	,.mediaInfo = 0
	,.lines = 1
    ,.line0 = "按 [确定] 按钮清除错误并取消当前打印作业。"
	}
	,{
		.string_index = MSG_04004
		,.title = "未检测到纸张。"
	,.mediaInfo = 1
	,.lines = 4
	,.line0 = "拉出纸盒。"
	,.line1 = "设置纸张和加载纸盒。"
	,.line2 = "纸张大小:"
    ,.line3 = "纸张类型:"
	}
	,{
		.string_index = MSG_04009
		,.title = "发生打印机错误。无法继续处理当前打印作业。"
	,.mediaInfo = 0
	,.lines = 1
	,.line0 = "按 [确定] 按钮清除邮件并取消当前打印作业。"
	}
	,{
		.string_index = MSG_04010
		,.title = "指定的纸张不可用。"
	,.mediaInfo = 1
	,.lines = 3
    ,.line0 = "将指定的纸张装入纸盒。"
	,.line1 = "纸张大小:"
    ,.line2 = "纸张类型:"
	}
	,{
		.string_index = MSG_04011
		,.title = "指定的纸张不可用。"
	,.mediaInfo = 1
    ,.lines = 3
        ,.line0 = "将指定的纸张装入纸盒。"
	,.line1 = "纸张大小:"
	,.line2 = "纸张类型："
	}
	,{
		.string_index = MSG_04016
		,.title = "纸盒中纸张的实际纸张大小与指定的纸张大小不同。"
	,.mediaInfo = 1
    ,.lines = 3
        ,.line0 = "将指定的纸张装入纸盒。"
	,.line1 = "纸张大小:"
    ,.line2 = "纸张类型:"
	}
	,{
		.string_index = MSG_04020
		,.title = "指定的纸张不可用。"
	,.mediaInfo = 0
	,.lines = 1
	,.line0 = "在纸盒中加载 A4 或Letter。"
	}
	,{
		.string_index = MSG_05002
        ,.title = "黑色粉仓丢失或未完全插入打印机。"
	,.mediaInfo = 0
    ,.lines = 1
    ,.line0 = "打开碳粉盒访问盖并确保已完全安装黑色粉仓。"
//	,.line1 = "有关详细信息, 请单击 [显示我如何] 按钮。"
	}
	,{
		.string_index = MSG_05003
        ,.title = "青色粉仓丢失或未完全插入打印机。"
	,.mediaInfo = 0
    ,.lines = 1
    ,.line0 = "打开碳粉盒访问盖并确保已完全安装青色粉仓。"
//	,.line1 = "有关详细信息, 请单击 [显示我如何] 按钮。"
	}
	,{
		.string_index = MSG_05004
        ,.title = "品红色粉仓丢失或未完全插入打印机。"
	,.mediaInfo = 0
    ,.lines = 1
    ,.line0 = "打开碳粉盒访问盖并确保已完全安装了品红色粉仓。"
//	,.line1 = "有关详细信息, 请单击 [显示我如何] 按钮。"
	}
	,{
		.string_index = MSG_05005
        ,.title = "黄色粉仓丢失或未完全插入打印机。"
	,.mediaInfo = 0
    ,.lines = 1
    ,.line0 = "打开碳粉盒访问盖并确保已完全安装黄色粉仓。"
//	,.line1 = "有关详细信息, 请单击 [显示我如何] 按钮。"
	}
	,{
		.string_index = MSG_05006
		,.title = "出现卡纸。"
	,.mediaInfo = 0
    ,.lines = 3
	,.line0 = "拉出纸盒, 取出卡纸。"
    ,.line1 = "打开盖门, 提起熔断单元松开拉杆, "
    ,.line2 = "取出卡纸。关闭盖门。"
//	,.line2 = "有关详细信息, 请单击 [显示我如何] 按钮。"
	}
	,{
		.string_index = MSG_05007
		,.title = "出现卡纸。"
        ,.mediaInfo = 0
        ,.lines = 3
        ,.line0 = "拉出纸盒, 取出卡纸。"
        ,.line1 = "打开盖门, 提起熔断单元松开拉杆, "
        ,.line2 = "取出卡纸。关闭盖门。"
//	,.line2 = "有关详细信息, 请单击 [显示我如何] 按钮。"
	}
	,{
		.string_index = MSG_05008
		,.title = "出现卡纸。"
        ,.mediaInfo = 0
        ,.lines = 3
        ,.line0 = "拉出纸盒, 取出卡纸。"
        ,.line1 = "打开盖门, 提起熔断单元松开拉杆, "
        ,.line2 = "取出卡纸。关闭盖门。"
//	,.line2 = "有关详细信息, 请单击 [显示我如何] 按钮。"
	}
	,{
		.string_index = MSG_05009
		,.title = "出现卡纸。"
        ,.mediaInfo = 0
        ,.lines = 3
        ,.line0 = "拉出纸盒, 取出卡纸。"
        ,.line1 = "打开盖门, 提起熔断单元松开拉杆, "
        ,.line2 = "取出卡纸。关闭盖门。"
//	,.line2 = "有关详细信息, 请单击 [显示我如何] 按钮。"
	}
    ,{
        .string_index = MSG_05010
        ,.title = "出现卡纸。"
        ,.mediaInfo = 0
        ,.lines = 3
        ,.line0 = "拉出纸盒, 取出卡纸。"
        ,.line1 = "打开盖门, 提起熔断单元松开拉杆, "
        ,.line2 = "取出卡纸。关闭盖门。"
//	,.line2 = "有关详细信息, 请单击 [显示我如何] 按钮。"
    }
	,{
		.string_index = MSG_05011
        ,.title = "出现卡纸。"
        ,.mediaInfo = 0
        ,.lines = 3
        ,.line0 = "拉出纸盒, 取出卡纸。"
        ,.line1 = "打开盖门, 提起熔断单元松开拉杆, "
        ,.line2 = "取出卡纸。关闭盖门。"
//	,.line2 = "有关详细信息, 请单击 [显示我如何] 按钮。"
	}
	,{
		.string_index = MSG_05012
        ,.title = "出现卡纸。"
        ,.mediaInfo = 0
        ,.lines = 3
        ,.line0 = "拉出纸盒, 取出卡纸。"
        ,.line1 = "打开盖门, 提起熔断单元松开拉杆, "
        ,.line2 = "取出卡纸。关闭盖门。"
//	,.line2 = "有关详细信息, 请单击 [显示我如何] 按钮。"
	}
	,{
		.string_index = MSG_05013
        ,.title = "出现卡纸。"
        ,.mediaInfo = 0
        ,.lines = 3
        ,.line0 = "拉出纸盒, 取出卡纸。"
        ,.line1 = "打开盖门, 提起熔断单元松开拉杆, "
        ,.line2 = "取出卡纸。关闭盖门。"
//	,.line2 = "有关详细信息, 请单击 [显示我如何] 按钮。"
	}
	,{
		.string_index = MSG_05014
        ,.title = "出现卡纸。"
        ,.mediaInfo = 0
        ,.lines = 3
        ,.line0 = "拉出纸盒, 取出卡纸。"
        ,.line1 = "打开盖门, 提起熔断单元松开拉杆, "
        ,.line2 = "取出卡纸。关闭盖门。"
//	,.line2 = "有关详细信息, 请单击 [显示我如何] 按钮。"
	}
	,{
		.string_index = MSG_05015
        ,.title = "出现卡纸。"
        ,.mediaInfo = 0
        ,.lines = 3
        ,.line0 = "拉出纸盒, 取出卡纸。"
        ,.line1 = "打开盖门, 提起熔断单元松开拉杆, "
        ,.line2 = "取出卡纸。关闭盖门。"
//	,.line2 = "有关详细信息, 请单击 [显示我如何] 按钮。"
	}
	,{
		.string_index = MSG_05016
        ,.title = "出现卡纸。"
        ,.mediaInfo = 0
        ,.lines = 3
        ,.line0 = "拉出纸盒, 取出卡纸。"
        ,.line1 = "打开盖门, 提起熔断单元松开拉杆, "
        ,.line2 = "取出卡纸。关闭盖门。"
//	,.line2 = "有关详细信息, 请单击 [显示我如何] 按钮。"
	}
	,{
		.string_index = MSG_05017
        ,.title = "出现卡纸。"
        ,.mediaInfo = 0
        ,.lines = 3
        ,.line0 = "拉出纸盒, 取出卡纸。"
        ,.line1 = "打开盖门, 提起熔断单元松开拉杆, "
        ,.line2 = "取出卡纸。关闭盖门。"
//	,.line2 = "有关详细信息, 请单击 [显示我如何] 按钮。"
	}
	,{
		.string_index = MSG_05018
		,.title = "打印机错误。"
	,.mediaInfo = 0
	,.lines = 2
    ,.line0 = "关闭打印机，然后再次打开它。"
	,.line1 = "如果重复此故障, 请与客户支持联系。"
	}
    ,{
        .string_index = MSG_05019
        ,.title = "黑色硒鼓套件丢失或未完全插入打印机。"
    ,.mediaInfo = 0
    ,.lines = 1
    ,.line0 = "请插入黑色硒鼓套件。"
    }
    ,{
        .string_index = MSG_05020
        ,.title = "彩色硒鼓套件丢失或未完全插入打印机。"
    ,.mediaInfo = 0
    ,.lines = 1
    ,.line0 = "请插入彩色硒鼓套件。"
    }
    ,{
        .string_index = MSG_05021
        ,.title = "黑色显影单元丢失或未完全插入打印机。"
    ,.mediaInfo = 0
    ,.lines = 1
    ,.line0 = "请插入黑色显影单元。"
    }
    ,{
        .string_index = MSG_05022
        ,.title = "青色显影单元丢失或未完全插入打印机。"
    ,.mediaInfo = 0
    ,.lines = 1
    ,.line0 = "请插入青色显影单元。"
    }
    ,{
        .string_index = MSG_05023
        ,.title = "品红色显影单元丢失或未完全插入打印机。"
    ,.mediaInfo = 0
    ,.lines = 1
    ,.line0 = "请插入品红色显影单元。"
    }
    ,{
        .string_index = MSG_05024
        ,.title = "黄色显影单元丢失或未完全插入打印机。"
    ,.mediaInfo = 0
    ,.lines = 1
    ,.line0 = "请插入黄色显影单元。"
    }
    ,{
        .string_index = MSG_05025
        ,.title = "废粉仓丢失或未完全插入打印机。"
    ,.mediaInfo = 0
    ,.lines = 1
    ,.line0 = "请插入废粉仓。"
    }
    ,{
        .string_index = MSG_05026
        ,.title = "黑色硒鼓套件寿命尽。"
    ,.mediaInfo = 0
    ,.lines = 1
    ,.line0 = "请更换黑色硒鼓套件。"
    }
    ,{
        .string_index = MSG_05027
        ,.title = "彩色硒鼓套件寿命尽。"
    ,.mediaInfo = 0
    ,.lines = 1
    ,.line0 = "请更换彩色硒鼓套件。"
    }
    ,{
        .string_index = MSG_05028
        ,.title = "黑色显影单元寿命尽。"
    ,.mediaInfo = 0
    ,.lines = 1
    ,.line0 = "请更换黑色显影单元。"
    }
    ,{
        .string_index = MSG_05029
        ,.title = "青色显影单元寿命尽。"
    ,.mediaInfo = 0
    ,.lines = 1
    ,.line0 = "请更换青色显影单元。"
    }
    ,{
        .string_index = MSG_05030
        ,.title = "品红色显影单元寿命尽。"
    ,.mediaInfo = 0
    ,.lines = 1
    ,.line0 = "请更换品红色显影单元。"
    }
    ,{
        .string_index = MSG_05031
        ,.title = "黄色显影单元寿命尽。"
    ,.mediaInfo = 0
    ,.lines = 1
    ,.line0 = "请更换黄色显影单元。"
    }
    ,{
        .string_index = MSG_05032
        ,.title = "废粉仓满。"
    ,.mediaInfo = 0
    ,.lines = 1
    ,.line0 = "请更换废粉仓。"
    }
	,{
		.string_index = MSG_05033
        ,.title = "更换黑色粉仓。"
	,.mediaInfo = 0
    ,.lines = 1
    ,.line0 = "打开碳粉盒入口盖。然后卸下所用的黑色粉仓并安装一个新的黑色粉仓。"
//	,.line1 = "有关详细信息, 请单击 [显示我如何] 按钮。"
	}
	,{
		.string_index = MSG_05034
        ,.title = "更换青色粉仓。"
	,.mediaInfo = 0
    ,.lines = 1
    ,.line0 = "打开碳粉盒入口盖。然后卸下所用的青色粉仓并安装一个新的青色粉仓。"
//	,.line1 = "有关详细信息, 请单击 [显示我如何] 按钮。"
	}
	,{
		.string_index = MSG_05035
        ,.title = "更换品红色粉仓。"
	,.mediaInfo = 0
    ,.lines = 1
    ,.line0 = "打开碳粉盒入口盖。然后卸下所用的品红色粉仓并安装一个新的品红色粉仓。"
//	,.line1 = "有关详细信息, 请单击 [显示我如何] 按钮。"
	}
	,{
		.string_index = MSG_05036
        ,.title = "更换黄色粉仓。"
	,.mediaInfo = 0
    ,.lines = 1
    ,.line0 = "打开碳粉盒入口盖。然后卸下所用的黄色粉仓并安装一个新的黄色粉仓。"
//	,.line1 = "有关详细信息, 请单击 [显示我如何] 按钮。"
	}
	,{
		.string_index = MSG_05037
		,.title = "机盖被打开。"
	,.mediaInfo = 0
	,.lines = 1
	,.line0 = "请关闭所有盖门。"
	}
	,{
        .string_index = MSG_05043
        ,.title = "需要立即更换黑色粉仓。"
        ,.mediaInfo = 0
        ,.lines = 1
        ,.line0 = "打开碳粉盒入口盖。然后卸下所用的黑色粉仓并安装一个新的黑色粉仓。"
	}
	,{
		.string_index = MSG_05044
        ,.title = "需要立即更换青色粉仓。"
        ,.mediaInfo = 0
        ,.lines = 1
        ,.line0 = "打开碳粉盒入口盖。然后卸下所用的青色粉仓并安装一个新的青色粉仓。"
	}
	,{
		.string_index = MSG_05045
        ,.title = "需要立即更换品红色粉仓。"
        ,.mediaInfo = 0
        ,.lines = 1
        ,.line0 = "打开碳粉盒入口盖。然后卸下所用的品红色粉仓并安装一个新的品红色粉仓。"
	}
	,{
        .string_index = MSG_05046
        ,.title = "需要立即更换黄色粉仓。"
        ,.mediaInfo = 0
        ,.lines = 1
        ,.line0 = "打开碳粉盒入口盖。然后卸下所用的黄色粉仓并安装一个新的黄色粉仓。"
	}
	,{
		.string_index = MSG_05072
		,.title = "未插入纸盘"
	,.mediaInfo = 0
    ,.lines = 3
	,.line0 = "拉出纸盘。"
    ,.line1 = "检查设备中发生送纸错误的纸张。"
    ,.line2 = "取出纸张然后插入纸盘。"
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
