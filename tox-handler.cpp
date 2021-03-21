#include "tox-handler.h"

#include <sodium/utils.h>

typedef struct DHT_node
{
    const char *ip;
    uint16_t port;
    const char key_hex[TOX_PUBLIC_KEY_SIZE * 2 + 1];
} DHT_node;

void friend_request_cb(Tox *tox, const uint8_t *public_key, const uint8_t *message, size_t length, void *user_data)
{
    TOX_ERR_FRIEND_ADD error;
    tox_friend_add_norequest(tox, public_key, &error);
    if (error != TOX_ERR_FRIEND_ADD_OK)
    {
        fprintf(stderr, "Erro adicionando amigo com cod: %d\n", error);
    }
    else
    {
        printf("adicionado amigo\n");
    }
}

void friend_message_cb(Tox *tox, uint32_t friend_number, TOX_MESSAGE_TYPE type, const uint8_t *message, size_t length, void *user_data)
{
    TOX_ERR_FRIEND_SEND_MESSAGE error;
    tox_friend_send_message(tox, friend_number, type, message, length, &error);
    if (error != TOX_ERR_FRIEND_SEND_MESSAGE_OK)
    {
        fprintf(stderr, "Erro enviando mensagem: %d\n", error);
    }
}

void self_connection_status_cb(Tox *tox, TOX_CONNECTION connection_status, void *user_data)
{
    switch (connection_status)
    {
    case TOX_CONNECTION_NONE:
        printf("Offline\n");
        break;
    case TOX_CONNECTION_TCP:
        printf("Online, using TCP\n");
        break;
    case TOX_CONNECTION_UDP:
        printf("Online, using UDP\n");
        break;
    }
}

ToxHandler::ToxHandler(Tox *t, bool *b, MainFrame *mFrame) : wxThread(wxTHREAD_DETACHED), mTox(t), mShouldRun(b), mFrame(mFrame)
{
}

ToxHandler::~ToxHandler() {}

void *ToxHandler::Entry()
{
    char myName[1024];
    strncpy(myName, (const char *)mFrame->mMyNameCtrl->GetValue().mb_str(wxConvUTF8), 1023);

    printf("my name is: %s\n", myName);

    tox_self_set_name(mTox, (uint8_t *)myName, strlen(myName), NULL);

    const char *status_message = "Echoing your messages";
    tox_self_set_status_message(mTox, (uint8_t *)status_message, strlen(status_message), NULL);

    DHT_node nodes[] = {{"85.143.221.42", 33445, "DA4E4ED4B697F2E9B000EEFE3A34B554ACD3F45F5C96EAEA2516DD7FF9AF7B43"},
                        {"2a04:ac00:1:9f00:5054:ff:fe01:becd", 33445, "DA4E4ED4B697F2E9B000EEFE3A34B554ACD3F45F5C96EAEA2516DD7FF9AF7B43"},
                        {"78.46.73.141", 33445, "02807CF4F8BB8FB390CC3794BDF1E8449E9A8392C5D3F2200019DA9F1E812E46"},
                        {"2a01:4f8:120:4091::3", 33445, "02807CF4F8BB8FB390CC3794BDF1E8449E9A8392C5D3F2200019DA9F1E812E46"},
                        {"tox.initramfs.io", 33445, "3F0A45A268367C1BEA652F258C85F4A66DA76BCAA667A49E770BCC4917AB6A25"},
                        {"tox2.abilinski.com", 33445, "7A6098B590BDC73F9723FC59F82B3F9085A64D1B213AAF8E610FD351930D052D"},
                        {"205.185.115.131", 53, "3091C6BEB2A993F1C6300C16549FABA67098FF3D62C6D253828B531470B53D68"},
                        {"tox.kurnevsky.net", 33445, "82EF82BA33445A1F91A7DB27189ECFC0C013E06E3DA71F588ED692BED625EC23"}};

    for (size_t i = 0; i < sizeof(nodes) / sizeof(DHT_node); i++)
    {
        unsigned char key_bin[TOX_PUBLIC_KEY_SIZE];
        sodium_hex2bin(key_bin, sizeof(key_bin), nodes[i].key_hex, sizeof(nodes[i].key_hex) - 1, NULL, NULL, NULL);
        tox_bootstrap(mTox, nodes[i].ip, nodes[i].port, key_bin, NULL);
    }

    uint8_t tox_id_bin[TOX_ADDRESS_SIZE];
    tox_self_get_address(mTox, tox_id_bin);

    char tox_id_hex[TOX_ADDRESS_SIZE * 2 + 1];
    sodium_bin2hex(tox_id_hex, sizeof(tox_id_hex), tox_id_bin, sizeof(tox_id_bin));

    for (size_t i = 0; i < sizeof(tox_id_hex) - 1; i++)
    {
        tox_id_hex[i] = toupper(tox_id_hex[i]);
    }

    mFrame->mMyIDCtrl->AppendText(wxString::FromUTF8(tox_id_hex));
    printf("Tox ID: %s\n", tox_id_hex);

    //mFrame->AddToClipBoard(tox_id_hex);

    tox_callback_friend_request(mTox, friend_request_cb);
    tox_callback_friend_message(mTox, friend_message_cb);
    tox_callback_self_connection_status(mTox, self_connection_status_cb);

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