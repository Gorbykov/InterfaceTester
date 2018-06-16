#ifndef FRAMESETTINGSWINDOW_H
#define FRAMESETTINGSWINDOW_H

#include <QDialog>
#include "frame.h"

namespace Ui {
class FrameSettingsWindow;
}

class FrameSettingsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit FrameSettingsWindow(QWidget *parent = 0, Frame *currentFrame = nullptr);
    ~FrameSettingsWindow();

private slots:
    void on_saveButton_clicked();

    void on_pushButtonFolderDialog_clicked();

signals:
    void setFrame(Frame *newFrame);
    void refreshFrameOut();

private:
    Ui::FrameSettingsWindow *ui;
};

#endif // FRAMESETTINGSWINDOW_H
