/*********************************************************************************
 * Copyright(C), sqsanshao
 * FileName: network.cpp
 * Author:
 * Version:
 * Date:
 * Description: 
 *  存放与网络有关的命令，如账号，语音识别等
 *  
 * Others: 
 * Function List: 
 *  
 *  
 * History: 
 *	 1.Date: 
 *	  Author: 
 *	  Modification: 
**********************************************************************************/
#include "command.h"
#include "../include/aegisub/context.h"
#include "../libresrc/libresrc.h"
#include <libaegisub/make_unique.h>
//#include "account/dialog_login.h"
#include "../recognize/dialog_recognize.h"
#include <wx/msgdlg.h>

namespace {
    using cmd::Command;

//struct account_login final : public Command {
//    CMD_NAME("account/login")
//    CMD_ICON(about_menu)
//    STR_MENU("&Login")
//    STR_DISP("Login")
//    STR_HELP("Login")
//
//    void operator()(AGI::Context *c) override {
//        LoginDialog Dlg(c->parent);
//        Dlg.ShowModal();
//    }
//};
//
//struct account_detail final : public Command {
//    CMD_NAME("account/detail")
//    CMD_ICON(about_menu)
//    STR_MENU("&Account details")
//    STR_DISP("Display account details")
//    STR_HELP("Display account details")
//
//    void operator()(AGI::Context *c) override {
//        ShowAccountDetailDialog(c->parent);
//    }
//};
//
//struct account_logout final : public Command {
//    CMD_NAME("account/logout")
//    CMD_ICON(about_menu)
//    STR_MENU("&Logout")
//    STR_DISP("Logout")
//    STR_HELP("Logout")
//
//    void operator()(AGI::Context *c) override {
//        ShowLogoutDialog(c->parent);
//    }
//};

struct recognize final : public Command {
    CMD_NAME("AI/speech recognize")
    CMD_ICON(about_menu)
    STR_MENU("&Recognize")
    STR_DISP("Recognize")
    STR_HELP("Recognize")

    void operator()(agi::Context *c) override {
        RecognizeDialog dlg(c);
        dlg.ShowModal();
    }
};

} // end namespace 

namespace cmd {
    void init_network() {
        //reg(AGI::make_unique<account_login>());
        //reg(AGI::make_unique<account_detail>());
        //reg(AGI::make_unique<account_logout>());
        reg(agi::make_unique<recognize>());
    }
}
