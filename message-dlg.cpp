#include "message-dlg.h"

MessageDialog::MessageDialog(const wxString &title)
    : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(800, 600))
{
    //ListBox
    mMessageListBox = new wxListBox(this, wxID_ANY, wxPoint(10, 10), wxSize(300, 300));

    //Ctrl
    mMessageTextCtrl = new wxTextCtrl(this, wxID_ANY, wxT("Type your message here"), wxPoint(95, 105));
    mMessageTextCtrl->Bind(wxEVT_TEXT, &MessageDialog::OnTxtEdit, this);

    //Btn
    mSendBtn = new wxButton(this, wxID_ANY, "Send", wxPoint(10, 30), wxSize(60, 20));
    mSendBtn->Bind(wxEVT_BUTTON, &MessageDialog::OnButtonClicked, this);

    CreateInterface();

    Centre();
    ShowModal();

    Destroy();
}

void MessageDialog::CreateInterface()
{

    //grid - rows, col, padding
    wxGridSizer *grid = new wxGridSizer(3, 1, 10, 10);

    wxFont font(30, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);
    mSendBtn->SetFont(font);

    //Add
    grid->Add(mMessageListBox, 1, wxEXPAND | wxALL);
    grid->Add(mMessageTextCtrl, 1, wxEXPAND | wxALL);
    grid->Add(mSendBtn, 1, wxEXPAND | wxALL);

    this->SetSizer(grid);
    grid->Layout();
}

void MessageDialog::OnTxtEdit(wxCommandEvent &evt)
{
    printf("edit: %s\n", mMessageTextCtrl->GetValue().c_str().AsChar());
    evt.Skip();
}

void MessageDialog::OnButtonClicked(wxCommandEvent &evt)
{
    printf("clicked send button\n");
    evt.Skip();
}