#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "secondscreen.h" // Include secondscreen.h
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QCryptographicHash>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Set fixed size for the main window
    setFixedSize(500, 600);

    // Create layout for registration form
    QVBoxLayout *registrationLayout = new QVBoxLayout;
    registrationLayout->setAlignment(Qt::AlignCenter);
    registrationLayout->setSpacing(20);
    registrationLayout->setContentsMargins(30, 30, 30, 30);

    // Add labels and line edits for username and password
    QLabel *usernameLabel = new QLabel("Username:");
    usernameLineEdit = new QLineEdit;
    usernameLineEdit->setStyleSheet("QLineEdit { margin: 0; padding: 10px; font-size: 14px; }");
    QLabel *passwordLabel = new QLabel("Password:");
    passwordLineEdit = new QLineEdit;
    passwordLineEdit->setEchoMode(QLineEdit::Password);
    passwordLineEdit->setStyleSheet("QLineEdit { margin: 0; padding: 10px; font-size: 14px; }");

    registrationLayout->addWidget(usernameLabel);
    registrationLayout->addWidget(usernameLineEdit);
    registrationLayout->addWidget(passwordLabel);
    registrationLayout->addWidget(passwordLineEdit);

    // Create layout for login and signup buttons
    QHBoxLayout *buttonsLayout = new QHBoxLayout;

    // Add login and signup buttons with styling
    QPushButton *loginButton = new QPushButton("Login");
    loginButton->setStyleSheet("QPushButton {"
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
    connect(loginButton, &QPushButton::clicked, this, &MainWindow::loginClicked);
    buttonsLayout->addWidget(loginButton);

    QPushButton *signupButton = new QPushButton("Signup");
    signupButton->setStyleSheet("QPushButton {"
                                "background-color: #008CBA;"
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
                                "background-color: #45a7d5;"
                                "}"
                                "QPushButton:pressed {"
                                "background-color: #005f75;"
                                "}");
    connect(signupButton, &QPushButton::clicked, this, &MainWindow::signupClicked);
    buttonsLayout->addWidget(signupButton);

    // Add the buttons layout to the main registration layout
    registrationLayout->addLayout(buttonsLayout);

    // Create widget to hold registration form
    QWidget *registrationWidget = new QWidget;
    registrationWidget->setLayout(registrationLayout);

    // Add registration form widget to main window
    setCentralWidget(registrationWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loginClicked()
{
    // Retrieve username and password from QLineEdit fields
    QString username = usernameLineEdit->text();
    QString password = passwordLineEdit->text();

    // Open database connection
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("D:/final pr/databaseee/data.db"); // Replace with your database file path

    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
        QMessageBox::critical(this, "Database Error", "Connection with database failed. Login failed.");
        return;
    }

    // Prepare SELECT statement
    QByteArray passwordHash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    QString hashedPasswordHex = QString(passwordHash.toHex());

    QSqlQuery query(db);
    query.prepare("SELECT * FROM users WHERE username = :username AND password = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", hashedPasswordHex);

    // Execute the query
    if (query.exec() && query.next()) {
        qDebug() << "Login successful!";
        loggedInUsername = username;
        QMessageBox::information(this, "Login", "Login successful!");

        // Close the database connection
        db.close();
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);

        // Open the "secondscreen" window
        secondscreen *secondScreen = new secondscreen(loggedInUsername);
        secondScreen->show();
        close(); // Close the current MainWindow
    } else {
        qDebug() << "Login failed:" << query.lastError().text();
        QMessageBox::critical(this, "Login Error", "Login failed: Username or password incorrect.");

        // Close the database connection
        db.close();
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
    }
}

void MainWindow::signupClicked()
{
    // Retrieve username and password from QLineEdit fields
    QString username = usernameLineEdit->text().trimmed();
    QString password = passwordLineEdit->text().trimmed();

    // Check if username or password is empty
    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::critical(this, "Signup Error", "Username and password cannot be empty.");
        return;
    }

    // Open database connection
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("D:/final pr/databaseee/data.db"); // Replace with your database file path

    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
        QMessageBox::critical(this, "Database Error", "Connection with database failed. Signup failed.");
        return;
    }

    // Check if username already exists
    QSqlQuery checkQuery(db);
    checkQuery.prepare("SELECT username FROM users WHERE username = :username");
    checkQuery.bindValue(":username", username);

    if (checkQuery.exec() && checkQuery.next()) {
        qDebug() << "Username already exists.";
        QMessageBox::critical(this, "Signup Error", "Username already exists. Please choose another name.");
        db.close();
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
        return;
    }

    // Prepare INSERT statement
    QByteArray passwordHash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    QString hashedPasswordHex = QString(passwordHash.toHex());

    QSqlQuery query(db);
    query.prepare("INSERT INTO users (username, password, wins, draws, losses) "
                  "VALUES (:username, :password, 0, 0, 0)");
    query.bindValue(":username", username);
    query.bindValue(":password", hashedPasswordHex);

    // Execute the query
    if (query.exec()) {
        qDebug() << "Signup successful!";
        loggedInUsername = username;
        QMessageBox::information(this, "Signup", "Signup successful!");

        // Close the database connection
        db.close();
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);

        // Open the "secondscreen" window
        secondscreen *secondScreen = new secondscreen(loggedInUsername);
        secondScreen->show();
        close(); // Close the current MainWindow
    } else {
        qDebug() << "Signup failed:" << query.lastError().text();
        QMessageBox::critical(this, "Signup Error", "Signup failed: " + query.lastError().text());

        // Close the database connection
        db.close();
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
    }
}
