#include "framesettingswindow.h"
#include "ui_framesettingswindow.h"

FrameSettingsWindow::FrameSettingsWindow(QWidget *parent, Frame *currentFrame) :
    QDialog(parent),
    ui(new Ui::FrameSettingsWindow)
{
    ui->setupUi(this);
    connect(this, SIGNAL(setFrame(Frame*)),parent,SLOT(setFrameOut(Frame*)));
    if(currentFrame!=NULL)
    {
        ui->lineEditFrameName->setText(currentFrame->getFrameName());
        ui->lineEditFileName->setText( currentFrame->getFileName());
        ui->lineEditSize->setText(QString::number(currentFrame->getSize()));
        ui->lineEditDelay->setText(QString::number(currentFrame->getDelay()));
    }
}

FrameSettingsWindow::~FrameSettingsWindow()
{
    delete ui;
}

void FrameSettingsWindow::on_saveButton_clicked()
{
    QString frameName = ui->lineEditFrameName->text();
    QString fileName = ui->lineEditFileName->text();
    if(fileName.isEmpty())
    {
        fileName = frameName;
    }
    bool *ok = new bool(false);
    int size = ui->lineEditSize->text().toInt(ok);
    int delay = ui->lineEditDelay->text().toInt(ok);
    if (! *ok)
    {
        size = -1;
        delay = -1;
    }
    Frame *newFrame = new Frame(frameName,fileName,size,delay);
    emit setFrame(newFrame);
    emit close();
}
