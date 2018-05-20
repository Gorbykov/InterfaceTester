#include "framesettingswindow.h"
#include "ui_framesettingswindow.h"

FrameSettingsWindow::FrameSettingsWindow(QWidget *parent, FrameIn *currentFrameIn, FrameOut *currentFrameOut ):
    QDialog(parent),
    ui(new Ui::FrameSettingsWindow)
{
    ui->setupUi(this);
    connect(this, SIGNAL(setFrameIn(FrameIn*)),parent,SLOT(setFrameIn(FrameIn*)));
    connect(this, SIGNAL(setFrameOut(FrameOut*)),parent,SLOT(setFrameOut(FrameOut*)));
    if(currentFrameIn!= nullptr)
    {
        ui->lineEditFileInName->setText(currentFrameIn->getFileName());
        ui->lineEditFileOutName->setText(currentFrameOut->getFileName());
        ui->lineEditFrameInName->setText(currentFrameIn->getFrameName());
        ui->lineEditFrameOutName->setText(currentFrameOut->getFrameName());
        ui->lineEditSize->setText(QString::number(currentFrameOut->getSize()));
        ui->lineEditDelay->setText(QString::number(currentFrameOut->getDelay()));
        //        ui->lineEditFrameName->setText(currentFrame->getFrameName());
        //        ui->lineEditFileName->setText( currentFrame->getFileName());
        //        ui->lineEditSize->setText(QString::number(currentFrame->getSize()));
        //        ui->lineEditDelay->setText(QString::number(currentFrame->getDelay()));
    }
}

FrameSettingsWindow::~FrameSettingsWindow()
{
    delete ui;
}

void FrameSettingsWindow::on_saveButton_clicked()
{
    QString frameInName = ui->lineEditFrameInName->text();
    QString fileInName = ui->lineEditFileInName->text();
    QString frameOutName = ui->lineEditFrameOutName->text();
    QString fileOutName = ui->lineEditFileOutName->text();

    if(frameInName.isEmpty() && frameInName.isEmpty())
    {
        frameInName = "defaultIn";
        frameOutName = "defaultOut";
    }else if(frameInName.isEmpty())
    {
        frameInName = frameOutName +"In";
        frameOutName = frameOutName +"Out";
    } else if(frameOutName.isEmpty())
    {
        frameOutName = frameInName +"Out";
        frameInName = frameInName +"In";
    }

    if(fileInName.isEmpty())
    {
        fileInName = frameInName;
    }
    if(fileOutName.isEmpty())
    {
        fileOutName = frameOutName;
    }
    bool *ok = new bool(false);
    int size = ui->lineEditSize->text().toInt(ok);
    int delay = ui->lineEditDelay->text().toInt(ok);
    if (! *ok)
    {
        size = -1;
        delay = -1;
    }
    FrameIn *newFrameIn = new FrameIn(frameInName,fileInName);
    FrameOut *newFrameOut = new FrameOut(frameOutName,fileOutName,size,delay);
    emit setFrameIn(newFrameIn);
    emit setFrameOut(newFrameOut);
    emit close();
}
