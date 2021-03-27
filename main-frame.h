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
    bool mBtnConnectedClicked{false};
    bool m_isDeleting{false};

public:
    MainFrame();
    ~MainFrame();

    enum
    {
        MAIN_FRAME = wxID_HIGHEST + 1,
        NAME_BTN,
        SEND_BTN
    };

public:
    //ctrls
    wxTextCtrl *m_toxIDCtrl;
    wxTextCtrl *m_nameCtrl;
    wxTextCtrl *m_messageCtrl;
    wxListBox *m_friendsBox;
    wxListBox *m_messageBox;

    //buttons
    wxButton *m_nameBtn;
    wxButton *m_sendBtn;

    // Event handlers
    void OnClose(wxCloseEvent &evt);
    void OnSendMessage(wxCommandEvent &evt);
    void OnChangeName(wxCommandEvent &evt);
    void OnToxID(wxCommandEvent &evt);

    void AddToClipBoard(char *txt);

    ToxHandler *mTHandler{nullptr};
    wxCriticalSection mTHandlerCS;

    wxDECLARE_EVENT_TABLE();
};

#endif