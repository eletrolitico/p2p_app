#include "main-frame.h"

#define MAIN_STYLE wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_BUTTON(CONNECT_BTN, MainFrame::OnButtonClicked)
        wxEND_EVENT_TABLE()


 MainFrame::MainFrame() : wxFrame(nullptr, wxID_ANY, "Chat P2P", wxPoint(30, 30), wxSize(800, 600), MAIN_STYLE)
{

    //rows, col, padding
    wxGridSizer *grid = new wxGridSizer(3, 1, 50, 50);

    wxFont font(30, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);

    mLabelID = new wxStaticText(this, wxID_ANY, "ID: 321", wxPoint(30, 30), wxSize(300, 20), wxALIGN_CENTRE_HORIZONTAL | wxALIGN_CENTRE_VERTICAL, "label");
    mLabelID->SetFont(font);

    grid->Add(mLabelID, 1, wxEXPAND | wxALL);

    mUserNameEditBox = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxPoint(10, 10), wxSize(300, 20));
    grid->Add(mUserNameEditBox, 1, wxEXPAND | wxALL);
    
    mUserNameEditBox->Bind(wxEVT_TEXT, &MainFrame::OnTxtEdit, this);

    mConnectBtn = new wxButton(this, CONNECT_BTN, "Conectar", wxPoint(10, 30), wxSize(60, 20));
    grid->Add(mConnectBtn, 1, wxEXPAND | wxALL);
    
    this->SetSizer(grid);
    grid->Layout();
    
}

MainFrame::~MainFrame()
{
}

void MainFrame::OnButtonClicked(wxCommandEvent &evt)
{
    printf("clicked\n");
    evt.Skip();
}

void MainFrame::OnTxtEdit(wxCommandEvent &evt)
{
    printf("edit: %s\n", mUserNameEditBox->GetValue().c_str().AsChar());
    evt.Skip();
}
