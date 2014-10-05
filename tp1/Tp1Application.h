#ifndef TP1APPLICATION_H
#define TP1APPLICATION_H

#include <vector>
#include <string>

#include <QApplication>

class Workspace;

class Tp1Application : public QApplication
{
private:

    static Tp1Application* _app;

    std::vector<Workspace*> _workspaces;

public:

    Tp1Application(int& argc, char** argv);

    friend Tp1Application* tp1App();

    void addNewWorkspace();
    void addNewWorkspace(const std::string& imagePath);

private:

    void createWorkspaceView(Workspace* workspace);
};

extern Tp1Application* tp1App();

#endif // TP1APPLICATION_H
