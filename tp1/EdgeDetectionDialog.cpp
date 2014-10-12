#include "EdgeDetectionDialog.h"
#include "ui_EdgeDetectionDialog.h"

EdgeDetectionDialog::EdgeDetectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EdgeDetectionDialog)
{
    ui->setupUi(this);
}
EdgeDetectionDialog::~EdgeDetectionDialog()
{
    delete ui;
}

GradientKernel EdgeDetectionDialog::getSelectedKernel() const
{
    GradientKernel gk;

    if (ui->maskTypeComboBox->currentText().contains("Prewitt"))
    {
        if (ui->filteringTypeComboBox->currentText().contains("Bi-directionnel"))
            gk = GradientKernel::Prewitt2D();
    }

    return gk;
}
