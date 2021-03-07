#include "main-frame.h"

Main::Main() : wxFrame(nullptr, wxID_ANY, "Chat P2P")
{
    userNameEditBox = new wxTextCtrl(this, TEXT_Main, "Username", wxDefaultPosition, wxDefaultSize,  
                        wxTE_MULTILINE | wxTE_RICH , wxDefaultValidator, wxTextCtrlNameStr);

}

Main::~Main() {}