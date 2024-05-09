#ifndef GAMESCENE_H
#define GAMESCENE_H
#include <QGraphicsScene>
#include "bucket.h"
#include "droplet.h"

class GameScene: public QGraphicsScene
{

    const int GAME_WIDTH = 910;
    const int GAME_HEIGHT = 512;

    void drawBucket();
    void drawDroplets();


public:
    GameScene();

    int get_width();

    int get_height();
};

#endif // GAMESCENE_H
