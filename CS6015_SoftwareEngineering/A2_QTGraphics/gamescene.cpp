#include "gamescene.h"

GameScene::GameScene() {

    // Add background
    setBackgroundBrush(QPixmap("://images/background.jpg").scaled(910, 650));
    // Image fit the window correctly
    setSceneRect(0,0,910,650);

    drawBucket();

    //Handle spawning the droplets
    QTimer *timer_spawn = new QTimer(this);
    connect(timer_spawn, &QTimer::timeout, this, &GameScene::drawDroplets);
    timer_spawn->start(QRandomGenerator::global()->bounded(4000));

}

/*
 * Return MV
 */
int GameScene::get_height(){
    return GAME_HEIGHT;
}

/*
 * Return MV
 */
int GameScene::get_width(){
    return GAME_WIDTH;
}

/*
 * Handle the drawing of the bucket + set focus
 */
void GameScene::drawBucket(){
    Bucket *bucket = new Bucket();

    //To allow the bucket to respond to a key event
    // a) make the object focusable
    bucket->setFlag(QGraphicsItem::ItemIsFocusable);
    // b) let the item be the focus item
    bucket->setFocus();

    addItem(bucket);

    // Draw bucket to the btm center of screen
    bucket->setPos(bucket->getInitialPosX(), bucket->getInitialPosY());
}

/*
 * Draw droplets with a random X pos
 */
void GameScene::drawDroplets() {
    Droplet *droplet = new Droplet();
    int pos = droplet->generate_X_position();

    droplet->setPos(pos, 0);

    addItem(droplet);
}

