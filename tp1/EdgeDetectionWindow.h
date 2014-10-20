#ifndef EDGEDETECTIONWINDOW_H
#define EDGEDETECTIONWINDOW_H

#include <QMainWindow>

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

    EdgeDetectionWindow(const Image& image, const GradientKernel& kernel, QWidget *parent = 0);
    ~EdgeDetectionWindow();

    void setHysterisisThresholds(int low, int high)
    {
        _hysterisisLowThreshold = low;
        _hysterisisHighThreshold = high;
    }

private:

    void updateView();
};

#endif // EDGEDETECTIONWINDOW_H
