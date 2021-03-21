#include <wx/wx.h>

class MessageDialog : public wxDialog
{
private:
  wxListBox *mMessageListBox{nullptr};   // messages received/sent
  wxTextCtrl *mMessageTextCtrl{nullptr}; // type a message
  wxButton *mSendBtn{nullptr};           // send a message

  void OnTxtEdit(wxCommandEvent &evt);
  void OnButtonClicked(wxCommandEvent &evt);

  void CreateInterface();

public:
  MessageDialog(const wxString &title);
};
