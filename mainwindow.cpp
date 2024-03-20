#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "Windows.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Lua Launcher");
    QAction::connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::createAboutPopup);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createAboutPopup()
{
    MessageBoxA(NULL, "A Lua script launcher made using Qt", "About", MB_OK);
}
