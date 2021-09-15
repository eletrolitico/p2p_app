#include "tox-handler.h"

#include <sstream>
#include <iomanip>

wxDEFINE_EVENT(wxEVT_TOX_INIT, wxCommandEvent);
wxDEFINE_EVENT(wxEVT_TOX_FRIEND_ADD, wxCommandEvent);
wxDEFINE_EVENT(wxEVT_TOX_FRIEND_STATUS, wxCommandEvent);

typedef struct DHT_node
{
    const char *ip;
    uint16_t port;
    const char key_hex[TOX_PUBLIC_KEY_SIZE * 2 + 1];
} DHT_node;

DHT_node bootstrap_nodes[] = {

    // Setup tox bootrap nodes

    {"198.199.98.108", 33445, "BEF0CFB37AF874BD17B9A8F9FE64C75521DB95A37D33C5BDB00E9CF58659C04F"},
    {"205.185.115.131", 53, "3091C6BEB2A993F1C6300C16549FABA67098FF3D62C6D253828B531470B53D68"},
    {"tox2.abilinski.com", 33445, "7A6098B590BDC73F9723FC59F82B3F9085A64D1B213AAF8E610FD351930D052D"}

};

Tox *mTox = NULL;
// Mas isso vai dar ruim de um jeito
MainFrame *local_mFrame = NULL;
Friend self;
std::vector<Friend *> mFriends;
std::vector<Request> mRequests;
const char *savedata_filename = "./savedata.tox";
const char *savedata_tmp_filename = "./savedata.tox.tmp";

/*******************************************************************************
 *
 * Utils
 *
 ******************************************************************************/

uint8_t *hex2bin(const char *hex)
{
    size_t len = strlen(hex) / 2;
    uint8_t *bin = (uint8_t *)malloc(len);

    for (size_t i = 0; i < len; ++i, hex += 2)
    {
        sscanf(hex, "%2hhx", &bin[i]);
    }

    return bin;
}

std::string bin2hex(const uint8_t *bin, const size_t len)
{
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (size_t i = 0; i < len; ++i)
    {
        ss << std::hex << std::setw(2) << static_cast<int>(bin[i]);
    }
    return wxString(ss.str()).Upper().ToStdString();
}

Friend *getfriend(uint32_t fid)
{
    for (Friend *fnd : mFriends)
        if (fnd->friend_num == fid)
        {
            return fnd;
            break;
        }
    return NULL;
}

const char *connection_enum2text(TOX_CONNECTION conn)
{
    switch (conn)
    {
    case TOX_CONNECTION_NONE:
        return "Offline";
    case TOX_CONNECTION_TCP:
        return "Online(TCP)";
    case TOX_CONNECTION_UDP:
        return "Online(UDP)";
    default:
        return "UNKNOWN";
    }
}

void update_savedata_file(void)
{
    if (!(savedata_filename && savedata_tmp_filename))
        return;

    size_t size = tox_get_savedata_size(mTox);
    char *savedata = (char *)malloc(size);
    tox_get_savedata(mTox, (uint8_t *)savedata);

    FILE *f = fopen(savedata_tmp_filename, "wb");
    fwrite(savedata, size, 1, f);
    fclose(f);

    rename(savedata_tmp_filename, savedata_filename);

    free(savedata);
}

/*******************************************************************************
 *
 * Tox Callbacks
 *
 ******************************************************************************/

void friend_message_cb(Tox *tox, uint32_t friend_num, TOX_MESSAGE_TYPE type, const uint8_t *message, size_t length, void *user_data)
{
    Friend *f = getfriend(friend_num);

    if (!f)
        return;

    if (type != TOX_MESSAGE_TYPE_NORMAL)
    {
        printf("* receive MESSAGE ACTION type from %s, no supported\n", f->name);
        return;
    }

    f->hist.push_back(std::string((char *)message));

    auto evt = new wxCommandEvent(wxEVT_TOX_FRIEND_STATUS);
    evt->SetInt(friend_num);
    evt->SetString(wxString((char *)message));
    evt->SetId(FriendUpdate::MESSAGE);
    wxQueueEvent(local_mFrame, evt);

    printf("* receive message from %s\n", f->name);
}

void friend_name_cb(Tox *tox, uint32_t friend_num, const uint8_t *name, size_t length, void *user_data)
{
    Friend *f = getfriend(friend_num);

    if (f)
    {
        f->name = (char *)realloc(f->name, length + 1);
        sprintf(f->name, "%.*s", (int)length, (char *)name);

        auto evt = new wxCommandEvent(wxEVT_TOX_FRIEND_STATUS);
        evt->SetInt(friend_num);
        evt->SetId(FriendUpdate::NAME);
        wxQueueEvent(local_mFrame, evt);
    }
}

