#include "mainwidget.h"

mainWidget::mainWidget() {

    // Combine all vertical layouts into a main layout
    mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(setVerticalLayout1());
    mainLayout->addLayout(setVerticalLayout2());
    mainLayout->addLayout(setVerticalLayout3());

    connectBtns();

}

QVBoxLayout* mainWidget::setVerticalLayout1() {
    QVBoxLayout* verticalLayout1 = new QVBoxLayout();
    QGridLayout* gridLayout = setGridLayout(); // Initialize and populate the grid layout
    QWidget* gridWidget = new QWidget(); // Create a wrapper widget
    gridWidget->setLayout(gridLayout); // Set the grid layout to the wrapper widget
    verticalLayout1->addWidget(gridWidget); // Add the wrapper widget to the vertical layout

    return verticalLayout1;
}

QVBoxLayout* mainWidget::setVerticalLayout2() {
    QVBoxLayout* verticalLayout2 = new QVBoxLayout();
    mainTextEdit = new QTextEdit;
    verticalLayout2->addWidget(mainTextEdit);

    return verticalLayout2;
}

QVBoxLayout* mainWidget::setVerticalLayout3() {
    QVBoxLayout* verticalLayout3 = new QVBoxLayout();
    finishPushB = new QPushButton("Display Summary");
    verticalLayout3->addWidget(finishPushB);

    return verticalLayout3;
}

QGridLayout* mainWidget::setGridLayout() {
    QGridLayout *gridLayout = new QGridLayout;
    QLabel* firstNameLabel = new QLabel("First Name");
    QLabel* ageLabel = new QLabel("Age");
    QLabel* lastNameLabel = new QLabel("Last Name");
    QLabel* genderLabel = new QLabel("Gender");

    firstNameLineEdit = new QLineEdit;
    ageSpinBox = new QSpinBox;
    lastNameLineEdit = new QLineEdit;
    genderMaleRadioB = new QRadioButton("Male");
    genderFemaleRadioB = new QRadioButton("Female");
    genderNonBinaryRadioB = new QRadioButton("Nonbinary");
    refreshPushB = new QPushButton("Reset");

    // Row 0
    gridLayout->addWidget(firstNameLabel, 0, 0);
    gridLayout->addWidget(firstNameLineEdit, 0, 1);
    gridLayout->addWidget(ageLabel, 0, 2);
    gridLayout->addWidget(ageSpinBox, 0, 3);

    // Row 1
    gridLayout->addWidget(lastNameLabel, 1, 0);
    gridLayout->addWidget(lastNameLineEdit, 1, 1);
    gridLayout->addItem(new QSpacerItem(50, 10), 1, 2);
    gridLayout->addItem(new QSpacerItem(50, 10), 1, 3);

    // Row 2 - Gender Label and Radio Buttons
    gridLayout->addWidget(genderLabel, 2, 0);
    QVBoxLayout* genderLayout = new QVBoxLayout();
    genderLayout->addWidget(genderMaleRadioB);
    genderLayout->addWidget(genderFemaleRadioB);
    genderLayout->addWidget(genderNonBinaryRadioB);
    QWidget* genderWidget = new QWidget();
    genderWidget->setLayout(genderLayout);
    gridLayout->addWidget(genderWidget, 2, 1);

    // Row 3 - Refresh Button
    gridLayout->addWidget(refreshPushB, 3, 0);

    return gridLayout;
}

void mainWidget::connectBtns(){
    //Param 1: Sender
    //Param 2: Signal
    //Param 3: Where the connection is being made
    //Param 4: The function called
    connect(finishPushB, &QPushButton::clicked, this, &mainWidget::fillSummary);
    connect(refreshPushB, &QPushButton::clicked, this, &mainWidget::clearWidgets);
}

void mainWidget::fillSummary(){
    QString summary;
    QString gender = genderMaleRadioB->isChecked() ? "Male" : genderFemaleRadioB->isChecked() ? "Female" : genderNonBinaryRadioB->isChecked() ? "NonBinary" : "";

    summary += "First Name: " + firstNameLineEdit->text()  + "\n";
    summary += "Last Name: " + lastNameLineEdit->text() + "\n";
    summary += "Age: " + QString::number(ageSpinBox->value()) + "\n";
    summary += "Gender: " + gender + "\n";

    mainTextEdit->setText(summary);
}

void mainWidget::clearWidgets() {
    firstNameLineEdit->clear();
    lastNameLineEdit->clear();
    ageSpinBox->setValue(ageSpinBox->minimum());
    genderMaleRadioB->setChecked(false);
    genderFemaleRadioB->setChecked(false);
    mainTextEdit->clear();
}


