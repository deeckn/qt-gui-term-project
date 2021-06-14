#ifndef CUSTOMERSTATUSPAGE_H
#define CUSTOMERSTATUSPAGE_H

#include <QDialog>
#include <QtCore>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QtSql>
#include <QDate>
#include <QTime>
#include <QMessageBox>
#include <databaseManager.h>
#include <permissionpage.h>
#include <addremoverooms.h>
#include <addremovecustomer.h>

namespace Ui {
class CustomerStatusPage;
}

class CustomerStatusPage : public QDialog
{
    Q_OBJECT

public:
    explicit CustomerStatusPage(QWidget *parent = nullptr);
    ~CustomerStatusPage();

signals:
    void sendLogUpdateSignal();

private slots:
    // Qt Built-in slots
    void on_pushButton_clicked();
    void on_updateInfoButton_clicked();
    void on_editRoomsButton_clicked();
    void on_editCustomersButton_clicked();

    // User created slots
    void editDatabase();
    void updatePage();

private:
    Ui::CustomerStatusPage *ui;
    QVBoxLayout *vlay;
    DatabaseManager manager;

    void fillScrollArea();
    void fillAgeComboBox();
    void fillCustomerIdComboBox();
    void updateInfo();
};

#endif // CUSTOMERSTATUSPAGE_H
