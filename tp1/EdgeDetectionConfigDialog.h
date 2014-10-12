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

public:

    explicit EdgeDetectionConfigDialog(QWidget *parent = 0);
    ~EdgeDetectionConfigDialog();

    GradientKernel getSelectedKernel() const;

private:

    Ui::EdgeDetectionConfigDialog *ui;
};

#endif // EDGEDETECTIONConfigDIALOG_H
