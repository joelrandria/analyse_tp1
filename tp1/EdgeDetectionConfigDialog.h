#ifndef EDGEDETECTIONConfigDIALOG_H
#define EDGEDETECTIONConfigDIALOG_H

#include <QDialog>

#include "GradientKernel.h"

namespace Ui {
class EdgeDetectionConfigDialog;
}

class EdgeDetectionConfigDialog : public QDialog
{
    Q_OBJECT

private:

    Ui::EdgeDetectionConfigDialog *ui;

public:

    explicit EdgeDetectionConfigDialog(QWidget *parent = 0);
    ~EdgeDetectionConfigDialog();

    GradientKernel getSelectedKernel() const;

    int hysterisisLowThreshold() const;
    int hysterisisHighThreshold() const;

private slots:

    void on_highThresholdLineEdit_valueChanged(int arg1);
    void on_lowThresholdLineEdit_valueChanged(int arg1);
};

#endif // EDGEDETECTIONConfigDIALOG_H
