#ifndef GAMEFRAMEAI_H
#define GAMEFRAMEAI_H

#include <QMainWindow>
#include <QPushButton>
#include <QtSql>

QT_BEGIN_NAMESPACE
namespace Ui { class gameframeai; }
QT_END_NAMESPACE

class gameframeai : public QMainWindow
{
    Q_OBJECT

public:
    explicit gameframeai(const QString &loggedInUsername, QWidget *parent = nullptr); // Constructor to accept loggedInUsername
    ~gameframeai();

private slots:
    void cellClicked();
    void goBack();
    void restartGame(); // Added slot for restarting the current game

private:
    Ui::gameframeai *ui;
    QPushButton *cellButtons[3][3];
    QPushButton *backButton;
    QPushButton *restartButton; // Added restart button
    QSqlDatabase db;
    int maxDepth; // Variable to control AI difficulty

    QString loggedInUsername; // Member variable to store loggedInUsername

    void setDifficultyEasy();
    void setDifficultyMedium();
    void setDifficultyHard();
    void updateDatabase();


    struct Move {
        int row;
        int col;
    } lastAIMove;

    int minimax(int depth, bool isMax);
    int evaluateBoard();
    void aiMakeMove();
    bool checkWin(int row, int col, const QString& player);
    bool checkDraw();
    void resetGame();

    int playerWins;    // Counter for player wins
    int aiWins;        // Counter for AI wins
    int drawsCounter;  // Counter for draws

    void updateScores(); // Function to update the scores display
};

#endif // GAMEFRAMEAI_H
