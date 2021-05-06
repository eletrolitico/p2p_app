#include "main-frame.h"

#define MAIN_STYLE wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_CLOSE(MainFrame::OnClose)
    EVT_BUTTON(SEND_BTN, MainFrame::OnSendMessage)
    EVT_BUTTON(NAME_BTN, MainFrame::OnChangeName)
    EVT_COMMAND(wxID_ANY, wxEVT_TOX_INIT, MainFrame::OnToxID)
wxEND_EVENT_TABLE()

MainFrame::MainFrame() : wxFrame(nullptr, wxID_ANY, "Chat P2P", wxPoint(30, 30), wxSize(800, 600), MAIN_STYLE)
{

    //ctrl
    m_toxIDCtrl = new wxTextCtrl(this, wxID_ANY, "ID: ", wxDefaultPosition, wxSize(800, 30), wxTE_READONLY);
    m_nameCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(400, 30));
    m_messageCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(300, 30));

    //btn
    m_nameBtn = new wxButton(this, NAME_BTN, "Change", wxDefaultPosition, wxSize(60, 30));
    m_sendBtn = new wxButton(this, SEND_BTN, "Send", wxDefaultPosition, wxSize(60, 30));

    // list boxes
    m_friendsBox = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxSize(460, 500));
    m_messageBox = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxSize(400, 500));

    auto nameSz = new wxBoxSizer(wxHORIZONTAL);
    auto messageSz = new wxBoxSizer(wxHORIZONTAL);

    auto left = new wxBoxSizer(wxVERTICAL);
    auto right = new wxBoxSizer(wxVERTICAL);

    auto main = new wxBoxSizer(wxHORIZONTAL);
    auto top = new wxBoxSizer(wxVERTICAL);

    nameSz->Add(m_nameCtrl);
    nameSz->Add(m_nameBtn);

    messageSz->Add(m_messageCtrl);
    messageSz->Add(m_sendBtn);

    left->Add(nameSz);
    left->Add(m_friendsBox);

    right->Add(m_messageBox);
    right->Add(messageSz);

    main->Add(left);
    main->Add(right);

    top->Add(m_toxIDCtrl, 0, wxALIGN_CENTER);
    top->Add(main);

    SetSizerAndFit(top);

    mTHandler = new ToxHandler(this);
    mTHandler->Run();
}

MainFrame::~MainFrame() {}

void MainFrame::OnSendMessage(wxCommandEvent &evt)
{
    evt.Skip();
}

void MainFrame::OnChangeName(wxCommandEvent &evt)
{
    mTHandler->SetName(m_nameCtrl->GetValue().ToStdString());
    evt.Skip();
}

void MainFrame::OnClose(wxCloseEvent &evt)
{
    if (m_isDeleting)
        return;
    mTHandler->Delete();
    m_isDeleting = true;
    Destroy();
}

void MainFrame::OnToxID(wxCommandEvent &evt)
{
    *m_toxIDCtrl << mTHandler->m_toxID;
    *m_nameCtrl << mTHandler->m_name;
    //std::cout << "entrei no ontoxid" << std::endl;
    evt.Skip();
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