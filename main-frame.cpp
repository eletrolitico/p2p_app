#include "main-frame.h"

#define MAIN_STYLE wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_BUTTON(CONNECT_BTN, MainFrame::OnButtonClicked)
wxEND_EVENT_TABLE()


MainFrame::MainFrame() : wxFrame(nullptr, wxID_ANY, "Chat P2P", wxPoint(30, 30), wxSize(800, 600), MAIN_STYLE)
{

    //labels
    mMyIDLabel = new wxStaticText(this, wxID_ANY, "My ID:", wxPoint(30, 30), wxSize(300, 20), wxALIGN_LEFT, "label");
    mFriendIDLabel = new wxStaticText(this, wxID_ANY, "Friend ID:", wxPoint(30, 30), wxSize(300, 20), wxALIGN_LEFT, "label");

    //ctrl
    mMyIDCtrl = new wxTextCtrl(this, wxID_ANY, "129837shadsd19283718923hgqd19237", wxPoint(10, 10), wxSize(300, 20), wxTE_READONLY);
    mFriendIDCtrl = new wxTextCtrl(this, wxID_ANY, "Friend ID here", wxPoint(10, 10), wxSize(300, 20));

    mMyIDCtrl->Bind(wxEVT_TEXT, &MainFrame::OnTxtEdit, this);
   
    //btn
    mConnectBtn = new wxButton(this, CONNECT_BTN, "Connect", wxPoint(10, 30), wxSize(60, 20));
    
    CreateUserInterface();
    
}

MainFrame::~MainFrame()
{
}

void MainFrame::CreateUserInterface() {
    
    //rows, col, padding
    wxGridSizer *grid = new wxGridSizer(5, 1, 0, 0);
    
    wxFont font(50, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);
    
    mMyIDLabel->SetFont(font);
    mFriendIDLabel->SetFont(font);

    grid->Add(mMyIDLabel, 1, wxEXPAND | wxALL);     //label
    grid->Add(mMyIDCtrl, 1, wxEXPAND | wxALL);      //ctrl
    
    grid->Add(mFriendIDLabel, 1, wxEXPAND | wxALL); //label
    grid->Add(mFriendIDCtrl, 1, wxEXPAND | wxALL);  //ctrl
    
    grid->Add(mConnectBtn, 1, wxEXPAND | wxALL);    //btn

    this->SetSizer(grid);
    grid->Layout();
}


void MainFrame::OnButtonClicked(wxCommandEvent &evt)
{
    printf("clicked\n");
    evt.Skip();
}

void MainFrame::OnTxtEdit(wxCommandEvent &evt)
{
    printf("edit: %s\n", mMyIDCtrl->GetValue().c_str().AsChar());
    evt.Skip();
}
