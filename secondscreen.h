#ifndef SECONDSCREEN_H
#define SECONDSCREEN_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class secondscreen; }
QT_END_NAMESPACE

class QPushButton;
class QVBoxLayout;

class secondscreen : public QMainWindow
{
    Q_OBJECT

public:
    explicit secondscreen(const QString &loggedInUsername, QWidget *parent = nullptr); // Constructor to receive loggedInUsername
    ~secondscreen();

private slots:
    void onButton1Clicked();
    void onButton2Clicked();
    void onButton3Clicked();

private:
    Ui::secondscreen *ui;
    QString loggedInUsername; // Member variable to store loggedInUsername
    void setButtonStyle(QPushButton *button);
};

#endif // SECONDSCREEN_H
