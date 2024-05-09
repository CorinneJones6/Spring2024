#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>

#include "gamescene.h" //customized graphics scene
#include "login.h"

int main(int argc, char **argv) {
    QApplication app(argc, argv);

    login l;  // Create an instance of the login window

    // Connect the guestLoggedIn signal to a lambda that sets up and shows the main graphics view
    QObject::connect(&l, &login::guestLoggedIn, [&]() {
        int width = 910;
        int height = 650;

        // Create the custom game scene
        GameScene *gameScene = new GameScene();

        // Create the view to display the scene
        QGraphicsView *mainView = new QGraphicsView();
        mainView->setScene(gameScene);

        // Configure the view's properties
        mainView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        mainView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        mainView->setFixedSize(width, height);

        // Show the view
        mainView->show();
    });

    // Show the login window
    l.show();

    // Start the Qt event loop
    return app.exec();
}
