#ifndef WORKSPACEWINDOW_H
#define WORKSPACEWINDOW_H

#include <QMainWindow>

namespace Ui {
class WorkspaceWindow;
}

class Workspace;

class WorkspaceWindow : public QMainWindow
{
    Q_OBJECT

private:

    Ui::WorkspaceWindow *ui;

    Workspace* _workspace;

public:

    explicit WorkspaceWindow(Workspace* workspace, QWidget *parent = 0);
    ~WorkspaceWindow();

private slots:

    // Menu Fichier
    void on_actionOpenFile_triggered();
    void on_actionCloseFile_triggered();
    void on_actionNewWorkspace_triggered();

    // Menu Traitements
    void on_actionEdgeDetection_triggered();

private:

    void updateImageView();
};

#endif // WORKSPACEWINDOW_H
