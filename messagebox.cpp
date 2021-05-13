#include "messagebox.h"

MessageBox::MessageBox(wxWindow *parent, wxWindowID winid, const wxPoint &pos, const wxSize &size, long style, const wxString &name)
    : wxScrolledWindow(parent, winid, pos, size, style, name), senderFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD)
{
    sizer = new wxBoxSizer(wxVERTICAL);

    for (size_t i = 0; i < 100; ++i)
    {
        std::string n = i % 2 == 0 ? "voce: teste " : "outro: teste ";
        n += std::to_string(i);
        wxStaticText *test = new wxStaticText(this, wxID_ANY, n);
        if (i % 2)
        {
            test->SetFont(senderFont);
            test->SetForegroundColour(wxColour("#00ff00"));
        }
        else
        {
            test->SetForegroundColour(wxColour("#5555ff"));
        }

        sizer->Add(test, 1, wxEXPAND);
    }

    this->SetSizer(sizer);

    this->FitInside();
    this->SetScrollRate(5, 5);

    this->ScrollToBottom();
}

void MessageBox::ScrollToBottom()
{
    int range = this->GetScrollRange(wxVERTICAL);
    this->Scroll(0, range);
}

void MessageBox::ClearMsg()
{
    this->DestroyChildren();
    this->FitInside();
}

void MessageBox::AddMessage(const wxString &msg, bool isSender)
{
    if (curFriend == -1)
        return;

    auto tp = std::make_tuple(msg, isSender);
    m_Messages[curFriend].push_back(tp);

    wxStaticText *txt = new wxStaticText(this, wxID_ANY, msg);
    if (isSender)
    {
        txt->SetFont(senderFont);
        txt->SetForegroundColour(wxColour("#00ff00"));
    }
    else
    {
        txt->SetForegroundColour(wxColour("#5555ff"));
    }
    wxSize sz = this->GetSize();
    txt->Wrap(sz.GetWidth());
    sizer->Add(txt, 0, wxEXPAND);

    this->FitInside();
    this->ScrollToBottom();
    this->Refresh();
}

void MessageBox::SwitchFriend(int fID)
{
    if (fID == curFriend)
        return;

    curFriend = fID;
    this->ClearMsg();

    for (auto m : m_Messages[curFriend])
    {
        auto txt = new wxStaticText(this, wxID_ANY, std::get<0>(m));
        if (std::get<1>(m))
        {
            txt->SetFont(senderFont);
            txt->SetForegroundColour(wxColour("#00ff00"));
        }
        else
        {
            txt->SetForegroundColour(wxColour("#5555ff"));
        }
        sizer->Add(txt, 1, wxEXPAND);
    }

    this->FitInside();
    this->ScrollToBottom();
}
