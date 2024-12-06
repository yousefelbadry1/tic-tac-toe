#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QLineEdit* getUsernameLineEdit() const { return usernameLineEdit; }
    QLineEdit* getPasswordLineEdit() const { return passwordLineEdit; }
    QString getLoggedInUsername() const { return loggedInUsername; } // Add this method

public slots:
    void loginClicked();
    void signupClicked();

private:
    Ui::MainWindow *ui;
    QLineEdit *usernameLineEdit;
    QLineEdit *passwordLineEdit;
    QString loggedInUsername;
};

#endif // MAINWINDOW_H