void friend_status_message_cb(Tox *tox, uint32_t friend_num, const uint8_t *message, size_t length, void *user_data)
{
    Friend *f = getfriend(friend_num);
    if (f)
    {
        f->status_message = (char *)realloc(f->status_message, length + 1);
        sprintf(f->status_message, "%.*s", (int)length, (char *)message);

        auto evt = new wxCommandEvent(wxEVT_TOX_FRIEND_STATUS);
        evt->SetInt(friend_num);
        evt->SetId(FriendUpdate::STATUS_MSG);
        wxQueueEvent(local_mFrame, evt);
    }
}

void friend_connection_status_cb(Tox *tox, uint32_t friend_num, TOX_CONNECTION connection_status, void *user_data)
{
    Friend *f = getfriend(friend_num);
    if (f)
    {
        f->connection = connection_status;
        printf("* %s is %s", f->name, connection_enum2text(connection_status));
    }

    auto evt = new wxCommandEvent(wxEVT_TOX_FRIEND_STATUS);
    evt->SetInt(friend_num);
    evt->SetId(FriendUpdate::CON_STATUS);
    wxQueueEvent(local_mFrame, evt);
}

void friend_request_cb(Tox *tox, const uint8_t *public_key, const uint8_t *message, size_t length, void *user_data)
{
    Request req;

    req.id = 1 + (mRequests.size() ? mRequests.at(mRequests.size() - 1).id : 0);
    req.is_friend_request = true;
    memcpy(req.userdata.pubkey, public_key, TOX_PUBLIC_KEY_SIZE);
    req.msg = (char *)malloc(length + 1);
    sprintf(req.msg, "%.*s", (int)length, (char *)message);

    mRequests.push_back(req);
    wxQueueEvent(local_mFrame, new wxCommandEvent(wxEVT_TOX_FRIEND_ADD));
}

void self_connection_status_cb(Tox *tox, TOX_CONNECTION connection_status, void *user_data)
{
    self.connection = connection_status;
    printf("* You are %s\n", connection_enum2text(connection_status));
}

void ToxHandler::create_tox()
{
    struct Tox_Options *options = tox_options_new(NULL);
    tox_options_set_start_port(options, PORT_RANGE_START);
    tox_options_set_end_port(options, PORT_RANGE_END);
    tox_options_set_experimental_thread_safety(options, true);

    TOX_ERR_NEW err_new;

    if (savedata_filename)
    {
        FILE *f = fopen(savedata_filename, "rb");
        if (f)
        {
            fseek(f, 0, SEEK_END);
            long fsize = ftell(f);
            fseek(f, 0, SEEK_SET);

            char *savedata = (char *)malloc(fsize);
            fread(savedata, fsize, 1, f);
            fclose(f);

            tox_options_set_savedata_type(options, TOX_SAVEDATA_TYPE_TOX_SAVE);
            tox_options_set_savedata_data(options, (uint8_t *)savedata, fsize);

            mTox = tox_new(options, &err_new);
            if (err_new != TOX_ERR_NEW_OK)
            {
                fprintf(stderr, "tox_new failed with error code %d\n", err_new);
                exit(1);
            }

            free(savedata);
        }
    }

    if (!mTox)
    {
        mTox = tox_new(options, &err_new);
        if (err_new != TOX_ERR_NEW_OK)
        {
            fprintf(stderr, "tox_new failed with error code %d\n", err_new);
            exit(1);
        }
    }
    tox_options_free(options);

    uint8_t tox_id_bin[TOX_ADDRESS_SIZE];
    tox_self_get_address(mTox, tox_id_bin);
    m_toxID = bin2hex(tox_id_bin, TOX_ADDRESS_SIZE);
    //std::cout << "ToxID: " << m_toxID << std::endl;
}

Friend *ToxHandler::add_friend(uint32_t fNum)
{
    Friend *f = (Friend *)calloc(1, sizeof(Friend));
    f->friend_num = fNum;
    f->connection = TOX_CONNECTION_NONE;
    tox_friend_get_public_key(mTox, fNum, f->pubkey, NULL);
    mFriends.push_back(f);
    return f;
}

void ToxHandler::init_friends()
{
    size_t sz = tox_self_get_friend_list_size(mTox);
    uint32_t *friend_list = (uint32_t *)malloc(sizeof(uint32_t) * sz);
    tox_self_get_friend_list(mTox, friend_list);

    size_t len;

    for (size_t i = 0; i < sz; i++)
    {
        uint32_t friend_num = friend_list[i];
        Friend *f = add_friend(friend_num);

        len = tox_friend_get_name_size(mTox, friend_num, NULL) + 1;
        f->name = (char *)calloc(1, len);
        tox_friend_get_name(mTox, friend_num, (uint8_t *)f->name, NULL);

        len = tox_friend_get_status_message_size(mTox, friend_num, NULL) + 1;
        f->status_message = (char *)calloc(1, len);
        tox_friend_get_status_message(mTox, friend_num, (uint8_t *)f->status_message, NULL);

        tox_friend_get_public_key(mTox, friend_num, f->pubkey, NULL);
    }
    free(friend_list);

    // add self
    self.friend_num = UINT32_MAX;
    len = tox_self_get_name_size(mTox) + 1;
    self.name = (char *)calloc(1, len);
    tox_self_get_name(mTox, (uint8_t *)self.name);

    len = tox_self_get_status_message_size(mTox) + 1;
    self.status_message = (char *)calloc(1, len);
    tox_self_get_status_message(mTox, (uint8_t *)self.status_message);

    tox_self_get_public_key(mTox, self.pubkey);

    m_name = std::string(self.name);
}

