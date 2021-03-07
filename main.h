#ifndef MAIN_H
#define MAIN_H

#include <wx/wx.h>

class Main : public wxFrame
{

private:
    wxTextCtrl *MainEditBox;

public:
    Main();
    ~Main();

    enum
    {
        TEXT_Main = wxID_HIGHEST + 1, // declares an id which will be used to call our button
    };
};

#endif