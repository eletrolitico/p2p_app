#ifndef MAIN_FRAME_H
#define MAIN_FRAME_H

#include <wx/wx.h>
#include <wx/clipbrd.h>
#include <string>

#include "tox-handler.h"
class ToxHandler;

#include "message-dlg.h"

class MainFrame : public wxFrame
{
private:
    void CreateUserInterface();

    bool mBtnConnectedClicked{false};
    bool m_isDeleting{false};

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

    // Event handlers
    void OnClose(wxCloseEvent &evt);
    void OnButtonClicked(wxCommandEvent &evt);
    void OnTxtEdit(wxCommandEvent &evt);
    void OnToxID(wxCommandEvent &evt);

    void AddToClipBoard(char *txt);

    ToxHandler *mTHandler{nullptr};
    wxCriticalSection mTHandlerCS;

    wxDECLARE_EVENT_TABLE();
};

#endif