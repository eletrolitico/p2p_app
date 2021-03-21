#include "message-dlg.h"

MessageDialog::MessageDialog(const wxString &title)
    : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(800, 600))
{

    wxPanel *panel = new wxPanel(this, -1);

    wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);

    tc = new wxTextCtrl(panel, -1, wxT(""),
                        wxPoint(95, 105));
    tc->Bind(wxEVT_TEXT, &MessageDialog::OnTxtEdit, this);

    wxButton *okButton = new wxButton(this, -1, wxT("Ok"),
                                      wxDefaultPosition, wxSize(70, 30));

    wxButton *closeButton = new wxButton(this, -1, wxT("Close"),
                                         wxDefaultPosition, wxSize(70, 30));

    lb = new wxListBox(this, -1, wxDefaultPosition, wxSize(700, 200));

    hbox->Add(lb);

    hbox->Add(okButton, 1);
    hbox->Add(closeButton, 1, wxLEFT, 5);

    vbox->Add(panel, 1);
    vbox->Add(hbox, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);

    SetSizer(vbox);

    Centre();
    ShowModal();

    Destroy();
}

void MessageDialog::OnTxtEdit(wxCommandEvent &evt)
{
    printf("edit: %s\n", tc->GetValue().c_str().AsChar());
    evt.Skip();
}