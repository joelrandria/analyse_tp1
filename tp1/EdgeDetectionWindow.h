#ifndef EDGEDETECTIONWINDOW_H
#define EDGEDETECTIONWINDOW_H

#include <QMainWindow>

#include "Image.h"
#include "GradientKernel.h"
#include "GradientMap.h"

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

    const GradientMap _gradientMap;

public:

    EdgeDetectionWindow(const Image& image, const GradientKernel& kernel, QWidget *parent = 0);
    ~EdgeDetectionWindow();

private:

    void updateView() const;
};

#endif // EDGEDETECTIONWINDOW_H
