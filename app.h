#ifndef APP_H
#define APP_H

#include <wx/wx.h>
#include <tox/tox.h>

#include "main-frame.h"
#include "tox-handler.h"

class App : public wxApp
{
public:
    App();
    ~App();

    virtual bool OnInit() override;
    virtual int OnExit() override;

private:
    Tox *mTox {nullptr};
    MainFrame *mMainFrame {nullptr};
    ToxHandler *mTHandler {nullptr};
    bool mIsRunning;
};

#endif