#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->Quit, &QPushButton::pressed, qApp->quit);

    // Init FMOD library
    FMOD::System_Create(&_system);
    _system->init(4, FMOD_INIT_NORMAL, NULL);
}

MainWindow::~MainWindow()
{
    _system->close();
    delete ui;
}
