#include "Tp1Application.h"
#include "Workspace.h"
#include "WorkspaceWindow.h"

Tp1Application* Tp1Application::_app = 0;

Tp1Application* tp1App()
{
    return (Tp1Application::_app);
}

Tp1Application::Tp1Application(int& argc, char** argv)
    :QApplication(argc, argv)
{
    _app = this;
}

void Tp1Application::addNewWorkspace()
{
    Workspace* ws = new Workspace();
    _workspaces.push_back(ws);

    createWorkspaceView(ws);
}
void Tp1Application::addNewWorkspace(const std::string &imagePath)
{
    Workspace* ws = new Workspace(imagePath);
    _workspaces.push_back(ws);

    createWorkspaceView(ws);
}

void Tp1Application::createWorkspaceView(Workspace* workspace)
{
    WorkspaceWindow* wsWin = new WorkspaceWindow(workspace);
    wsWin->show();
}
