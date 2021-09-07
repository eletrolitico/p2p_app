#include <wx/wx.h>
#include "tox-handler.h"

class RequestDlg : public wxDialog
{

public:

    template <typename Class>
    RequestDlg(void (Class::*method)(), wxWindow *parent, wxWindowID id,
               const wxString &title,
               const wxPoint &pos = wxDefaultPosition,
               const wxSize &size = wxDefaultSize,
               long style = wxDEFAULT_DIALOG_STYLE,
               const wxString &name = wxString(wxDialogNameStr));

    template <typename Class>
    RequestDlg(Request req, void (Class::*method)(), wxWindow *parent, wxWindowID id,
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


    void (*method)();

public:
    void OnNopeBtn(wxCommandEvent &evt);
    void OnOkBtn(wxCommandEvent &evt);
};