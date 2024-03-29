wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_CLOSE(MainFrame::OnClose)
    EVT_BUTTON(SEND_BTN, MainFrame::OnSendMessage)
    EVT_BUTTON(NAME_BTN, MainFrame::OnChangeName)
    EVT_BUTTON(ADD_BTN, MainFrame::OnFriendAdd)
    EVT_LISTBOX(wxID_ANY,MainFrame::OnSwitchFriend)
    EVT_COMMAND(wxID_ANY, wxEVT_TOX_INIT, MainFrame::OnToxID)
    EVT_COMMAND(wxID_ANY, wxEVT_TOX_FRIEND_ADD, MainFrame::OnFriendRequest)
    EVT_COMMAND(wxID_ANY, wxEVT_TOX_FRIEND_STATUS, MainFrame::OnFriendStatusUpdate)
wxEND_EVENT_TABLE()