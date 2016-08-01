#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QDesktopWidget>
#include <QDateTime>
#include <QDate>
#include <QTime>
#include <QTimer>
#include <QEvent>
#include <QKeyEvent>
#include <QRect>
#include <QPropertyAnimation>
#include <QColor>
#include <QDebug>

#include "ui_mainWindow.h"
#include "hotkey.h"
#include "dwm.h"

namespace Ui
{
  class CMainWindow;
}

class CMainWindow : public QMainWindow
{
  private:
    Q_OBJECT

    Ui::CMainWindow *ui;

    bool visible;
    int transitionDuration;
    QString uiCss;

    CDwm dwm;
    CHotkey *hotkey;
    QPropertyAnimation *opacityAnim;
    QTimer *updateStatsTimer;

  protected slots:
    void updateStats();
    void updateColor();
    void keyPress(int key);
    void transitionFinished();

  public:
    explicit CMainWindow(uint modifier, uint key, int transitionDuration, QWidget *parent = NULL);
    ~CMainWindow();

    void changeVisibility(bool visible);
};

#endif // MAINWINDOW_H
