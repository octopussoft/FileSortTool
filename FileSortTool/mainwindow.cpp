#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDir>
#include <QDirIterator>
#include <QFileInfo>
#include <QDateTime>
#include <QSettings>

#include "overwritedialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    /* load application settings */
    loadSettings();

    /* initialize gui */
    ui->setupUi(this);
    initializeGui();
    saveGui();
}

MainWindow::~MainWindow()
{
    saveSettings();

    delete ui;
}

void MainWindow::loadSettings()
{
    QSettings* settings = new QSettings(QApplication::organizationName(), QApplication::applicationName());

    mInputPath = settings->value("mInputPath", "").toString();
    mOutputPath = settings->value("mOutputPath", "").toString();
    mSortCriteriaModifyDate = settings->value("mSortCriteriaModifyDate", true).toBool();
    mSortCriteriaCreationDate = settings->value("mSortCriteriaCreationDate", false).toBool();
    mSortCriteriaRecursive = settings->value("mSortCriteriaRecursive", false).toBool();
    mSortCriteriaCopy = settings->value("mSortCriteriaCopy", false).toBool();
    mSortOptionYear = settings->value("mSortOptionYear", false).toBool();
    mSortOptionMonth = settings->value("mSortOptionMonth", false).toBool();
    mSortOptionDay = settings->value("mSortOptionDay", true).toBool();
}

void MainWindow::saveSettings()
{
    saveGui();

    QSettings* settings = new QSettings(QApplication::organizationName(), QApplication::applicationName());
    settings->setValue("mInputPath", mInputPath);
    settings->setValue("mOutputPath", mOutputPath);
    settings->setValue("mSortCriteriaModifyDate", mSortCriteriaModifyDate);
    settings->setValue("mSortCriteriaCreationDate", mSortCriteriaCreationDate);
    settings->setValue("mSortCriteriaRecursive", mSortCriteriaRecursive);
    settings->setValue("mSortCriteriaCopy", mSortCriteriaCopy);
    settings->setValue("mSortOptionYear", mSortOptionYear);
    settings->setValue("mSortOptionMonth", mSortOptionMonth);
    settings->setValue("mSortOptionDay", mSortOptionDay);
}

void MainWindow::initializeGui()
{
    ui->txtInputPath->setText(mInputPath);
    ui->txtOutputPath->setText(mOutputPath);
    ui->chOptionCopy->setChecked(mSortCriteriaCopy);
    ui->chOptionRecursive->setChecked(mSortCriteriaRecursive);
    ui->rbSortCriteriaCreationDate->setChecked(mSortCriteriaCreationDate);
    ui->rbSortCriteriaModifyDate->setChecked(mSortCriteriaModifyDate);
    ui->rbSortOptionDay->setChecked(mSortOptionDay);
    ui->rbSortOptionMonth->setChecked(mSortOptionMonth);
    ui->rbSortOptionYear->setChecked(mSortOptionYear);
}

void MainWindow::saveGui()
{
    mInputPath = ui->txtInputPath->text();
    mOutputPath = ui->txtOutputPath->text();
    mSortCriteriaModifyDate = ui->rbSortCriteriaModifyDate->isChecked();
    mSortCriteriaCreationDate = ui->rbSortCriteriaCreationDate->isChecked();
    mSortCriteriaRecursive = ui->chOptionRecursive->isChecked();
    mSortCriteriaCopy = ui->chOptionCopy->isChecked();
    mSortOptionYear = ui->rbSortOptionYear->isChecked();
    mSortOptionMonth = ui->rbSortOptionMonth->isChecked();
    mSortOptionDay = ui->rbSortOptionDay->isChecked();
}


void MainWindow::on_btnInputPath_clicked()
{
    mInputPath = QFileDialog::getExistingDirectory(this, tr("Select Input Folder"), mInputPath);
    if (QDir(mInputPath).exists() == true)  {
        ui->txtInputPath->setText(mInputPath);
    }
}

void MainWindow::on_btnOutputPath_clicked()
{
    mOutputPath = QFileDialog::getExistingDirectory(this, tr("Select Output Folder"), mOutputPath);
    if (QDir(mOutputPath).exists() == true)  {
        ui->txtOutputPath->setText(mOutputPath);
    }
}

