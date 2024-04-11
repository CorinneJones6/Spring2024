#include "droplet.h"

Droplet::Droplet() {
    setPixmap((QPixmap("://images/water.gif")).scaled(SIZE_X, SIZE_Y));

    QTimer *timer_drop = new QTimer(this);

    connect(timer_drop, &QTimer::timeout, this, &Droplet::move_droplet );

    timer_drop->start(10);
}

/*
 * Move the droplets down the screen
 */
void Droplet::move_droplet() {

    // Get the current position of the droplet
    QPointF curr_pos = pos();

    // Move the droplet down by updating its y position
    setPos(curr_pos.x(), curr_pos.y() + 1);

    if (curr_pos.y() > 610 || doesCollide()) {
        delete this;
    }
}

/*
 * Generate random x location for spawning
 */
int Droplet::generate_X_position(){
    // Generate a random integer between 0 and 912
    return QRandomGenerator::global()->bounded(913);
}

/*
 * Return true/false if collides with any item
 */
bool Droplet::doesCollide() {
    return !collidingItems().isEmpty();
}


