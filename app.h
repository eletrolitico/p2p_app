#ifndef APP_H
#define APP_H

#include <wx/wx.h>
#include <tox/tox.h>

#include "main.h"

class App : public wxApp
{
public:
    App();
    ~App();

    virtual bool onInit();

private:
    Tox *mTox;
    Main *mMainFrame = nullptr;
};

#endif