void ToxHandler::update_savedata_file()
{
    if (!(savedata_filename && savedata_tmp_filename))
        return;

    size_t size = tox_get_savedata_size(mTox);
    char *savedata = (char *)malloc(size);
    tox_get_savedata(mTox, (uint8_t *)savedata);

    FILE *f = fopen(savedata_tmp_filename, "wb");
    fwrite(savedata, size, 1, f);
    fclose(f);

    rename(savedata_tmp_filename, savedata_filename);

    free(savedata);
}

void ToxHandler::bootstrap()
{
    for (size_t i = 0; i < sizeof(bootstrap_nodes) / sizeof(struct DHT_node); i++)
    {
        uint8_t *bin = hex2bin(bootstrap_nodes[i].key_hex);
        tox_bootstrap(mTox, bootstrap_nodes[i].ip, bootstrap_nodes[i].port, bin, NULL);
        free(bin);
    }
}

void ToxHandler::setup_tox()
{
    local_mFrame = mFrame;
    create_tox();
    init_friends();
    wxQueueEvent(mFrame, new wxCommandEvent(wxEVT_TOX_INIT));

    ////// register callbacks

    // self
    tox_callback_self_connection_status(mTox, self_connection_status_cb);

    // friend
    tox_callback_friend_request(mTox, friend_request_cb);
    tox_callback_friend_message(mTox, friend_message_cb);
    tox_callback_friend_name(mTox, friend_name_cb);
    tox_callback_friend_status_message(mTox, friend_status_message_cb);
    tox_callback_friend_connection_status(mTox, friend_connection_status_cb);

    bootstrap();
}

void *ToxHandler::Entry()
{
    setup_tox();

    std::cout << "Connecting..." << std::endl;

    while (!TestDestroy())
    {
        tox_iterate(mTox, NULL);
        this->Sleep(tox_iteration_interval(mTox));
    }

    update_savedata_file();
    tox_kill(mTox);
    printf("Exiting thread...\n");

    return 0;
}

void ToxHandler::SetName(const std::string &str)
{
    m_name = str;
    tox_self_set_name(mTox, (uint8_t *)str.c_str(), str.size(), NULL);
    auto stsMsg = "sts msg";
    tox_self_set_status_message(mTox, (uint8_t *)stsMsg, strlen(stsMsg), NULL);
}

std::vector<Request> ToxHandler::GetRequests()
{
    return mRequests;
}

std::vector<Friend *> ToxHandler::GetFriends()
{
    return mFriends;
}

Friend *ToxHandler::GetFriend(uint32_t fNum)
{
    return getfriend(fNum);
}

void ToxHandler::SendMessage(uint32_t fNum, wxString msg)
{
    TOX_ERR_FRIEND_SEND_MESSAGE err;
    tox_friend_send_message(mTox, fNum, TOX_MESSAGE_TYPE_NORMAL, (uint8_t *)msg.c_str().AsChar(), msg.length(), &err);
    if (err != TOX_ERR_FRIEND_SEND_MESSAGE_OK)
    {
        std::cout << "Erro ao mandar msg: " << err << std::endl;
    }
}

uint32_t ToxHandler::AddFriend(wxString toxID, wxString msg)
{
    std::cout << "Id antes de coisar " << toxID << "... msg: " << msg << std::endl;
    const char *tId = toxID.c_str().AsChar();
    printf("Id com printf: %s\n", tId);

    uint8_t *bin_id = hex2bin(tId);
    TOX_ERR_FRIEND_ADD err;
    uint32_t friend_num = tox_friend_add(mTox, bin_id, (uint8_t *)msg.c_str().AsChar(), msg.length(), &err);
    free(bin_id);

    if (err != TOX_ERR_FRIEND_ADD_OK)
    {
        printf("! add friend with id %s failed, errcode:%d\n", tId, err);
        return -1;
    }

    add_friend(friend_num);
    return friend_num;
}

uint32_t ToxHandler::AcceptRequest(Request req)
{
    TOX_ERR_FRIEND_ADD err;
    uint32_t friend_num = tox_friend_add_norequest(mTox, req.userdata.pubkey, &err);
    if (err != TOX_ERR_FRIEND_ADD_OK)
    {
        printf("! accept friend request failed, errcode:%d", err);
        return -1;
    }

    add_friend(friend_num);
    return friend_num;
}

ToxHandler::ToxHandler(MainFrame *mFrame) : wxThread(wxTHREAD_DETACHED), mFrame(mFrame)
{
}

ToxHandler::~ToxHandler()
{
    wxCriticalSectionLocker enter(mFrame->mTHandlerCS);
    // the thread is being destroyed; make sure not to leave dangling pointers around
    mFrame->mTHandler = NULL;
}