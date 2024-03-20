#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "Windows.h"
#include <time.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Lua Launcher");
    QAction::connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::createAboutPopup);
    log("Sucessfully initialized Lua Launcher");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createAboutPopup()
{
    MessageBoxA(NULL, "A Lua script launcher made using Qt", "About", MB_OK);
}

void MainWindow::log(QString msg)
{
    time_t timer;
    char buffer[12];
    struct tm* tm_info;

    timer = time(NULL);
    tm_info = localtime(&timer);

    strftime(buffer, 12, "[%H:%M:%S] ", tm_info);
    puts(buffer);
    QString str = buffer;
    str.append(msg);
    ui->log->append(str);
}
