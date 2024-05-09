#include "login.h"

login::login(QWidget *parent) : QWidget(parent) {

    mainLayout = new QVBoxLayout(this);

    guestB = new QPushButton("Guest Login");
    signB = new QPushButton("Returning User");
    createB = new QPushButton("Create An Account");

    QVBoxLayout* verticalLayout = new QVBoxLayout();

    verticalLayout->addWidget(guestB);
    verticalLayout->addWidget(signB);
    verticalLayout->addWidget(createB);

    connect(guestB, &QPushButton::clicked, [this]() {
        emit guestLoggedIn();
    });

    verticalLayout->setContentsMargins(30, 30, 30, 30);

    mainLayout->addLayout(verticalLayout);

    resize(910, 650);

}
