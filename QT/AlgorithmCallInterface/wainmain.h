#ifndef WAINMAIN_H
#define WAINMAIN_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class wainMain; }
QT_END_NAMESPACE

class wainMain : public QWidget
{
    Q_OBJECT

public:
    wainMain(QWidget *parent = nullptr);
    ~wainMain();

private:
    Ui::wainMain *ui;
};
#endif // WAINMAIN_H
