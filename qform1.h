#ifndef QFORM1_H
#define QFORM1_H

#include <QMainWindow>
#include "qpaintbox.h"
#include <QPainter>
#include <QPixmap>
#include <QTimer>
#include "ball.h"
#include "debug.h"

QT_BEGIN_NAMESPACE
namespace Ui { class QForm1; }
QT_END_NAMESPACE

class QResizeEvent;

class QForm1 : public QMainWindow
{
    Q_OBJECT

public:
    QForm1();
    ~QForm1();

private slots:
    void onQPB_MouseMove(QMouseEvent *event);
    void onQPB_MousePress(QMouseEvent *event);
    void onQPB_MouseRelease(QMouseEvent *event);
    void onQTimer1();

    void drawBall(Ball *ball, bool update);

    void resizeEvent(QResizeEvent *event) override;

    void on_actionDeveloper_Mode_triggered();

    void on_actionLaunch_triggered();

private:
    Ui::QForm1 *ui;
    QPaintBox *QPaintBox1;
    QPixmap *basePixmap;
    QTimer *QTimer1;
    Ball *Ball1;
    bool ballPressed=false;

    Debug *dev;

    void QPB_initialize();
};
#endif // QFORM1_H
