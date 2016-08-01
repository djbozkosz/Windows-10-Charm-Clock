#include "hotkey.h"

CHotkey::CHotkey(uint modifier, uint key, QObject *parent) : QThread(parent), id(0), modifier(modifier), key(key)
{
}

CHotkey::~CHotkey()
{
}

void CHotkey::run()
{
  bool ret;
  ATOM atom;
  MSG msg;

  id = GetCurrentThreadId();
  atom = GlobalAddAtom(MAKEINTATOM(0xAC92));
  if(QSysInfo::windowsVersion() > QSysInfo::WV_VISTA)
    ret = RegisterHotKey(NULL, atom, modifier | 0x4000, key);
  else
    ret = RegisterHotKey(NULL, atom, modifier, key);

  if(!ret)
  {
    qDebug() << "Unable to register a hotkey!";
    return;
  }

  memset(&msg, 0, sizeof(MSG));
  while(0 < GetMessage(&msg, NULL, 0, 0))
  {
    if(msg.message == WM_HOTKEY)
      emit keyPress(msg.lParam);
    else if(msg.message == (WM_USER + 2))
      break;
  }

  UnregisterHotKey(NULL, atom);
  GlobalDeleteAtom(atom);
}

void CHotkey::stop()
{
  if(id)
    PostThreadMessage(id, WM_USER + 2, 0, 0);
}
