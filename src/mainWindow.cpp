#include "mainWindow.h"

CMainWindow::CMainWindow(uint modifier, uint key, int transitionDuration, QWidget *parent) : QMainWindow(parent), ui(new Ui::CMainWindow), visible(false),
  transitionDuration(transitionDuration), hotkey(NULL), opacityAnim(NULL), updateStatsTimer(NULL)
{
  ui->setupUi(this);
  setGeometry(70, QApplication::desktop()->screenGeometry().height() - 80 - height(), width(), height());

  setAttribute(Qt::WA_TranslucentBackground);
  setAttribute(Qt::WA_NoSystemBackground);
  setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

  dwm.load(reinterpret_cast<HWND>(winId()));
  //dwm.extendFrameIntoClientArea(SDwmMargins(-1, -1, -1, -1));

  SAccentPolicy accentPolicy = SAccentPolicy(NDwm::ACCENT_ENABLE_BLURBEHIND, 0, 0, 0);
  SWinCompAttrData compAttrData = SWinCompAttrData(NDwm::WCA_CLOAKED, &accentPolicy, sizeof(SAccentPolicy));
  dwm.setWindowCompositionAttribute(&compAttrData);

  hotkey = new CHotkey(modifier, key, this);
  connect(hotkey, SIGNAL(keyPress(int)), this, SLOT(keyPress(int)));
  hotkey->start();

  opacityAnim = new QPropertyAnimation(this, "windowOpacity", this);
  connect(opacityAnim, SIGNAL(finished()), this, SLOT(transitionFinished()));

  updateStatsTimer = new QTimer(this);
  updateStatsTimer->setInterval(1000);
  connect(updateStatsTimer, SIGNAL(timeout()), this, SLOT(updateStats()));

  uiCss = styleSheet();
  setWindowOpacity(0.0);
}

CMainWindow::~CMainWindow()
{
  hotkey->stop();

  delete ui;
}

void CMainWindow::updateStats()
{
  //qDebug() << "updateStats";

  QDateTime dt = QDateTime::currentDateTime();
  ui->lbClock->setText(dt.toString("h:mm"));
  ui->lbDay->setText(dt.toString("dddd"));
  ui->lbDate->setText(dt.toString("d. MMMM"));
}

void CMainWindow::updateColor()
{
  //qDebug() << "updateColor";

  int opaque = 0;
  unsigned long color = 0;
  dwm.getColorizationColor(&color, &opaque);
  QColor dwmColor(color);

  QString uiCss2(uiCss);
  uiCss2.replace("black", QString("rgba(%1, %2, %3, %4)").arg(dwmColor.red() / 2).arg(dwmColor.green() / 2).arg(dwmColor.blue() / 2).arg(200));
  setStyleSheet(uiCss2);
}

void CMainWindow::keyPress(int key)
{
  Q_UNUSED(key);
  //qDebug() << "press " << key;

  if(visible)
    changeVisibility(false);
  else
    changeVisibility(true);
}

void CMainWindow::changeVisibility(bool visible)
{
  //qDebug() << visible << windowOpacity();

  if(visible)
  {
    updateStats();
    updateColor();
    show();
    raise();
    opacityAnim->stop();
    opacityAnim->setDuration((1.0 - windowOpacity()) * transitionDuration);
    opacityAnim->setStartValue(windowOpacity());
    opacityAnim->setEndValue(1.0);
    opacityAnim->start();
    updateStatsTimer->start();
  }
  else
  {
    opacityAnim->stop();
    opacityAnim->setDuration(windowOpacity() * transitionDuration);
    opacityAnim->setStartValue(windowOpacity());
    opacityAnim->setEndValue(0.0);
    opacityAnim->start();
  }

  this->visible = visible;
}

void CMainWindow::transitionFinished()
{
  //qDebug() << windowOpacity();

  if(windowOpacity() == 0.0)
  {
    updateStatsTimer->stop();
    hide();
  }
}
