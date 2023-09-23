#ifndef DEBUG_H
#define DEBUG_H

#include <QDialog>

namespace Ui {
class Debug;
}

class Debug : public QDialog
{
    Q_OBJECT

public:
    explicit Debug(QWidget *parent = nullptr);
    ~Debug();

private slots:
    void on_dial_sliderMoved(int value);

private:
    Ui::Debug *ui;
};

#endif // DEBUG_H
