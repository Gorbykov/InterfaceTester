#ifndef FRAMESETTINGSWINDOW_H
#define FRAMESETTINGSWINDOW_H

#include <QDialog>

#include "frameIn.h"
#include "frameOut.h"

namespace Ui {
class FrameSettingsWindow;
}

class FrameSettingsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit FrameSettingsWindow(QWidget *parent = 0, FrameIn *currentFrameIn = nullptr, FrameOut *currentFrameOut = nullptr);
    ~FrameSettingsWindow();

private slots:
    void on_saveButton_clicked();

signals:
    void setFrameIn(FrameIn *newFrameIn);
    void setFrameOut(FrameOut *newFrameOut);

private:
    Ui::FrameSettingsWindow *ui;
};

#endif // FRAMESETTINGSWINDOW_H
