#include "WorkspaceWindow.h"
#include "ui_WorkspaceWindow.h"

WorkspaceWindow::WorkspaceWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WorkspaceWindow)
{
    ui->setupUi(this);
}

WorkspaceWindow::~WorkspaceWindow()
{
    delete ui;
}
