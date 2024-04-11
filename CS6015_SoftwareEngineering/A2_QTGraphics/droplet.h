#ifndef DROPLET_H
#define DROPLET_H

#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QTimer>
#include <QRandomGenerator>
#include "bucket.h"

class Droplet: public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

    const int SIZE_X  = 25;
    const int SIZE_Y = 40;

    void move_droplet();
    bool doesCollide();


public:
    Droplet();
    int generate_X_position();

};

#endif // DROPLET_H
