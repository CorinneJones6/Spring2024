#include "bucket.h"

Bucket::Bucket() {

    setPixmap(QPixmap("://images/bucket.png").scaled(SIZE_X, SIZE_Y));

}

/*
 * Get member variable
 */
int Bucket::getInitialPosX(){
    return INITIAL_POS_X;
}

/*
 * Get member variable
 */
int Bucket::getInitialPosY(){
    return INITIAL_POS_Y;
}

/*
 * Handle the bucket going left and right
 */
void Bucket::keyPressEvent(QKeyEvent *event){

    // Move the bucket left ( -10 )
    if (event->key() == Qt::Key_Left) {

        //Get the BUCKET to the L then stop
        if(this->pos().x() < LEFT_BARRIER){
            return;
        }
        else {
        setPos(x() - 15, y());
        }
    }
    // Move the bucket right ( +10 )
    else if (event->key() == Qt::Key_Right) {

        //Get the BUCKET to the R then stop
        if(this->pos().x() > RIGHT_BARRIER){
            return;
        }
        else{
        setPos(x() + 15, y());
        }
    }
}

