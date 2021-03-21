#include "tox-handler.h"

#include <sodium/utils.h>

typedef struct DHT_node
{
    const char *ip;
    uint16_t port;
    const char key_hex[TOX_PUBLIC_KEY_SIZE * 2 + 1];
} DHT_node;

DHT_node bootstrap_nodes[] = {

    // Setup tox bootrap nodes

    {"node.tox.biribiri.org", 33445, "F404ABAA1C99A9D37D61AB54898F56793E1DEF8BD46B1038B9D822E8460FAB67"},
    {"128.199.199.197", 33445, "B05C8869DBB4EDDD308F43C1A974A20A725A36EACCA123862FDE9945BF9D3E09"},
    {"2400:6180:0:d0::17a:a001", 33445, "B05C8869DBB4EDDD308F43C1A974A20A725A36EACCA123862FDE9945BF9D3E09"},
    {"85.143.221.42", 33445, "DA4E4ED4B697F2E9B000EEFE3A34B554ACD3F45F5C96EAEA2516DD7FF9AF7B43"},
    {"2a04:ac00:1:9f00:5054:ff:fe01:becd", 33445, "DA4E4ED4B697F2E9B000EEFE3A34B554ACD3F45F5C96EAEA2516DD7FF9AF7B43"},
    {"78.46.73.141", 33445, "02807CF4F8BB8FB390CC3794BDF1E8449E9A8392C5D3F2200019DA9F1E812E46"},
    {"2a01:4f8:120:4091::3", 33445, "02807CF4F8BB8FB390CC3794BDF1E8449E9A8392C5D3F2200019DA9F1E812E46"},
    {"tox.initramfs.io", 33445, "3F0A45A268367C1BEA652F258C85F4A66DA76BCAA667A49E770BCC4917AB6A25"},
    {"tox2.abilinski.com", 33445, "7A6098B590BDC73F9723FC59F82B3F9085A64D1B213AAF8E610FD351930D052D"},
    {"205.185.115.131", 53, "3091C6BEB2A993F1C6300C16549FABA67098FF3D62C6D253828B531470B53D68"},
    {"tox.kurnevsky.net", 33445, "82EF82BA33445A1F91A7DB27189ECFC0C013E06E3DA71F588ED692BED625EC23"},
};

Friend self;
std::vector<Friend *> mFriends;
std::vector<Request> mRequests;
const char *savedata_filename = "./savedata.tox";
const char *savedata_tmp_filename = "./savedata.tox.tmp";

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
        printf("* receive MESSAGE ACTION type from %s, no supported", f->name);
        return;
    }

    f->hist.push_back(std::string((char *)message));

    printf("* receive message from %s", f->name);
}

void friend_name_cb(Tox *tox, uint32_t friend_num, const uint8_t *name, size_t length, void *user_data)
{
    Friend *f = getfriend(friend_num);

    if (f)
    {
        f->name = (char *)realloc(f->name, length + 1);
        sprintf(f->name, "%.*s", (int)length, (char *)name);
    }
}

void friend_status_message_cb(Tox *tox, uint32_t friend_num, const uint8_t *message, size_t length, void *user_data)
{
    Friend *f = getfriend(friend_num);
    if (f)
    {
        f->status_message = (char *)realloc(f->status_message, length + 1);
        sprintf(f->status_message, "%.*s", (int)length, (char *)message);
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
}

void self_connection_status_cb(Tox *tox, TOX_CONNECTION connection_status, void *user_data)
{
    self.connection = connection_status;
    printf("* You are %s", connection_enum2text(connection_status));
}

void ToxHandler::create_tox()
{
    struct Tox_Options *options = tox_options_new(NULL);
    tox_options_set_start_port(options, PORT_RANGE_START);
    tox_options_set_end_port(options, PORT_RANGE_END);

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

            mTox = tox_new(options, NULL);

            free(savedata);
        }
    }

    if (!mTox)
        mTox = tox_new(options, NULL);
    tox_options_free(options);
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

    for (int i = 0; i < sz; i++)
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
    create_tox();
    init_friends();
    bootstrap();

    ////// register callbacks

    // self
    tox_callback_self_connection_status(mTox, self_connection_status_cb);

    // friend
    tox_callback_friend_request(mTox, friend_request_cb);
    tox_callback_friend_message(mTox, friend_message_cb);
    tox_callback_friend_name(mTox, friend_name_cb);
    tox_callback_friend_status_message(mTox, friend_status_message_cb);
    tox_callback_friend_connection_status(mTox, friend_connection_status_cb);
}

void *ToxHandler::Entry()
{
    setup_tox();
    printf("Connecting...\n");

    while (*mShouldRun)
    {
        tox_iterate(mTox, NULL);

        uint32_t v = tox_iteration_interval(mTox);
        this->Sleep(v);
    }

    tox_kill(mTox);
    printf("Exiting thread...\n");

    return 0;
}

ToxHandler::ToxHandler(Tox *t, bool *b, MainFrame *mFrame) : wxThread(wxTHREAD_DETACHED), mTox(t), mShouldRun(b), mFrame(mFrame)
{
}

ToxHandler::~ToxHandler() {}