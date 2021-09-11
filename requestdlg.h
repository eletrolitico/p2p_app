#ifndef REQUEST_DLG_H
#define REQUEST_DLG_H

#include <wx/wx.h>
#include "tox-handler.h"

class RequestDlg : public wxDialog
{

public:
    RequestDlg(wxWindow *parent, wxWindowID id,
               const wxString &title,
               const wxPoint &pos = wxDefaultPosition,
               const wxSize &size = wxDefaultSize,
               long style = wxDEFAULT_DIALOG_STYLE,
               const wxString &name = wxString(wxDialogNameStr));

    RequestDlg(Request req, wxWindow *parent, wxWindowID id,
               const wxString &title,
               const wxPoint &pos = wxDefaultPosition,
               const wxSize &size = wxDefaultSize,
               long style = wxDEFAULT_DIALOG_STYLE,
               const wxString &name = wxString(wxDialogNameStr));

    ~RequestDlg();

private:
    Request m_request;
    bool isAdd{true};

    wxStaticText *m_label_id;
    wxTextCtrl *m_tox_id;
    wxStaticText *m_label_msg;
    wxTextCtrl *m_msg;
    wxButton *m_ok_btn;
    wxButton *m_nein_btn;

    void InitInterface();

public:
    void OnNopeBtn(wxCommandEvent &evt);
    void OnOkBtn(wxCommandEvent &evt);
};

class RequestDlgEvt : public wxEvent
{
public:
    RequestDlgEvt(wxEventType eventType, int winid = 0)
        : wxEvent(winid, eventType)
    {
    }

    inline Request getRequest() { return m_request; }
    inline wxString getToxID() { return m_tID; }
    inline wxString getMessage() { return m_msg; }

    inline void setRequest(Request r) { this->m_request = r; }
    inline void setToxID(wxString id) { this->m_tID = id; }
    inline void setMessage(wxString msg) { this->m_msg = msg; }

    virtual wxEvent *Clone() const { return new RequestDlgEvt(*this); }

private:
    Request m_request;
    wxString m_tID;
    wxString m_msg;
};

wxDECLARE_EVENT(FRIEND_ADD_CB, RequestDlgEvt);
wxDECLARE_EVENT(FRIEND_ACCEPT_CB, RequestDlgEvt);

#endif