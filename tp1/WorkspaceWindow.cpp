#include "WorkspaceWindow.h"
#include "ui_WorkspaceWindow.h"
#include "Tp1Application.h"
#include "Workspace.h"
#include "EdgeDetectionConfigDialog.h"
#include "EdgeDetectionWindow.h"

#include <QFileDialog>
#include <QUrl>

#include <opencv2/core/core.hpp>

#include <iostream>

WorkspaceWindow::WorkspaceWindow(Workspace* workspace, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WorkspaceWindow)
{
    ui->setupUi(this);

    _workspace = workspace;

    updateImageView();
}
WorkspaceWindow::~WorkspaceWindow()
{
    delete ui;
}

void WorkspaceWindow::updateImageView()
{
    cv::Mat imageMatrix = _workspace->image().matrix();

    QImage* pImage = new QImage(imageMatrix.cols, imageMatrix.rows, QImage::Format_RGB888);

    for (int y = 0; y < imageMatrix.rows; ++y)
    {
        for (int x = 0; x < imageMatrix.cols; ++x)
        {
            cv::Vec3b pix = imageMatrix.at<cv::Vec3b>(cv::Point(x, y));
            pImage->setPixel(x, y, qRgb(pix[2], pix[1], pix[0]));
        }
    }

    ui->pixmapLabel->setPixmap(QPixmap::fromImage(*pImage));
}

void WorkspaceWindow::on_actionNewWorkspace_triggered()
{
    tp1App()->addNewWorkspace();
}
void WorkspaceWindow::on_actionOpenFile_triggered()
{
    QFileDialog fd(this, tr("Ouvrir une image"));
    fd.setFileMode(QFileDialog::ExistingFile);
    fd.setNameFilter(tr("Fichiers images (*.png *.jpg *.bmp)"));

    if (fd.exec())
    {
        QString filePath = fd.selectedFiles().first();
        _workspace->loadImage(filePath.toUtf8().data());

        updateImageView();
        setWindowTitle(QUrl(filePath).fileName());
    }
}
void WorkspaceWindow::on_actionCloseFile_triggered()
{
    close();
}

void WorkspaceWindow::on_actionEdgeDetection_triggered()
{
    EdgeDetectionConfigDialog configDialog(this);
    if (!configDialog.exec())
        return;

    EdgeDetectionWindow* detectionWindow = new EdgeDetectionWindow(_workspace->image(),
                                                                   configDialog.getSelectedKernel(),
                                                                   configDialog.hysterisisLowThreshold(),
                                                                   configDialog.hysterisisHighThreshold());
    detectionWindow->show();
}
