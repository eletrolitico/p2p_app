#ifndef TOX_HANDLER_H
#define TOX_HANDLER_H

#include <wx/wx.h>
#include <tox/tox.h>
#include <vector>

#include "main-frame.h"

#define PORT_RANGE_START 33445 // tox listen port range
#define PORT_RANGE_END 34445

class MainFrame;

wxDECLARE_EVENT(wxEVT_TOX_ID, wxThreadEvent);

typedef struct
{
    uint32_t friend_num;
    char *name;
    char *status_message;
    uint8_t pubkey[TOX_PUBLIC_KEY_SIZE];
    TOX_CONNECTION connection;

    std::vector<std::string> hist;
} Friend;

struct FriendUserData
{
    uint8_t pubkey[TOX_PUBLIC_KEY_SIZE];
};

typedef struct
{
    char *msg;
    uint32_t id;
    bool is_friend_request;
    FriendUserData userdata;
} Request;

class ToxHandler : public wxThread
{
public:
    ToxHandler(MainFrame *);
    ~ToxHandler();

    virtual ExitCode Entry() override;
    std::string m_toxID;

private:
    MainFrame *mFrame;

    void setup_tox();
    void create_tox();
    void init_friends();
    void bootstrap();
    void update_savedata_file();
    Friend *add_friend(uint32_t);
};
#endif