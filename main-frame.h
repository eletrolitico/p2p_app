#ifndef MAIN_FRAME_H
#define MAIN_FRAME_H

#include <wx/wx.h>
#include <string>

#include "tox-handler.h"
#include "message-dlg.h"

class ToxHandler;

class MainFrame : public wxFrame
{
private:
    void CreateUserInterface();

    Tox *mTox {nullptr};
    bool mIsRunning {false};
    bool mBtnConnectedClicked {false};

    ToxHandler *mTHandler {nullptr};

public:
    MainFrame();
    ~MainFrame();

    enum
    {
        CONNECT_BTN = wxID_HIGHEST + 1, // declares an id which will be used to call our button
    };

public:
    //labels
    wxStaticText *mMyIDLabel;
    wxStaticText *mFriendIDLabel;

    //ctrls
    wxTextCtrl *mMyIDCtrl;
    wxTextCtrl *mMyNameCtrl;
    wxTextCtrl *mFriendIDCtrl;

    //buttons
    wxButton *mConnectBtn;

    void OnClose(wxCloseEvent &evt);

    void OnButtonClicked(wxCommandEvent &evt);
    void OnTxtEdit(wxCommandEvent &evt);
    wxDECLARE_EVENT_TABLE();

};

#endif