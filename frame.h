#ifndef FRAME_H
#define FRAME_H

#include <QString>
#include <QFile>
#include <QByteArray>

#define DEFAULT_IN "in.hex"
#define DEFAULT_OUT "out.hex"
#define DEFAULT_OUT_DELAYS "out_delays.txt"
#define DEFAULT_IN_DELAYS "in_delays.txt"

enum class Direction
  {
     in,
     out
  };

class Frame
{
public:
    Frame();    
    Frame(QString folderName);
    void setDataIn(QVector<QByteArray*> *dataIn);
    void setFullDataIn(QByteArray* dataIn);
    void setDelaysIn(QVector<int> *delaysIn);
    void setDataOut(QVector<QByteArray*> *dataOut);
    void setDelaysOut(QVector<int> *delaysOut);
    void setFolderName(QString folderName);
    void setSizesIn(QVector<int> *sizesIn);
    //void setFileName(QString fileName);
    //void setSize(int size);
    //void setDelay(int delay);
    QVector<QByteArray*>* getDataIn();
    QByteArray* getFullDataIn();
    QVector<int>* getDelaysIn();
    QVector<QByteArray*>* getDataOut();
    QVector<int>* getDelaysOut();
    QVector<int>* getSizesIn();
    QVector<int>* getSizesOut();
    QVector<int>* getFullSizesIn();
    QVector<int>* getFullDelaysIn();
    QString getFolderName();
    //int getSize();
    //int getDelay();
    //QString getFrameName();
    //QString getFileName();
    //bool loadFile(QString fileName);
    //bool saveFile(QString fileName);
    //bool saveFile();
    bool saveAll();
    bool saveAll(QString folderName);

    bool saveIn();
    bool saveIn(QString folderName);

    bool saveOut();
    bool saveOut(QString folderName);

    bool openAll();
    bool openAll(QString folderName);

    bool openIn();
    bool openIn(QString folderName);

    bool openOut();
    bool openOut(QString folderName);

private:
    bool _save(QString folderName, Direction direction);
    bool _open(QString folderName, Direction direction);

    QString _folderName;
    QVector<QByteArray*> *_dataIn = nullptr;
    QByteArray* _fullDataIn = nullptr;
    QVector<int> *_delaysIn = nullptr;
    QVector<int> *_fullDelaysIn = nullptr;
    QVector<int> *_fullSizesIn = nullptr;
    QVector<QByteArray*> *_dataOut = nullptr;
    QVector<int> *_delaysOut = nullptr;
};

#endif // FRAME_H
