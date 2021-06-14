#ifndef ADDREMOVECUSTOMER_H
#define ADDREMOVECUSTOMER_H

#include <QDialog>
#include <QtCore>
#include <QSql>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>
#include <customer.h>
#include <room.h>
#include <databaseManager.h>
#include <permissionpage.h>

namespace Ui {
class AddRemoveCustomer;
}

class AddRemoveCustomer : public QDialog
{
    Q_OBJECT

public:
    explicit AddRemoveCustomer(QWidget *parent = nullptr);
    ~AddRemoveCustomer();

signals:
    void updateStatusPage();

private slots:
    // Qt Built-in slots
    void on_addRadioButton_clicked();
    void on_removeRadioButton_clicked();
    void on_submitButton_clicked();

    // User created slots
    void addCustomer();
    void removeCustomer();

    void on_backButton_clicked();

private:
    Ui::AddRemoveCustomer *ui;
    DatabaseManager manager;
    void fillAge();
    void fillCustomerID();
    void fillRooms();
};

#endif // ADDREMOVECUSTOMER_H
