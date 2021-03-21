#include <wx/wx.h>

class MessageDialog : public wxDialog
{
private:
  wxTextCtrl *tc{nullptr};
  wxListBox *lb{nullptr};

  void OnTxtEdit(wxCommandEvent &evt);

public:
  MessageDialog(const wxString &title);
};
