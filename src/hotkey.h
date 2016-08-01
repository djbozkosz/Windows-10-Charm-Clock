#ifndef HOTKEY_H
#define HOTKEY_H

#include <QObject>
#include <QThread>
#include <QApplication>
#include <QCoreApplication>
#include <QDebug>
#include <windows.h>

class CHotkey : public QThread
{
  private:
    Q_OBJECT

    DWORD id;
    uint modifier;
    uint key;

  protected:
    virtual void run();

  public:
    explicit CHotkey(uint modifier, uint key, QObject *parent = NULL);
    virtual ~CHotkey();

    inline DWORD getId() const { return id; }
    inline uint getModifier() const { return modifier; }
    inline uint getKey() const { return key; }

  public slots:
    void stop();

  signals:
    void keyPress(int key);
};

#endif // HOTKEY_H
