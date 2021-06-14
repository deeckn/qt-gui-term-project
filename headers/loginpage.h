#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QMainWindow>
#include <QtCore>
#include <QMessageBox>
#include <QDate>
#include <QTime>
#include <memory>
#include <databaseManager.h>
#include <directorypage.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainPage; }
QT_END_NAMESPACE

class MainPage : public QMainWindow
{
    Q_OBJECT

public:
    MainPage(QWidget *parent = nullptr);
    ~MainPage();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainPage *ui;
    DatabaseManager manager;
    std::unique_ptr<DirectoryPage> directoryPage;
    void verifyLogIn();
};
#endif // LOGINPAGE_H
