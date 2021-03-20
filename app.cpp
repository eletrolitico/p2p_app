#include <ctype.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#include "app.h"
#include "tox-handler.h"

wxIMPLEMENT_APP(App);

App::App() {}

App::~App() {}

bool App::OnInit()
{
    mMainFrame = new MainFrame();
    mMainFrame->Show();
    mMainFrame->CenterOnScreen();
    
    printf("It's running!\n");
    
    return true;
}

int App::OnExit()
{
    return 0;
}