#ifndef TOX_HANDLER_H
#define TOX_HANDLER_H

#include <wx/wx.h>
#include <tox/tox.h>

class ToxHandler : public wxThread
{
public:
    ToxHandler(Tox *, bool *);
    ~ToxHandler();

    virtual void *Entry() override;

private:
    Tox *mTox;
    bool *mShouldRun;
};

#endif