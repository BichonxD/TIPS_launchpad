#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "fmod.hpp"
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    FMOD::System* getSystem() { return _system; }

private:
    Ui::MainWindow *ui;
    FMOD::System *_system;
};

#endif // MAINWINDOW_H
