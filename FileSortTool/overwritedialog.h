#ifndef OVERWRITEDIALOG_H
#define OVERWRITEDIALOG_H

#include <QDialog>

namespace Ui {
class OverwriteDialog;
}

class OverwriteDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OverwriteDialog(QWidget *parent = nullptr);
    ~OverwriteDialog();

    enum DIALOG_RESULT {
        RESULT_UNKNOWN,
        RESULT_YES,
        RESULT_YES_TO_ALL,
        RESULT_NO,
        RESULT_NO_TO_ALL
    };

    int result();

private:
    DIALOG_RESULT mResult = RESULT_UNKNOWN;

private slots:
    void on_btnYes_clicked();
    void on_btnYesToAll_clicked();
    void on_btnNo_clicked();
    void on_btnNotoAll_clicked();

private:
    Ui::OverwriteDialog *ui;
};

Q_DECLARE_METATYPE(OverwriteDialog::DIALOG_RESULT)

#endif // OVERWRITEDIALOG_H
