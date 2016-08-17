#include "mainWindow.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  CMainWindow w(/*MOD_WIN | */MOD_ALT, 0x43, 200);

  return a.exec();
}
