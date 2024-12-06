#include "gameframe.h"
#include "ui_gameframe.h"
#include "secondscreen.h" // Include secondscreen header
#include <QPushButton>
#include <QGridLayout>
#include <QMessageBox>
#include <QFont>
#include <QElapsedTimer>
#include <QThread>
#include <QDebug>

gameframe::gameframe(const QString &loggedInUsername, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::gameframe)
    , currentPlayer('X')
    , xWins(0)
    , oWins(0)
    , draws(0)
    ,loggedInUsername(loggedInUsername) // Initialize the member variable
{
    ui->setupUi(this);
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("D:/final pr/databaseee/data.db"); // Replace with your database file path

    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
        QMessageBox::critical(this, "Database Error", "Connection with database failedd. Signup failed.");
        return;}
    setFixedSize(500, 600);

    gridLayout = new QGridLayout(ui->centralwidget);

    // Create buttons for each cell of the Tic Tac Toe board
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            QPushButton *cellButton = new QPushButton;
            cellButton->setFixedSize(100, 100);
            cellButton->setProperty("row", row);
            cellButton->setProperty("col", col);

            QFont font = cellButton->font();
            font.setPointSize(36);
            cellButton->setFont(font);

            connect(cellButton, &QPushButton::clicked, this, &gameframe::cellClicked);

            gridLayout->addWidget(cellButton, row, col);
            cellButtons[row][col] = cellButton;
        }
    }

    backButton = new QPushButton("Back");
    QFont backFont = backButton->font();
    backFont.setPointSize(16);
    backButton->setFont(backFont);
    connect(backButton, &QPushButton::clicked, this, &gameframe::goBack);
    gridLayout->addWidget(backButton, 3, 0, 1, 3);

    restartButton = new QPushButton("Restart");
    QFont restartFont = restartButton->font();
    restartFont.setPointSize(16);
    restartButton->setFont(restartFont);
    connect(restartButton, &QPushButton::clicked, this, &gameframe::resetGame);
    gridLayout->addWidget(restartButton, 4, 0, 1, 3);

    // Instantiate the secondscreen window
    secondScreen = new secondscreen(loggedInUsername);

    // Initialize and display the scores in the status bar
    updateScores();
}

gameframe::~gameframe()
{
    delete ui;
}

void gameframe::cellClicked()
{
    QElapsedTimer timer;
    timer.start();

    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    if (!clickedButton)
        return;

    int row = clickedButton->property("row").toInt();
    int col = clickedButton->property("col").toInt();

    if (!clickedButton->text().isEmpty())
    {
        QMessageBox::warning(this, "Invalid Move", "This cell is already occupied.");
        return;
    }

    // Simulate additional workload to make elapsed time measurable
    QThread::msleep(50);

    clickedButton->setText(QString(currentPlayer));

    if (checkWin(row, col))
    {
        QMessageBox::information(this, "Winner", QString("Player %1 wins!").arg(currentPlayer));
        if (currentPlayer == 'X')
            xWins++;
        else
            oWins++;
        updateScores();
        resetGame();
    }
    else if (checkDraw())
    {
        QMessageBox::information(this, "Draw", "It's a draw!");
        draws++;
        updateScores();
        resetGame();
    }
    else
    {
        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
    }

    qint64 elapsed = timer.elapsed();
    qDebug() << "Elapsed time for the button clicked = " << elapsed << "ms";
}

bool gameframe::checkWin(int row, int col)
{
    QString symbol = QString(currentPlayer);

    if (cellButtons[row][0]->text() == symbol &&
        cellButtons[row][1]->text() == symbol &&
        cellButtons[row][2]->text() == symbol)
        return true;

    if (cellButtons[0][col]->text() == symbol &&
        cellButtons[1][col]->text() == symbol &&
        cellButtons[2][col]->text() == symbol)
        return true;

    if ((cellButtons[0][0]->text() == symbol &&
         cellButtons[1][1]->text() == symbol &&
         cellButtons[2][2]->text() == symbol) ||
        (cellButtons[0][2]->text() == symbol &&
         cellButtons[1][1]->text() == symbol &&
         cellButtons[2][0]->text() == symbol))
        return true;

    return false;
}

bool gameframe::checkDraw()
{
    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            if (cellButtons[row][col]->text().isEmpty())
                return false;
        }
    }
    return true;
}

void gameframe::resetGame()
{
    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            cellButtons[row][col]->setText("");
        }
    }
}

void gameframe::goBack()
{
    // Show the secondscreen window and hide this window
    secondScreen->show();
    updateDatabase();
    this->hide();
}

void gameframe::updateScores()
{
    // Update the score display in the status bar
    QString xScoreStr = "Player X Wins: " + QString::number(xWins);
    QString oScoreStr = "Player O Wins: " + QString::number(oWins);
    QString drawsStr = "Draws: " + QString::number(draws);
    ui->statusbar->showMessage(xScoreStr + " | " + oScoreStr + " | " + drawsStr);
}
void gameframe::updateDatabase() {
    db.open();
    QSqlDatabase::database().transaction();

    // Retrieve current values from the database for username 'oza'
    QSqlQuery querySelect(db);
    querySelect.prepare("SELECT wins, draws, losses FROM users WHERE username = :oza");
    querySelect.bindValue(":oza", loggedInUsername);
    if (!querySelect.exec() || !querySelect.next()) {
        qDebug() << "Error selecting current values from database:" << querySelect.lastError().text();
        QSqlDatabase::database().rollback();
        db.close();
        return;
    }

    // Retrieve current values
    int currentWins = querySelect.value(0).toInt();
    int currentDraws = querySelect.value(1).toInt();
    int currentLosses = querySelect.value(2).toInt();

    // Calculate new values
    int newWins = currentWins + xWins;  // Replace xWins with your variable holding new wins to add
    int newDraws = currentDraws + draws;  // Replace draws with your variable holding new draws to add
    int newLosses = currentLosses + oWins;  // Replace oWins with your variable holding new losses to add

    // Update values in the database for username 'oza'
    QSqlQuery queryUpdate(db);
    queryUpdate.prepare("UPDATE users SET wins = :wins, draws = :draws, losses = :losses WHERE username = :oza");
    queryUpdate.bindValue(":oza", loggedInUsername);
    queryUpdate.bindValue(":wins", newWins);
    queryUpdate.bindValue(":draws", newDraws);
    queryUpdate.bindValue(":losses", newLosses);

    if (!queryUpdate.exec()) {
        qDebug() << "Error updating database:" << queryUpdate.lastError().text();
        QSqlDatabase::database().rollback();
    } else {
        QSqlDatabase::database().commit();
    }

    db.close();
}

