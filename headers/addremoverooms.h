#ifndef ADDREMOVEROOMS_H
#define ADDREMOVEROOMS_H

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
class addRemoveRooms;
}

class addRemoveRooms : public QDialog
{
    Q_OBJECT

public:
    explicit addRemoveRooms(QWidget *parent = nullptr);
    ~addRemoveRooms();

signals:
    void updateStatusPage();

private slots:
    void on_backButton_clicked();
    void on_idComboBox_currentTextChanged(const QString &arg1);
    void on_addRadio_clicked();
    void on_removeRadio_clicked();
    void editDatabase();

    void on_updateButton_clicked();

private:
    Ui::addRemoveRooms *ui;
    DatabaseManager manager;

    void fillCustomerIdComboBox();
    void updateRooms();
};

#endif // ADDREMOVEROOMS_H
