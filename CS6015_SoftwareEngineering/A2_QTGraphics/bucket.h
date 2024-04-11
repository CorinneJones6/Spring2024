#ifndef BUCKET_H
#define BUCKET_H

#include <QGraphicsPixmapItem>
#include <QKeyEvent>

class Bucket: public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

    const int INITIAL_POS_X = 400;
    const int INITIAL_POS_Y = 460;

    const int SIZE_X  = 200;
    const int SIZE_Y = 200;

    const int LEFT_BARRIER = -60;
    const int RIGHT_BARRIER = 740;

    void keyPressEvent(QKeyEvent *event);

public:
    Bucket();

    int getInitialPosX();
    int getInitialPosY();
};

#endif // BUCKET_H
