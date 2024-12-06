#include "gameframeai.h"
#include "ui_gameframeai.h"
#include "secondscreen.h"
#include <QPushButton>
#include <QGridLayout>
#include <QMessageBox>
#include <QFont>
#include <QElapsedTimer>
#include <QThread>
#include <QDebug>
#include <QThread>
#include <QDebug>


gameframeai::gameframeai(const QString &loggedInUsername, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::gameframeai)
, loggedInUsername(loggedInUsername) // Initialize the member variable


{
    ui->setupUi(this);
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("D:/final pr/databaseee/data.db"); // Replace with your database file path

    if (!db.open()) {
        qDebug() << "Error: connection with database failed";
        QMessageBox::critical(this, "Database Error", "Connection with database failed. Signup failed.");
        return;}
    setFixedSize(500, 600);

    // Initialize scores
    playerWins = 0;
    aiWins = 0;
    drawsCounter = 0;
    // Set default AI difficulty
    maxDepth = 9; // Maximum depth for hardest difficulty

    // Create a grid layout to arrange the cells
    QGridLayout *gridLayout = new QGridLayout(ui->centralwidget);

    // Create buttons for each cell of the Tic Tac Toe board
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            // Create a clickable button for each cell
            QPushButton *cellButton = new QPushButton;
            cellButton->setFixedSize(100, 100); // Adjust the size of each cell
            cellButton->setProperty("row", row);
            cellButton->setProperty("col", col);
            // Set the font size of the button text
            QFont font = cellButton->font();
            font.setPointSize(36); // Set font size to 36 (adjust as needed)
            cellButton->setFont(font);

            // Connect the button to a slot for handling clicks
            connect(cellButton, &QPushButton::clicked, this, &gameframeai::cellClicked);

            // Add the cell button to the grid layout
            gridLayout->addWidget(cellButton, row, col);

            // Store the cell button in the 2D array for easy access later
            cellButtons[row][col] = cellButton;
        }
    }

    // Create back button
    backButton = new QPushButton("Back");
    QFont backFont = backButton->font();
    backFont.setPointSize(16); // Set font size for back button
    backButton->setFont(backFont);
    connect(backButton, &QPushButton::clicked, this, &gameframeai::goBack);
    gridLayout->addWidget(backButton, 3, 0, 1, 3); // Add back button to the layout

    // Create restart button
    restartButton = new QPushButton("Restart");
    QFont restartFont = restartButton->font();
    restartFont.setPointSize(16); // Set font size for restart button
    restartButton->setFont(restartFont);
    connect(restartButton, &QPushButton::clicked, this, &gameframeai::restartGame);
    gridLayout->addWidget(restartButton, 4, 0, 1, 3); // Add restart button to the layout

    // Create difficulty buttons
    QPushButton *easyButton = new QPushButton("Easy");
    connect(easyButton, &QPushButton::clicked, this, &gameframeai::setDifficultyEasy);
    gridLayout->addWidget(easyButton, 5, 0);

    QPushButton *mediumButton = new QPushButton("Medium");
    connect(mediumButton, &QPushButton::clicked, this, &gameframeai::setDifficultyMedium);
    gridLayout->addWidget(mediumButton, 5, 1);

    QPushButton *hardButton = new QPushButton("Hard");
    connect(hardButton, &QPushButton::clicked, this, &gameframeai::setDifficultyHard);
    gridLayout->addWidget(hardButton, 5, 2);

    // Initially update scores display
    updateScores();
}

gameframeai::~gameframeai()
{
    delete ui;
}

void gameframeai::cellClicked()
{
    QElapsedTimer timer;
    timer.start();

    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    if (!clickedButton)
        return;

    int row = clickedButton->property("row").toInt();
    int col = clickedButton->property("col").toInt();

    // Check if the cell is already occupied
    if (!clickedButton->text().isEmpty())
    {
        QMessageBox::warning(this, "Invalid Move", "This cell is already occupied.");
        return;
    }

    // Simulate additional workload to make elapsed time measurable
    QThread::msleep(50);

    // Set the player's symbol (X) on the clicked cell
    clickedButton->setText("X");

    // Check for a player win or draw
    if (checkWin(row, col, "Player"))
    {
        ++playerWins; // Increment player's win count
        updateScores(); // Update scores display
        QMessageBox::information(this, "Winner", "Player wins!");
        resetGame();
    }
    else if (checkDraw())
    {
        ++drawsCounter; // Increment draws counter
        updateScores(); // Update scores display
        QMessageBox::information(this, "Draw", "It's a draw!");
        resetGame();
    }
    else
    {

        // AI's turn
        aiMakeMove();

        // Check for an AI win or draw
        if (checkWin(lastAIMove.row, lastAIMove.col, "AI"))
        {
            ++aiWins; // Increment AI's win count
            updateScores(); // Update scores display
            QMessageBox::information(this, "Winner", "AI wins!");
            resetGame();
        }
        else if (checkDraw())
        {
            ++drawsCounter; // Increment draws counter
            updateScores(); // Update scores display
            QMessageBox::information(this, "Draw", "It's a draw!");
            resetGame();
        }
    }

    qint64 elapsed = timer.elapsed();
    qDebug() << "Elapsed time for the button clicked = " << elapsed << "ms";
}

void gameframeai::goBack()
{
    // Hide current window and show secondscreen
    hide(); // Hide the current window
    secondscreen *second = new secondscreen(loggedInUsername);
    updateDatabase();
    second->show(); // Show the secondscreen window
}

void gameframeai::restartGame()
{
    resetGame(); // Reset the game board
}

