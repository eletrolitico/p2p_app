#include "requestdlg.h"

#include "main-frame.h"

template <typename Class>
RequestDlg::RequestDlg(
    Request req,
    void (Class::*method)(),
    wxWindow *parent, wxWindowID id,
    const wxString &title,
    const wxPoint &pos,
    const wxSize &size,
    long style,
    const wxString &name) : wxDialog(parent, id, title, pos, size, style, name), m_request(req), isAdd(false)
{
    this->method = (void (*)())((void *)method);
    InitInterface();
}

template < >
RequestDlg::RequestDlg<MainFrame>(
    Request req,
    void (MainFrame::*method)(),
    wxWindow *parent, wxWindowID id,
    const wxString &title,
    const wxPoint &pos,
    const wxSize &size,
    long style,
    const wxString &name) : wxDialog(parent, id, title, pos, size, style, name), m_request(req), isAdd(false)
{
    this->method = (void (*)())((void *)method);
    InitInterface();
}

template <typename Class>
RequestDlg::RequestDlg(
    void (Class::*method)(),
    wxWindow *parent, wxWindowID id,
    const wxString &title,
    const wxPoint &pos,
    const wxSize &size,
    long style,
    const wxString &name) : wxDialog(parent, id, title, pos, size, style, name)
{
    this->method = (void (*)())((void *)method);
    InitInterface();
}


template <>
RequestDlg::RequestDlg<MainFrame>(
    void (MainFrame::*method)(),
    wxWindow *parent, wxWindowID id,
    const wxString &title,
    const wxPoint &pos,
    const wxSize &size,
    long style,
    const wxString &name) : wxDialog(parent, id, title, pos, size, style, name)
{
    this->method = (void (*)())((void *)method);
    InitInterface();
}

RequestDlg::~RequestDlg() {}

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
        m_tox_id->SetValue(m_request.userdata.pubkey);

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
    method();
    Destroy();
}