#ifndef MAIN_FRAME_H
#define MAIN_FRAME_H

#include <wx/wx.h>

class MainFrame : public wxFrame
{

public:
    MainFrame();
    ~MainFrame();

    enum
    {
        CONNECT_BTN = wxID_HIGHEST + 1, // declares an id which will be used to call our button
    };

public:
    wxTextCtrl *mUserNameEditBox;
    wxButton *mConnectBtn;

    void OnButtonClicked(wxCommandEvent &evt);
    void OnTxtEdit(wxCommandEvent &evt);
    wxDECLARE_EVENT_TABLE();
};

#endif