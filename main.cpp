#include "main.h"

Main::Main() : wxFrame(nullptr, wxID_ANY, "HDChat")
{
    MainEditBox = new wxTextCtrl(this, TEXT_Main, "Hi!", wxDefaultPosition, wxDefaultSize,  
                        wxTE_MULTILINE | wxTE_RICH , wxDefaultValidator, wxTextCtrlNameStr);

                        
}

Main::~Main() {}