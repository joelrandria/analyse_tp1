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

    if (ui->filteringTypeComboBox->currentText().contains("Bi-directionnel"))
    {
        if(ui->maskTypeComboBox->currentText().contains("Prewitt"))
        {
            gk = GradientKernel::Prewitt2D();
        }
        else if(ui->maskTypeComboBox->currentText().contains("Sobel"))
        {
            gk = GradientKernel::Sobel2D();
        }
        else if (ui->maskTypeComboBox->currentText().contains("Kirsch"))
        {
            gk = GradientKernel::Kirsch2D();
        }
    }
    else if (ui->filteringTypeComboBox->currentText().contains("Multi-directionnel"))
    {
        if(ui->maskTypeComboBox->currentText().contains("Prewitt"))
        {
            gk = GradientKernel::Prewitt4D();
        }
        else if(ui->maskTypeComboBox->currentText().contains("Sobel"))
        {
            gk = GradientKernel::Sobel4D();
        }
        else if (ui->maskTypeComboBox->currentText().contains("Kirsch"))
        {
            gk = GradientKernel::Kirsch4D();
        }
    }

    return gk;
}

int EdgeDetectionConfigDialog::hysterisisLowThreshold() const
{
    return ui->lowThresholdLineEdit->value();
}
int EdgeDetectionConfigDialog::hysterisisHighThreshold() const
{
    return ui->highThresholdLineEdit->value();
}

void EdgeDetectionConfigDialog::on_highThresholdLineEdit_valueChanged(int arg1)
{
    if (ui->lowThresholdSlider->value() > arg1)
        ui->lowThresholdSlider->setValue(arg1);
}
void EdgeDetectionConfigDialog::on_lowThresholdLineEdit_valueChanged(int arg1)
{
    if (ui->highThresholdSlider->value() < arg1)
        ui->highThresholdSlider->setValue(arg1);
}
