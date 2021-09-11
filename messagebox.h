#include <wx/wx.h>

#include <unordered_map>
#include <vector>

class MessageBox : public wxScrolledWindow
{
private:
  wxFont senderFont;
  wxBoxSizer *sizer;
  std::unordered_map<int, std::vector<std::tuple<wxString, bool>>> m_Messages;
  uint32_t curFriend = -1;

public:
  MessageBox(wxWindow *parent,
             wxWindowID winid = -1,
             const wxPoint &pos = wxDefaultPosition,
             const wxSize &size = wxDefaultSize,
             long style = -1073741824L,
             const wxString &name = wxString::FromAscii(wxPanelNameStr));

  void ScrollToBottom();
  void ClearMsg();

  void AddMessage(const wxString &msg, bool isSender);
  void AddMessage(uint32_t fNum, const wxString &msg);
  void SwitchFriend(uint32_t fID);
  inline uint32_t GetCurrFriend() { return curFriend; }
};
