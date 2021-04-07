#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/*实现打印浏览并生成PDF文件*/

#include <QMainWindow>
#include <QPrinter>
#include <QFileDialog>
#include <QTextEdit>
#include <QPushButton>
#include <QPrintPreviewDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTextEdit *textEdit;
    QPushButton *pushButton;
    QPushButton *pushButton_printPreview;
    QPrintPreviewDialog *printPreview;

private slots:
    void CreatPdf();
    void PrintPreview();
    void Preview(QPrinter *printer);
};
#endif // MAINWINDOW_H
