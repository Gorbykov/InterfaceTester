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
    }
}

FrameSettingsWindow::~FrameSettingsWindow()
{
    delete ui;
}

void FrameSettingsWindow::on_saveButton_clicked()
{
    QString folderName = ui->lineEditFolderName->text();
    Frame *newFrame = new Frame(folderName);
    emit setFrame(newFrame);
    emit refreshFrameOut();
    emit close();
}

void FrameSettingsWindow::on_pushButtonFolderDialog_clicked()
{
  ui->lineEditFolderName->setText(QFileDialog::getExistingDirectory(this,"Выберите папку с входными/выходными данными",QDir::currentPath()));
}
