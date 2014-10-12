#ifndef EDGEDETECTIONDIALOG_H
#define EDGEDETECTIONDIALOG_H

#include <QDialog>

#include "GradientKernel.h"

namespace Ui {
class EdgeDetectionDialog;
}

class EdgeDetectionDialog : public QDialog
{
    Q_OBJECT

public:

    explicit EdgeDetectionDialog(QWidget *parent = 0);
    ~EdgeDetectionDialog();

    GradientKernel getSelectedKernel() const;

private:

    Ui::EdgeDetectionDialog *ui;
};

#endif // EDGEDETECTIONDIALOG_H
