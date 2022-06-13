#ifndef WINMAIN_H
#define WINMAIN_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class winMain; }
QT_END_NAMESPACE

class winMain : public QWidget
{
    Q_OBJECT

public:
    winMain(QWidget *parent = nullptr);
    ~winMain();

private:
    Ui::winMain *ui;
};
#endif // WINMAIN_H
