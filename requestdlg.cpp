#include "requestdlg.h"

#include <sstream>
#include <iomanip>

wxDEFINE_EVENT(FRIEND_ADD_CB, RequestDlgEvt);
wxDEFINE_EVENT(FRIEND_ACCEPT_CB, RequestDlgEvt);

RequestDlg::RequestDlg(
    Request req,
    wxWindow *parent, wxWindowID id,
    const wxString &title,
    const wxPoint &pos,
    const wxSize &size,
    long style,
    const wxString &name) : wxDialog(parent, id, title, pos, size, style, name), m_request(req), isAdd(false)
{
    InitInterface();
}

RequestDlg::RequestDlg(
    wxWindow *parent, wxWindowID id,
    const wxString &title,
    const wxPoint &pos,
    const wxSize &size,
    long style,
    const wxString &name) : wxDialog(parent, id, title, pos, size, style, name)
{
    InitInterface();
}

RequestDlg::~RequestDlg() {}

std::string bin2hex(const uint8_t *bin, const size_t len);

void RequestDlg::InitInterface()
{
    wxBoxSizer *topsz = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *btnBar = new wxBoxSizer(wxHORIZONTAL);

    m_label_id = new wxStaticText(this, wxID_ANY, "Tox ID:");
    m_tox_id = new wxTextCtrl(this, wxID_ANY, "");
    m_label_msg = new wxStaticText(this, wxID_ANY, "Mensagem: ");
    m_msg = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(650, 250), wxTE_MULTILINE);

    m_ok_btn = new wxButton(this, wxID_ANY, "Ok");
    m_nein_btn = new wxButton(this, wxID_ANY, "Nope");

    m_nein_btn->Bind(wxEVT_BUTTON, &RequestDlg::OnNopeBtn, this);
    m_ok_btn->Bind(wxEVT_BUTTON, &RequestDlg::OnOkBtn, this);

    btnBar->Add(m_ok_btn, 1, wxEXPAND);
    btnBar->Add(m_nein_btn, 1, wxEXPAND);

    topsz->Add(m_label_id, 0, wxEXPAND);
    topsz->Add(m_tox_id, 0, wxEXPAND);
    topsz->Add(m_label_msg, 0, wxEXPAND);
    topsz->Add(m_msg, 0, wxEXPAND);
    topsz->Add(btnBar, 0, wxEXPAND);

    if (!isAdd)
    {
        m_tox_id->SetEditable(false);
        m_tox_id->SetValue(bin2hex(m_request.userdata.pubkey, TOX_PUBLIC_KEY_SIZE));

        m_msg->SetEditable(false);
        m_msg->SetValue(m_request.msg);
    }

    SetSizerAndFit(topsz);
}

void RequestDlg::OnNopeBtn(wxCommandEvent &evt)
{
    Destroy();
}

void RequestDlg::OnOkBtn(wxCommandEvent &evt)
{
    if (isAdd)
    {
        RequestDlgEvt event(FRIEND_ADD_CB, GetId());
        event.setToxID(m_tox_id->GetValue());
        event.setMessage(m_msg->GetValue());
        event.SetEventObject(this);
        ProcessWindowEvent(event);
    }
    else
    {
        RequestDlgEvt event(FRIEND_ACCEPT_CB, GetId());
        event.setRequest(m_request);
        event.SetEventObject(this);
        ProcessWindowEvent(event);
    }
    Destroy();
}