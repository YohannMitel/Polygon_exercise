#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->statusbar->showMessage("Welcome QtPolygonsExercice for Master IOT",2000);
    QObject::connect(ui->canvas, SIGNAL(updateSB(QString)), ui->statusbar, SLOT(showMessage(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionShow_triangles_toggled(bool state) {
    ui->statusbar->showMessage(state?"Show triangles":"Hide triangles",2000);
    ui->canvas->showTriangles(state);
}




void MainWindow::on_actionShowLine_toggled(bool state)
{
    ui->statusbar->showMessage(state?"Show closest line":"Hide closest line",2000);
    ui->canvas->showClosestLine(state);
}

