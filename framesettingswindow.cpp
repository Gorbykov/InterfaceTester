#include "framesettingswindow.h"
#include "ui_framesettingswindow.h"
#include <QFileDialog>

FrameSettingsWindow::FrameSettingsWindow(QWidget *parent, Frame *currentFrame):
    QDialog(parent),
    ui(new Ui::FrameSettingsWindow)
{
    ui->setupUi(this);
    connect(this, SIGNAL(setFrame(Frame*)),parent,SLOT(setFrame(Frame*)));
    connect(this, SIGNAL(refreshFrameIn()),parent,SLOT(refreshFrameIn()));
    QRegExp filePathRegex ("^([a-zA-Z]):[\\\/]((?:[^<>:\"\\\/\|\?\*]+[\\\/])*)([^<>:\"\\\/\|\?\*]+)$");
    ui->lineEditFolderName->setValidator(new QRegExpValidator(filePathRegex));
    if(currentFrame!= nullptr)
    {
        ui->lineEditFolderName->setText(currentFrame->getFolderName());
        //        ui->lineEditFileInName->setText(currentFrameIn->getFileName());
        //        ui->lineEditFileOutName->setText(currentFrameOut->getFileName());
        //        ui->lineEditFrameInName->setText(currentFrameIn->getFrameName());
        //        ui->lineEditFrameOutName->setText(currentFrameOut->getFrameName());
        //        ui->lineEditSize->setText(QString::number(currentFrameOut->getSize()));
        //        ui->lineEditDelay->setText(QString::number(currentFrameOut->getDelay()));
        //        //        ui->lineEditFrameName->setText(currentFrame->getFrameName());
        //        //        ui->lineEditFileName->setText( currentFrame->getFileName());
        //        //        ui->lineEditSize->setText(QString::number(currentFrame->getSize()));
        //        //        ui->lineEditDelay->setText(QString::number(currentFrame->getDelay()));
    }
}

FrameSettingsWindow::~FrameSettingsWindow()
{
    delete ui;
}

void FrameSettingsWindow::on_saveButton_clicked()
{
    QString folderName = ui->lineEditFolderName->text();
    //    QString fileInName = ui->lineEditFileInName->text();
    //    QString frameOutName = ui->lineEditFrameOutName->text();
    //    QString fileOutName = ui->lineEditFileOutName->text();

    //    if(frameInName.isEmpty() && frameInName.isEmpty())
    //    {
    //        frameInName = "defaultIn";
    //        frameOutName = "defaultOut";
    //    }else if(frameInName.isEmpty())
    //    {
    //        frameInName = frameOutName +"In";
    //        frameOutName = frameOutName +"Out";
    //    } else if(frameOutName.isEmpty())
    //    {
    //        frameOutName = frameInName +"Out";
    //        frameInName = frameInName +"In";
    //    }

    //    if(fileInName.isEmpty())
    //    {
    //        fileInName = frameInName;
    //    }
    //    if(fileOutName.isEmpty())
    //    {
    //        fileOutName = frameOutName;
    //    }
    //    bool *ok = new bool(false);
    //    int size = ui->lineEditSize->text().toInt(ok);
    //    int delay = ui->lineEditDelay->text().toInt(ok);
    //    if (! *ok)
    //    {
    //        size = -1;
    //        delay = -1;
    //    }
    Frame *newFrame = new Frame(folderName);
    emit setFrame(newFrame);
    emit refreshFrameOut();
    emit close();
}

void FrameSettingsWindow::on_pushButtonFolderDialog_clicked()
{
  ui->lineEditFolderName->setText(QFileDialog::getExistingDirectory(this,"Выберите папку с входными/выходными данными",QDir::currentPath()));
}
