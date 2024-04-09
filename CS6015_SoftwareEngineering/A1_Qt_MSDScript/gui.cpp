#include "gui.h"

GUI::GUI() {

    mainLayout = new QVBoxLayout(this);

     // Add the grid layout, which contains most of the GUI elements, to the main layout
    mainLayout->addWidget(setGridLayout());

    // Connect the buttons to their respective slot functions
    connectBtns();

}

// Creates the grid layout containing all the input fields, labels, and buttons
QWidget* GUI::setGridLayout() {
    QWidget* gridWidget = new QWidget(); // The widget that will contain the grid layout
    QGridLayout *gridLayout = new QGridLayout; // The grid layout itself

    // Create labels, radio buttons, text edits, and push buttons
    QLabel* expr_label = new QLabel("Expression:");
    QLabel* res_label =  new QLabel("Result:");
    interp_radioB = new QRadioButton("Interp");
    pretty_print_radioB = new QRadioButton("Pretty Print");
    expr_textE = new QTextEdit;
    result_textE = new QTextEdit;
    submit_pushB = new QPushButton("Submit");
    reset_pushB = new QPushButton("Reset");

    // Row 0
    gridLayout->addWidget(expr_label, 0, 0);
    gridLayout->addWidget(expr_textE, 0, 1);

    // Row 1
    gridLayout->addItem(new QSpacerItem(50, 10), 1, 0);
    QVBoxLayout* option_layout = new QVBoxLayout();
    option_layout->addWidget(interp_radioB);
    option_layout->addWidget(pretty_print_radioB);
    QWidget* option_widget = new QWidget();
    option_widget->setStyleSheet("background-color: lightgrey;");
    option_widget->setLayout(option_layout);
    gridLayout->addWidget(option_widget, 1, 1);

    //Row 2
    gridLayout->addItem(new QSpacerItem(50, 10), 2, 0);
    gridLayout->addWidget(submit_pushB);
    submit_pushB->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    //Row 3
    gridLayout->addWidget(res_label, 3, 0);
    gridLayout->addWidget(result_textE, 3, 1);

    //Row 4
    gridLayout->addItem(new QSpacerItem(50, 10), 4, 0);
    gridLayout->addWidget(reset_pushB);
    reset_pushB->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    gridWidget->setLayout(gridLayout);

    return gridWidget;
}

// Connects signals from buttons to the corresponding slots in this GUI instance
void GUI::connectBtns(){
    // Param 1: Sender
    // Param 2: Signal
    // Param 3: Where the connection is being made
    // Param 4: The function called
    connect(submit_pushB, &QPushButton::clicked, this, &GUI::fillSummary);
    connect(reset_pushB, &QPushButton::clicked, this, &GUI::clearWidgets);
}

// Slot to fill the summary based on the inputs and selected options
void GUI::fillSummary() {

    QString input = expr_textE->toPlainText();

    // Do nothing if input is empty
    if(input.trimmed().isEmpty()) {
        return;
    }

    // Processing the input based on the selected radio button
    string parse_string = input.toUtf8().constData();
    QString output;

    if(interp_radioB->isChecked()){
        // Interpreting the expression
        PTR(Val) s = parse_str(parse_string)->interp(Env::empty);
        output = QString::fromStdString(s->to_string());
    }
    else if(pretty_print_radioB->isChecked()){
        // Pretty print the expression
        string final = parse_str(parse_string)->to_pretty_string();
        output = QString::fromStdString(final);

    }

    // Displaying the output
    result_textE->setText(output);
}

// Slot to clear all input widgets
void GUI::clearWidgets(){
    expr_textE->clear();
    result_textE->clear();
}
