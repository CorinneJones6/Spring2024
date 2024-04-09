#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QRadioButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QGroupBox>

class mainWidget : public QWidget {

    QVBoxLayout* mainLayout;

    QLineEdit *firstNameLineEdit;
    QLineEdit *lastNameLineEdit;
    QSpinBox *ageSpinBox;
    QRadioButton *genderMaleRadioB;
    QRadioButton *genderFemaleRadioB;
    QRadioButton *genderNonBinaryRadioB;
    QTextEdit *mainTextEdit;
    QPushButton *refreshPushB;
    QPushButton *finishPushB;


    QVBoxLayout* setVerticalLayout1();
    QVBoxLayout* setVerticalLayout2();
    QVBoxLayout* setVerticalLayout3();
    QGridLayout* setGridLayout();


    void connectBtns();
    void fillSummary();
    void clearWidgets();


public:

    mainWidget();

};

#endif // MAINWIDGET_H
