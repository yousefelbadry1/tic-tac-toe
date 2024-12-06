#include "secondscreen.h"
#include "ui_secondscreen.h"
#include "gameframe.h"
#include "gameframeai.h"
#include "gamehistory.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QDebug>

secondscreen::secondscreen(const QString &loggedInUsername, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::secondscreen),
    loggedInUsername(loggedInUsername) // Store loggedInUsername in the member variable
{
    setFixedSize(500, 600);
    ui->setupUi(this);

    // Create layout for buttons
    QVBoxLayout *buttonLayout = new QVBoxLayout;

    // Add buttons to the layout
    QPushButton *button1 = new QPushButton("1 Player");
    setButtonStyle(button1);
    buttonLayout->addWidget(button1);
    connect(button1, &QPushButton::clicked, this, &secondscreen::onButton1Clicked);

    QPushButton *button2 = new QPushButton("2 Players");
    setButtonStyle(button2);
    buttonLayout->addWidget(button2);
    connect(button2, &QPushButton::clicked, this, &secondscreen::onButton2Clicked);

    QPushButton *button3 = new QPushButton("Score History");
    setButtonStyle(button3);
    buttonLayout->addWidget(button3);
    connect(button3, &QPushButton::clicked, this, &secondscreen::onButton3Clicked);

    // Set the layout for the central widget
    QWidget *centralWidget = new QWidget;
    centralWidget->setLayout(buttonLayout);
    setCentralWidget(centralWidget);
}

secondscreen::~secondscreen()
{
    delete ui;
}

void secondscreen::setButtonStyle(QPushButton *button)
{
    // Set custom style for the button
    button->setStyleSheet("QPushButton {"
                          "background-color: #4CAF50;"
                          "border: none;"
                          "color: white;"
                          "padding: 10px 24px;"
                          "text-align: center;"
                          "text-decoration: none;"
                          "display: inline-block;"
                          "font-size: 16px;"
                          "margin: 4px 2px;"
                          "cursor: pointer;"
                          "border-radius: 8px;"
                          "}"
                          "QPushButton:hover {"
                          "background-color: #45a049;"
                          "}"
                          "QPushButton:pressed {"
                          "background-color: #367533;"
                          "}");
}

void secondscreen::onButton1Clicked()
{
    // Create and show the gameframeai window when button1 is clicked
    qDebug() << "1 Player button clicked!";
    gameframeai *gameFrameWindow = new gameframeai(loggedInUsername); // Pass loggedInUsername
    gameFrameWindow->show();
    close(); // Close the current secondscreen window
}

void secondscreen::onButton2Clicked()
{
    // Create and show the gameframe window when button2 is clicked
    qDebug() << "2 Players button clicked!";
    gameframe *gameFrameWindow = new gameframe(loggedInUsername); // Pass loggedInUsername
    gameFrameWindow->show();
    close(); // Close the current secondscreen window
}

void secondscreen::onButton3Clicked()
{
    qDebug() << "Score History button clicked!";
    gamehistory *historyWindow = new gamehistory(loggedInUsername); // Pass loggedInUsername
    historyWindow->show();
    close(); // Close the current secondscreen window
}
