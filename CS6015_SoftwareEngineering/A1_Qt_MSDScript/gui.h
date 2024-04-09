#ifndef GUI_H
#define GUI_H

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
#include "Val.hpp"
#include "parse.hpp"


class GUI : public QWidget {

    QVBoxLayout* mainLayout;

    QRadioButton* interp_radioB;
    QRadioButton* pretty_print_radioB;
    QTextEdit *expr_textE;
    QTextEdit *result_textE;
    QPushButton *submit_pushB;
    QPushButton *reset_pushB;


    QWidget* setGridLayout();
    void connectBtns();
    void fillSummary();
    void clearWidgets();


public:

    GUI();

};

#endif // GUI_H
