#include "main-frame.h"

#define MAIN_STYLE wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_CLOSE(MainFrame::OnClose)
        EVT_BUTTON(CONNECT_BTN, MainFrame::OnButtonClicked)
            wxEND_EVENT_TABLE()

                MainFrame::MainFrame() : wxFrame(nullptr, wxID_ANY, "Chat P2P", wxPoint(30, 30), wxSize(800, 600), MAIN_STYLE)
{

    TOX_ERR_NEW err_new;
    Tox *mTox = tox_new(NULL, &err_new);
    if (err_new != TOX_ERR_NEW_OK)
    {
        fprintf(stderr, "tox_new failed with error code %d\n", err_new);
        exit(1);
    }

    mTHandler = new ToxHandler(mTox, &mIsRunning, this);

    //labels
    mMyIDLabel = new wxStaticText(this, wxID_ANY, "My ID/Name:", wxPoint(30, 30), wxSize(300, 20), wxALIGN_LEFT, "label");
    mFriendIDLabel = new wxStaticText(this, wxID_ANY, "Friend ID:", wxPoint(30, 30), wxSize(300, 20), wxALIGN_LEFT, "label");

    //ctrl
    mMyIDCtrl = new wxTextCtrl(this, wxID_ANY, "ID: ", wxPoint(10, 10), wxSize(300, 20), wxTE_READONLY);
    mFriendIDCtrl = new wxTextCtrl(this, wxID_ANY, "Friend ID here", wxPoint(10, 10), wxSize(300, 20));
    mMyNameCtrl = new wxTextCtrl(this, wxID_ANY, "Bot-P2P-2021", wxPoint(10, 10), wxSize(300, 20));
    mMyNameCtrl->SetFocus();

    mMyIDCtrl->Bind(wxEVT_TEXT, &MainFrame::OnTxtEdit, this);

    //btn
    mConnectBtn = new wxButton(this, CONNECT_BTN, "Connect", wxPoint(10, 30), wxSize(60, 20));

    CreateUserInterface();
}

MainFrame::~MainFrame() {}

void MainFrame::CreateUserInterface()
{

    //grid - rows, col, padding
    wxGridSizer *grid = new wxGridSizer(7, 1, 0, 0);

    wxFont font(40, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);

    mMyIDLabel->SetFont(font);
    mFriendIDLabel->SetFont(font);

    //my info
    grid->Add(mMyIDLabel, 1, wxEXPAND | wxALL);
    grid->Add(mMyIDCtrl, 1, wxEXPAND | wxALL);
    grid->Add(mMyNameCtrl, 1, wxEXPAND | wxALL);

    //jump a row
    grid->Add(new wxStaticText(this, wxID_ANY, ".", wxPoint(30, 30), wxSize(300, 20), wxALIGN_LEFT, "label"), wxEXPAND | wxALL);

    //friend info
    grid->Add(mFriendIDLabel, 1, wxEXPAND | wxALL);
    grid->Add(mFriendIDCtrl, 1, wxEXPAND | wxALL);

    //btn
    grid->Add(mConnectBtn, 1, wxEXPAND | wxALL);

    this->SetSizer(grid);
    grid->Layout();
}

void MainFrame::OnButtonClicked(wxCommandEvent &evt)
{
    if (mIsRunning)
    {
        evt.Skip();
        return;
    }

    if (std::string(mMyNameCtrl->GetValue()).empty() || std::string(mFriendIDCtrl->GetValue()).empty())
    {
        evt.Skip();
        return;
    }

    mConnectBtn->Enable(false);

    mIsRunning = true;
    mTHandler->Run();

    MessageDialog *message = new MessageDialog(wxT("Messages"));
    message->Show(true);

    printf("clicked\n");
    evt.Skip();
}

void MainFrame::OnTxtEdit(wxCommandEvent &evt)
{
    printf("edit: %s\n", mMyIDCtrl->GetValue().c_str().AsChar());
    evt.Skip();
}

void MainFrame::OnClose(wxCloseEvent &evt)
{
    mIsRunning = false;

    if (mTHandler)
    {
        mTHandler->Delete();
        delete mTHandler;
    }
}

void MainFrame::AddToClipBoard(char *txt)
{
    bool isOk = false;
    wxClipboard *clip = new wxClipboard();
    if (clip->Open())
    {
        clip->Clear();
        clip->SetData(new wxTextDataObject(wxString::FromUTF8(txt)));
        clip->Flush();
        clip->Close();

        isOk = true;
    }

    delete clip;

    if (!isOk)
        ::wxMessageBox("The clipboard copy failed", "Error", wxICON_ERROR | wxCENTRE);
}