void MainWindow::on_btnSortStart_clicked()
{
    /* sort running? */
    if (mSortRunning == false)  {

        /* sort is now running */
        mSortRunning = true;
        ui->btnSortStart->setText("Stop");
        mOverwriteDialogResult = OverwriteDialog::DIALOG_RESULT::RESULT_YES;

        ui->pbSortProgress->setMinimum(0);
        ui->pbSortProgress->setMaximum(100);
        ui->pbSortProgress->setValue(0);

        /* get options */
        saveGui();

        /* check if input dir is existing */
        QDir inputDir(mInputPath);
        if (inputDir.exists() != true)  {
            mSortRunning = false;
            ui->btnSortStart->setText("Start");
            return;
        }

        /* read the files */
        QDirIterator::IteratorFlag itFlag = QDirIterator::NoIteratorFlags;
        if (mSortCriteriaRecursive == true)  {
            itFlag = QDirIterator::Subdirectories;
        }
        QDirIterator it(mInputPath, QStringList() << "*", QDir::Files, itFlag);
        QStringList fileList;
        while (it.hasNext())  {
            fileList.append(it.next());
            QCoreApplication::processEvents();
        }

        /* create output dir if not existiong */
        QDir outputDir(mOutputPath);
        if (outputDir.exists() != true)  {
            outputDir.mkpath(mOutputPath);
        }

        /* check if output dir is existing */
        if (outputDir.exists() != true)  {
            mSortRunning = false;
            ui->btnSortStart->setText("Start");
            return;
        }
        outputDir = QDir(mOutputPath);
        bool success = false;

        /* sort files */
        for (int i = 0; i < fileList.count(); i++)  {

            /* keep gui responsive */
            QCoreApplication::processEvents();

            /* get file info */
            QFileInfo fileInfo(fileList[i]);

            /* get sort date */
            QDateTime sortTime;
            if (mSortCriteriaModifyDate == true)  {
                sortTime = fileInfo.lastModified();
            } else if (mSortCriteriaCreationDate == true)  {
                sortTime = fileInfo.birthTime();
            }

            /* compose folder stucture */
            QString newPath;
            QString tmpPathYear = outputDir.path() + "/" + QString::number(sortTime.date().year());
            QString tmpPathMonth = tmpPathYear + "/" +  QString::number(sortTime.date().month());
            QString tmpPathDay = tmpPathMonth + "/" +  QString::number(sortTime.date().day());

            if (mSortOptionDay == true)  {
                newPath = tmpPathDay;
            } else if (mSortOptionMonth == true)  {
                newPath = tmpPathMonth;
            } else if (mSortOptionYear == true)  {
                newPath = tmpPathYear;
            }

            /* create folder if not existing */
            QDir folderPath(newPath);
            if (folderPath.exists() == false)  {
                folderPath.mkpath(newPath);
            }

            /* compose final path */
            newPath += "/" + fileInfo.fileName();

            /* check if file exists */
            QFile newFile(newPath);

            /* show existing file handling dialog */
            if ((newFile.exists() == true) &&
                    ((mOverwriteDialogResult == OverwriteDialog::DIALOG_RESULT::RESULT_YES_TO_ALL) ||
                     (mOverwriteDialogResult == OverwriteDialog::DIALOG_RESULT::RESULT_NO_TO_ALL)) == false)  {

                OverwriteDialog* overwriteDialog = new OverwriteDialog();
                overwriteDialog->exec();

                /* save result */
                mOverwriteDialogResult = overwriteDialog->result();
            }

            /* skip file? */
            bool skipFile;
            switch (mOverwriteDialogResult)  {
            case OverwriteDialog::DIALOG_RESULT::RESULT_YES:
            case OverwriteDialog::DIALOG_RESULT::RESULT_YES_TO_ALL:
                skipFile = false;
                break;
            case OverwriteDialog::DIALOG_RESULT::RESULT_NO:
            case OverwriteDialog::DIALOG_RESULT::RESULT_NO_TO_ALL:
            default:
                skipFile = true;
                break;
            }

            /* keep gui responsive */
            QCoreApplication::processEvents();

            /* copy or move files */
            if (skipFile != true)  {
                if (mSortCriteriaCopy == true)  {
                    success = QFile::copy(fileList[i], newPath);
                } else {
                    success = QFile::rename(fileList[i], newPath);
                }

                if (success == false)  {
                    /* TODO: Fehlerbehandlung */
                }
            }

            /* keep gui responsive */
            QCoreApplication::processEvents();

            /* update progress bar */
            double progress = (double)i / (double)fileList.count() * 100;
            ui->pbSortProgress->setValue(qRound(progress));
        }
    }

    mSortRunning = false;
    ui->btnSortStart->setText("Start");
    ui->pbSortProgress->setValue(100);
}
