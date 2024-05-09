#ifndef LOGIN_H
#define LOGIN_H

#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class login : public QWidget {
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);

signals:
    void guestLoggedIn();  // Signal to indicate guest login

private:
    QVBoxLayout* mainLayout;
    QPushButton* guestB;
    QPushButton* signB;
    QPushButton* createB;
};

#endif // LOGIN_H
