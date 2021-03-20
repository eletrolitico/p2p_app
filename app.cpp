#include <ctype.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#include "app.h"

wxIMPLEMENT_APP(App);

App::App() {}

App::~App() {}

bool App::OnInit()
{

    mMainFrame = new MainFrame();
    mMainFrame->Show();
    mMainFrame->CenterOnScreen();

    printf("It's running!\n");

    mTox = tox_new(NULL, NULL);
    mIsRunning = true;
    //mTHandler = new ToxHandler(mTox, &mIsRunning);
    //mTHandler->Run();

    return true;
}

int App::OnExit()
{
    mIsRunning = false;
    //mTHandler->Wait();
    //delete mTHandler;
    return 0;
}