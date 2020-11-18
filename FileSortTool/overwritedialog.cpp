#include "overwritedialog.h"
#include "ui_overwritedialog.h"

OverwriteDialog::OverwriteDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OverwriteDialog)
{
    ui->setupUi(this);
}

OverwriteDialog::~OverwriteDialog()
{
    delete ui;
}

int OverwriteDialog::result()
{
    return mResult;
}

void OverwriteDialog::on_btnYes_clicked()
{
    mResult = RESULT_YES;
    this->close();
}

void OverwriteDialog::on_btnYesToAll_clicked()
{
    mResult = RESULT_YES_TO_ALL;
    this->close();
}

void OverwriteDialog::on_btnNo_clicked()
{
    mResult = RESULT_NO;
    this->close();
}

void OverwriteDialog::on_btnNotoAll_clicked()
{
    mResult = RESULT_NO_TO_ALL;
    this->close();
}
