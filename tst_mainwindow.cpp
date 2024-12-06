#include <QtTest/QtTest>
#include <QSignalSpy>
#include "mainwindow.h"
#include <QPushButton>
#include <QMessageBox>

class TestMainWindow : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();     // Called before the first test function
    void cleanupTestCase();  // Called after the last test function
    void init();             // Called before each test function
    void cleanup();          // Called after each test function

    // Test cases
    void testLoginSuccess();
    void testLoginFailure();
    void testSignupSuccess();
    void testSignupFailure();

private:
    MainWindow *mainWindow;
};

void TestMainWindow::initTestCase()
{
    // Initialize resources or setup database connections needed for tests
    mainWindow = new MainWindow();
}

void TestMainWindow::cleanupTestCase()
{
    // Cleanup resources or close database connections after all tests are done
    delete mainWindow;
}

void TestMainWindow::init()
{
    // Setup before each test function
}

void TestMainWindow::cleanup()
{
    // Cleanup after each test function
}

void TestMainWindow::testLoginSuccess()
{
    // Simulate user input
    QTest::keyClicks(mainWindow->getUsernameLineEdit(), "correctUsername");
    QTest::keyClicks(mainWindow->getPasswordLineEdit(), "correctPassword");

    // Connect to loginClicked() slot
    QSignalSpy spy(mainWindow, &MainWindow::loginClicked);

    // Trigger login button click
    QPushButton *loginButton = mainWindow->findChild<QPushButton*>("loginButton");
    QVERIFY(loginButton != nullptr);
    QTest::mouseClick(loginButton, Qt::LeftButton);

    // Check if loginClicked() was emitted
    QVERIFY(spy.wait());
    QCOMPARE(mainWindow->getLoggedInUsername(), QString("correctUsername"));
    // Add further verifications if needed
}

void TestMainWindow::testLoginFailure()
{
    // Simulate user input
    QTest::keyClicks(mainWindow->getUsernameLineEdit(), "wrongUsername");
    QTest::keyClicks(mainWindow->getPasswordLineEdit(), "wrongPassword");

    // Connect to loginClicked() slot
    QSignalSpy spy(mainWindow, &MainWindow::loginClicked);

    // Trigger login button click
    QPushButton *loginButton = mainWindow->findChild<QPushButton*>("loginButton");
    QVERIFY(loginButton != nullptr);
    QTest::mouseClick(loginButton, Qt::LeftButton);

    // Check if loginClicked() was emitted
    QVERIFY(spy.wait());
    QVERIFY(mainWindow->getLoggedInUsername().isEmpty());
    // Add further verifications if needed
}

void TestMainWindow::testSignupSuccess()
{
    // Simulate user input
    QTest::keyClicks(mainWindow->getUsernameLineEdit(), "newUser");
    QTest::keyClicks(mainWindow->getPasswordLineEdit(), "newPassword");

    // Connect to signupClicked() slot
    QSignalSpy spy(mainWindow, &MainWindow::signupClicked);

    // Trigger signup button click
    QPushButton *signupButton = mainWindow->findChild<QPushButton*>("signupButton");
    QVERIFY(signupButton != nullptr);
    QTest::mouseClick(signupButton, Qt::LeftButton);

    // Check if signupClicked() was emitted
    QVERIFY(spy.wait());
    QCOMPARE(mainWindow->getLoggedInUsername(), QString("newUser"));
    // Add further verifications if needed
}

void TestMainWindow::testSignupFailure()
{
    // Simulate user input
    QTest::keyClicks(mainWindow->getUsernameLineEdit(), "existingUser");
    QTest::keyClicks(mainWindow->getPasswordLineEdit(), "password");

    // Connect to signupClicked() slot
    QSignalSpy spy(mainWindow, &MainWindow::signupClicked);

    // Trigger signup button click
    QPushButton *signupButton = mainWindow->findChild<QPushButton*>("signupButton");
    QVERIFY(signupButton != nullptr);
    QTest::mouseClick(signupButton, Qt::LeftButton);

    // Check if signupClicked() was emitted
    QVERIFY(spy.wait());
    QVERIFY(mainWindow->getLoggedInUsername().isEmpty());
    // Add further verifications if needed
}


