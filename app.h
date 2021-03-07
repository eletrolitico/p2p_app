#ifndef APP_H
#define APP_H

#include <wx/wx.h>
#include <tox/tox.h>

#include "main-frame.h"

class App : public wxApp
{
public:
    App();
    ~App();

    virtual bool OnInit() override;

private:
    Tox *mTox;
    Main *mMainFrame = nullptr;
};

#endif