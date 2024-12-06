#include "gamehistory.h"
#include "ui_gamehistory.h"
#include "secondscreen.h"
#include <QSpacerItem>
#include <QLabel>
#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QFont> // Include QFont for styling

gamehistory::gamehistory(const QString &loggedInUsername, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::gamehistory)
    , loggedInUsername(loggedInUsername)
    , mainLayout(new QVBoxLayout)
    , backButton(new QPushButton("Back"))
    , tableView(new QTableView)
    , model(new QSqlTableModel(this))
{
    ui->setupUi(this);

    // Connect to SQLite database
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("D:/final pr/databaseee/data.db"); // Replace with your database file path

    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
        QMessageBox::critical(this, "Database Error", "Connection with database failed.");
        return;
    }

    // Set fixed size for the window
    setFixedSize(600, 500); // Adjusted size for better table visibility

    // Set background color and text color
    setStyleSheet("background-color: #f0f0f0; color: #333;");

    // Create welcome message label
    QLabel *welcomeLabel = new QLabel("Welcome " + loggedInUsername);
    welcomeLabel->setFont(QFont("Arial", 20, QFont::Bold));
    welcomeLabel->setAlignment(Qt::AlignCenter);

    // Configure back button
    QFont buttonFont("Arial", 14);
    backButton->setFont(buttonFont);
    backButton->setStyleSheet("QPushButton {"
                              "background-color: #4CAF50;"
                              "border: none;"
                              "color: white;"
                              "padding: 10px 24px;"
                              "text-align: center;"
                              "text-decoration: none;"
                              "display: inline-block;"
                              "font-size: 14px;"
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

    connect(backButton, &QPushButton::clicked, this, &gamehistory::goBackToSecondScreen);

    // Set up the table view
    updateModel(); // Initial update of the model with logged-in user's data

    tableView->setModel(model);
    tableView->horizontalHeader()->setStretchLastSection(true); // Stretch last column to fill space
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // Stretch columns to fill available space

    // Hide row headers (numbers on the left)
    tableView->verticalHeader()->setVisible(false);

    // Set font and size for table headers
    QFont headerFont("Arial", 14, QFont::Bold);
    tableView->horizontalHeader()->setFont(headerFont);

    // Set font and size for table cells
    QFont cellFont("Arial", 12);
    tableView->setFont(cellFont);

    // Set header labels
    model->setHeaderData(0, Qt::Horizontal, "Wins");
    model->setHeaderData(1, Qt::Horizontal, "Draws");
    model->setHeaderData(2, Qt::Horizontal, "Losses");

    // Set alternating row colors
    tableView->setAlternatingRowColors(true);
    tableView->setStyleSheet("QTableView { background-color: #fff; border: 1px solid #ccc; }");

    // Make the table view read-only
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Add widgets to the main layout
    mainLayout->addWidget(welcomeLabel);
    mainLayout->addWidget(tableView);
    mainLayout->addWidget(backButton, 0, Qt::AlignCenter);
    mainLayout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));

    // Set margins and spacing for the main layout
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    // Set the central widget layout
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);
}

gamehistory::~gamehistory()
{
    delete ui;
}

void gamehistory::goBackToSecondScreen()
{
    // Close the current gamehistory window
    this->close();

    // Optionally, you can open a new instance of secondscreen if needed
    secondscreen *secondScreen = new secondscreen(loggedInUsername);
    secondScreen->show();
}

void gamehistory::updateModel()
{
    // Custom SQL query to fetch data based on loggedInUsername
    QString queryStr = QString("SELECT wins, draws, losses FROM users WHERE username = :username");
    QSqlQuery query(db);
    query.prepare(queryStr);
    query.bindValue(":username", loggedInUsername);

    if (!query.exec()) {
        qDebug() << "Error executing query:" << query.lastError().text();
    } else {
        model->setQuery(query);
        // Set header labels (only needed once)
        model->setHeaderData(0, Qt::Horizontal, "Wins");
        model->setHeaderData(1, Qt::Horizontal, "Draws");
        model->setHeaderData(2, Qt::Horizontal, "Losses");
    }
}
