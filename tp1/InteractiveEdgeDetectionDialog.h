#ifndef INTERACTIVEEDGEDETECTIONDIALOG_H
#define INTERACTIVEEDGEDETECTIONDIALOG_H

#include <QList>
#include <QMainWindow>

#include "Image.h"
#include "GradientKernel.h"
#include "GradientMapMax.h"
#include "ConnectedComponent.h"

namespace Ui {
class InteractiveEdgeDetectionDialog;
}

class InteractiveEdgeDetectionDialog : public QMainWindow
{
    Q_OBJECT

private:

    Ui::InteractiveEdgeDetectionDialog *ui;

    const Image& _image;
    GradientMapMax _gradientMapMax;
    QList<ConnectedComponent> _connectedComponents;

public:

    explicit InteractiveEdgeDetectionDialog(const Image& image, QWidget *parent = 0);
    ~InteractiveEdgeDetectionDialog();

    GradientKernel selectedKernel() const;

    int hysterisisLowThreshold() const;
    int hysterisisHighThreshold() const;

public slots:

    void updateView();

    void on_actionEnregistrer_sous_triggered();

private slots:

    void updateGradientMapMax();

    void on_maskTypeComboBox_currentIndexChanged(int index);
    void on_filteringTypeComboBox_currentIndexChanged(int index);

    void on_highThresholdSlider_valueChanged(int value);
    void on_highThresholdLineEdit_valueChanged(int arg1);
    void on_lowThresholdSlider_valueChanged(int value);
    void on_lowThresholdLineEdit_valueChanged(int arg1);
};

#endif // INTERACTIVEEDGEDETECTIONDIALOG_H
