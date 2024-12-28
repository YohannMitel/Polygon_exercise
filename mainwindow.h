#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionShow_triangles_toggled(bool state);
    void on_actionShowLine_toggled(bool state);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
