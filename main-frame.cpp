#include "main-frame.h"
#include "main-frame-events.h"

MainFrame::MainFrame() : wxFrame(nullptr, wxID_ANY, "Chat P2P", wxPoint(30, 30), wxSize(800, 600), wxDEFAULT_FRAME_STYLE)
{
    Bind(FRIEND_ADD_CB, &MainFrame::AddFrndCb, this);
    Bind(FRIEND_ACCEPT_CB, &MainFrame::AcceptFrndCb, this);

    //ctrl
    m_toxIDCtrl = new wxTextCtrl(this, wxID_ANY, "ID: ", wxDefaultPosition, wxSize(800, 30), wxTE_READONLY);
    m_nameCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(400, 30));
    m_messageCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(300, 30));

    //btn
    m_nameBtn = new wxButton(this, NAME_BTN, "Change", wxDefaultPosition, wxSize(60, 30));
    m_sendBtn = new wxButton(this, SEND_BTN, "Send", wxDefaultPosition, wxSize(60, 30));
    m_addBtn = new wxButton(this, ADD_BTN, "Add friend", wxDefaultPosition, wxSize(60, 30));

    // list boxes
    m_friendsBox = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxSize(460, 500));
    m_messageBox = new MessageBox(this, wxID_ANY, wxDefaultPosition, wxSize(400, 500));

    auto nameSz = new wxBoxSizer(wxHORIZONTAL);
    auto messageSz = new wxBoxSizer(wxHORIZONTAL);

    auto left = new wxBoxSizer(wxVERTICAL);
    auto right = new wxBoxSizer(wxVERTICAL);

    auto main = new wxBoxSizer(wxHORIZONTAL);
    auto top = new wxBoxSizer(wxVERTICAL);

    nameSz->Add(m_nameCtrl, 1);
    nameSz->Add(m_nameBtn);

    messageSz->Add(m_messageCtrl, 1);
    messageSz->Add(m_sendBtn);

    left->Add(nameSz, 0, wxEXPAND);
    left->Add(m_friendsBox, 1, wxEXPAND);
    left->Add(m_addBtn, 0, wxEXPAND);

    right->Add(m_messageBox, 1, wxEXPAND);
    right->Add(messageSz, 0, wxEXPAND);

    main->Add(left, 0, wxEXPAND);
    main->Add(right, 1, wxEXPAND);

    top->Add(m_toxIDCtrl, 0, wxEXPAND);
    top->Add(main, 1, wxEXPAND);

    SetSizerAndFit(top);

    mTHandler = new ToxHandler(this);
    mTHandler->Run();

    /*********************
     ***** test code *****
     *********************/
}

MainFrame::~MainFrame() {}

void MainFrame::OnSendMessage(wxCommandEvent &evt)
{
    if (m_messageBox->GetCurrFriend() == -1)
        return;
    m_messageBox->AddMessage("voce: " + m_messageCtrl->GetValue(), true);
    mTHandler->SendMessage(m_messageBox->GetCurrFriend(), m_messageCtrl->GetValue());
    m_messageCtrl->SetValue("");
}

void MainFrame::OnChangeName(wxCommandEvent &evt)
{
    mTHandler->SetName(m_nameCtrl->GetValue().ToStdString());
}

void MainFrame::OnSwitchFriend(wxCommandEvent &evt)
{
    std::cout << "talking to fren: " << evt.GetSelection() << std::endl;
    m_messageBox->SwitchFriend(evt.GetSelection());
}

void MainFrame::OnClose(wxCloseEvent &evt)
{
    if (m_isDeleting)
        return;

    mTHandler->Delete();
    m_isDeleting = true;
    // Esse sleep resolve o problema de terminar a thread
    sleep(1);
    Destroy();
}

void MainFrame::OnToxID(wxCommandEvent &evt)
{
    *m_toxIDCtrl << mTHandler->m_toxID;
    *m_nameCtrl << mTHandler->m_name;
    auto frnds = mTHandler->GetFriends();

    for (auto f : frnds)
    {
        m_friendsBox->Insert(wxString(f->name) + " - " + std::to_string(f->friend_num) + " - " + wxString(f->status_message), f->friend_num);
    }
    //std::cout << "entrei no ontoxid" << std::endl;
}

void MainFrame::OnFriendRequest(wxCommandEvent &evt)
{
    auto reqs = mTHandler->GetRequests();

    while (reqs.size())
    {
        Request r = reqs[reqs.size() - 1];
        reqs.pop_back();
        auto dlg = new RequestDlg(r, this, wxID_ANY, "aceitar amigo");
        dlg->Show();
        dlg->Bind(FRIEND_ADD_CB, &MainFrame::AddFrndCb, this, wxID_ANY);
        dlg->Bind(FRIEND_ACCEPT_CB, &MainFrame::AcceptFrndCb, this, wxID_ANY);
    }
}

void MainFrame::OnFriendAdd(wxCommandEvent &evt)
{
    auto dlg = new RequestDlg(this, wxID_ANY, "Add a friend");
    dlg->Show();
    dlg->Bind(FRIEND_ADD_CB, &MainFrame::AddFrndCb, this);
    dlg->Bind(FRIEND_ACCEPT_CB, &MainFrame::AcceptFrndCb, this);
}

void MainFrame::AddFrndCb(RequestDlgEvt &evt)
{
    uint32_t fNum = mTHandler->AddFriend(evt.getToxID(), evt.getMessage());
    if (fNum != -1)
    {
        m_friendsBox->Insert("pending - " + std::to_string(fNum) + " - pending", fNum);
    }
}

void MainFrame::AcceptFrndCb(RequestDlgEvt &evt)
{
    uint32_t fNum = mTHandler->AcceptRequest(evt.getRequest());
    if (fNum != -1)
    {
        m_friendsBox->Insert("pending - " + std::to_string(fNum) + " - pending", fNum);
    }
}

void MainFrame::OnFriendStatusUpdate(wxCommandEvent &evt)
{
    switch (evt.GetId())
    {
    case FriendUpdate::MESSAGE:
        if (m_messageBox->GetCurrFriend() == evt.GetInt())
        {
            m_messageBox->AddMessage(evt.GetString(), false);
        }
        else
        {
            m_messageBox->AddMessage(evt.GetInt(), evt.GetString());
        }
        break;
    case FriendUpdate::NAME:
    case FriendUpdate::STATUS_MSG:
    case FriendUpdate::CON_STATUS:
        auto f = mTHandler->GetFriend(evt.GetInt());
        m_friendsBox->SetString(evt.GetInt(), wxString(f->name) + " - " + std::to_string(f->friend_num) + " - " + wxString(f->status_message));
        break;
    }
}
