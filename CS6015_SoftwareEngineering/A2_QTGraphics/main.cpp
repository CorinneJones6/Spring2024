#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>

#include "gamescene.h" //customized graphics scene

int main (int argc, char **argv) {

    QApplication app (argc, argv);

    int width = 910;
    int height = 650;

    //scene --> QGraphicScene
    //create our customized graphics scene

    GameScene *gameScene = new GameScene();

    //create a view --> QGraphicsView
    QGraphicsView *mainView = new QGraphicsView();

    //add the scene to the view --> set Scene
    mainView->setScene(gameScene);

    // Disable scroll bars
    mainView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mainView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //To fix the view window size and prevent it from ever growing or shrinking
    mainView->setFixedSize(width, height);

    //show the view -->show
    mainView->show();

    return app.exec();
}
