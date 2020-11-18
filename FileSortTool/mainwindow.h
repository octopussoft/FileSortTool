#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QString mInputPath = "";
    QString mOutputPath = "";
    bool mSortCriteriaModifyDate = false;
    bool mSortCriteriaCreationDate = false;
    bool mSortCriteriaRecursive = false;
    bool mSortCriteriaCopy = false;
    bool mSortOptionYear = false;
    bool mSortOptionMonth = false;
    bool mSortOptionDay = false;
    bool mSortRunning = false;

    int mOverwriteDialogResult = 0;

    void loadSettings();
    void saveSettings();
    void initializeGui();
    void saveGui();

private slots:
    void on_btnInputPath_clicked();
    void on_btnOutputPath_clicked();
    void on_btnSortStart_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
