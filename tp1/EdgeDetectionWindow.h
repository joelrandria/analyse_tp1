#ifndef EDGEDETECTIONWINDOW_H
#define EDGEDETECTIONWINDOW_H

#include <QMainWindow>
#include <QTime>

#include "Image.h"
#include "GradientKernel.h"
#include "GradientMapMax.h"

namespace Ui {
class EdgeDetectionWindow;
}

class EdgeDetectionWindow : public QMainWindow
{
    Q_OBJECT

private:

    Ui::EdgeDetectionWindow *ui;

    const Image& _image;
    const GradientKernel& _kernel;

    GradientMapMax _gradientMapMax;

    int _hysterisisLowThreshold;
    int _hysterisisHighThreshold;

public:

    EdgeDetectionWindow(const Image& image,
                        const GradientKernel& kernel,
                        int hysterisisLowThreshold,
                        int hysterisisHighThreshold,
                        QWidget *parent = 0);
    ~EdgeDetectionWindow();

private slots:
    void on_actionEnregistrer_triggered();

private:

    void updateView();
};

#endif // EDGEDETECTIONWINDOW_H
