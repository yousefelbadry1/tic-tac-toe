#ifndef GAMEFRAME_H
#define GAMEFRAME_H

#include <QMainWindow>
#include <QPushButton>
#include <QGridLayout>
#include <QMessageBox>
#include "secondscreen.h" // Include your secondscreen header file here
#include <QtSql>

QT_BEGIN_NAMESPACE
namespace Ui { class gameframe; }
QT_END_NAMESPACE

class gameframe : public QMainWindow
{
    Q_OBJECT

public:
    explicit gameframe(const QString &loggedInUsername, QWidget *parent = nullptr); // Constructor to accept loggedInUsername
    ~gameframe();

private slots:
    void cellClicked();
    void goBack();
    void resetGame(); // Added to reset the game board


private:
    Ui::gameframe *ui;
    QGridLayout *gridLayout;
    QPushButton *cellButtons[3][3];
    QPushButton *backButton;
    QPushButton *restartButton; // Added restart button
    char currentPlayer;
    QSqlDatabase db;
    int xWins;
    int oWins;
    int draws;

    // Instance of secondscreen
    secondscreen *secondScreen;
     QString loggedInUsername; // Member variable to store loggedInUsername

    void updateScores();
    void updateDatabase();
    bool checkWin(int row, int col);
    bool checkDraw();

};

#endif // GAMEFRAME_H