bool gameframeai::checkWin(int row, int col, const QString& player)
{
    QString symbol = (player == "Player") ? "X" : "O";

    // Check row
    if (cellButtons[row][0]->text() == symbol &&
        cellButtons[row][1]->text() == symbol &&
        cellButtons[row][2]->text() == symbol)
        return true;

    // Check column
    if (cellButtons[0][col]->text() == symbol &&
        cellButtons[1][col]->text() == symbol &&
        cellButtons[2][col]->text() == symbol)
        return true;

    // Check diagonals
    if ((cellButtons[0][0]->text() == symbol &&
         cellButtons[1][1]->text() == symbol &&
         cellButtons[2][2]->text() == symbol) ||
        (cellButtons[0][2]->text() == symbol &&
         cellButtons[1][1]->text() == symbol &&
         cellButtons[2][0]->text() == symbol))
        return true;

    return false;
}

bool gameframeai::checkDraw()
{
    // Check if all cells are filled
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

void gameframeai::resetGame()
{
    // Clear all cell buttons
    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            cellButtons[row][col]->setText("");
        }
    }

    // Update scores display after resetting
    updateScores();
}

void gameframeai::aiMakeMove()
{
    int bestScore = INT_MIN;
    Move bestMove;

    // Iterate through all empty cells
    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            if (cellButtons[row][col]->text().isEmpty())
            {
                cellButtons[row][col]->setText("O");
                int score = minimax(0, false);
                cellButtons[row][col]->setText("");

                if (score > bestScore)
                {
                    bestScore = score;
                    bestMove.row = row;
                    bestMove.col = col;
                }
            }
        }
    }

    // Set the AI's move
    cellButtons[bestMove.row][bestMove.col]->setText("O");
    lastAIMove = bestMove;
}

void gameframeai::setDifficultyEasy()
{
    maxDepth = 1; // Easy: very shallow search depth
}

void gameframeai::setDifficultyMedium()
{
    maxDepth = 4; // Medium: moderate search depth
}

void gameframeai::setDifficultyHard()
{
    maxDepth = 9; // Hard: maximum search depth
}

int gameframeai::minimax(int depth, bool isMax)
{
    int score = evaluateBoard();

    // Check if the game is over, the maximum depth is reached, or the game is a draw
    if (score == 10 || score == -10 || checkDraw() || depth >= maxDepth)
        return score;

    // If it's the maximizing player's turn (AI)
    if (isMax)
    {
        int bestScore = INT_MIN;
        for (int row = 0; row < 3; ++row)
        {
            for (int col = 0; col < 3; ++col)
            {
                if (cellButtons[row][col]->text().isEmpty())
                {
                    cellButtons[row][col]->setText("O");
                    bestScore = qMax(bestScore, minimax(depth + 1, !isMax));
                    cellButtons[row][col]->setText("");
                }
            }
        }
        return bestScore;
    }
    // If it's the minimizing player's turn (Player)
    else
    {
        int bestScore = INT_MAX;
        for (int row = 0; row < 3; ++row)
        {
            for (int col = 0; col < 3; ++col)
            {
                if (cellButtons[row][col]->text().isEmpty())
                {
                    cellButtons[row][col]->setText("X");
                    bestScore = qMin(bestScore, minimax(depth + 1, !isMax));
                    cellButtons[row][col]->setText("");
                }
            }
        }
        return bestScore;
    }
}

int gameframeai::evaluateBoard()
{
    // Evaluate the board and return a score
    // +10 for AI win, -10 for Player win, 0 for draw
    // The evaluation is based on the current state of the board
    // You can implement more sophisticated evaluation if needed

    for (int row = 0; row < 3; ++row)
    {
        if (cellButtons[row][0]->text() == cellButtons[row][1]->text() &&
            cellButtons[row][1]->text() == cellButtons[row][2]->text())
        {
            if (cellButtons[row][0]->text() == "O")
                return 10;
            else if (cellButtons[row][0]->text() == "X")
                return -10;
        }
    }

    for (int col = 0; col < 3; ++col)
    {
        if (cellButtons[0][col]->text() == cellButtons[1][col]->text() &&
            cellButtons[1][col]->text() == cellButtons[2][col]->text())
        {
            if (cellButtons[0][col]->text() == "O")
                return 10;
            else if (cellButtons[0][col]->text() == "X")
                return -10;
        }
    }

    if ((cellButtons[0][0]->text() == cellButtons[1][1]->text() &&
         cellButtons[1][1]->text() == cellButtons[2][2]->text()) ||
        (cellButtons[0][2]->text() == cellButtons[1][1]->text() &&
         cellButtons[1][1]->text() == cellButtons[2][0]->text()))
    {
        if (cellButtons[1][1]->text() == "O")
            return 10;
        else if (cellButtons[1][1]->text() == "X")
            return -10;
    }

    return 0; // No winner
}

void gameframeai::updateScores()
{
    // Update the score display
    QString playerScoreStr = "Player Wins: " + QString::number(playerWins);
    QString aiScoreStr = "AI Wins: " + QString::number(aiWins);
    QString drawsScoreStr = "Draws: " + QString::number(drawsCounter);
    ui->statusbar->showMessage(playerScoreStr + " | " + aiScoreStr + " | " + drawsScoreStr);
}
void gameframeai::updateDatabase() {
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
    int newWins = currentWins + playerWins;  // Replace xWins with your variable holding new wins to add
    int newDraws = currentDraws + drawsCounter;  // Replace draws with your variable holding new draws to add
    int newLosses = currentLosses + aiWins;  // Replace oWins with your variable holding new losses to add

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
