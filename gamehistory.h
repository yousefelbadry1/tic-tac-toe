#ifndef GAMEHISTORY_H
#define GAMEHISTORY_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTableView>
#include <QSqlTableModel>
#include <QHeaderView> // Include QHeaderView for table header

QT_BEGIN_NAMESPACE
namespace Ui { class gamehistory; }
QT_END_NAMESPACE

class gamehistory : public QMainWindow
{
    Q_OBJECT

public:
    explicit gamehistory(const QString &loggedInUsername, QWidget *parent = nullptr);
    ~gamehistory();

private slots:
    void goBackToSecondScreen();

private:
    Ui::gamehistory *ui;
    QString loggedInUsername;
    QVBoxLayout *mainLayout;
    QPushButton *backButton;
    QTableView *tableView;
    QSqlTableModel *model;
    QSqlDatabase db;

    void updateModel();
};

#endif // GAMEHISTORY_H
