#ifndef WORKSPACEWINDOW_H
#define WORKSPACEWINDOW_H

#include <QMainWindow>

namespace Ui {
class WorkspaceWindow;
}

class WorkspaceWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit WorkspaceWindow(QWidget *parent = 0);
    ~WorkspaceWindow();

private:
    Ui::WorkspaceWindow *ui;
};

#endif // WORKSPACEWINDOW_H
