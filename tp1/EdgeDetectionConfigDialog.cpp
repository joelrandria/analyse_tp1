#include "EdgeDetectionConfigDialog.h"
#include "ui_EdgeDetectionConfigDialog.h"

EdgeDetectionConfigDialog::EdgeDetectionConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EdgeDetectionConfigDialog)
{
    ui->setupUi(this);
}
EdgeDetectionConfigDialog::~EdgeDetectionConfigDialog()
{
    delete ui;
}

GradientKernel EdgeDetectionConfigDialog::getSelectedKernel() const
{
    GradientKernel gk;

    if (ui->maskTypeComboBox->currentText().contains("Prewitt"))
    {
        if (ui->filteringTypeComboBox->currentText().contains("Bi-directionnel"))
            gk = GradientKernel::Prewitt2D();
    }

    return gk;
}
