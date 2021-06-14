#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H
#include <QCoreApplication>
#include <QDir>
#include <QtCore>
#include <QtSql>
#include <QDebug>
#include <memory>
#include "customer.h"
#include "room.h"

/*
    This class manages the data between this program and the database file
    Adding, updating, and removing data from the database is done here.
*/

class DatabaseManager {
public:
    QSqlDatabase db;

    QString getCurrentDirectory() { return QDir::currentPath(); }

    void connectDatabase() {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(getCurrentDirectory() + "/SSDatabase.sqlite");
        if (!db.open()){ qDebug() << "Problem opening database"; }
        else {qDebug() << "Connected to Database";}
    }

    // Update Customer Infomation Methods

    void addCustomer(Customer customer)
    {
        QSqlQuery qry;
        qry.prepare("INSERT INTO customerList ("
                    "CustomerID, FirstName, LastName,"
                    "Age, Gender, DateJoined,"
                    "RentalDuration, RoomsRented,"
                    "PaymentBalance, PaymentStatus)"
                    "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
        qry.addBindValue(customer.getCustomerID());
        qry.addBindValue(customer.getFirstName());
        qry.addBindValue(customer.getLastName());
        qry.addBindValue(customer.getAge());
        qry.addBindValue(customer.getGender());
        qry.addBindValue(customer.getDateJoined());
        qry.addBindValue(customer.getRentalDuration());
        qry.addBindValue(customer.getCustomerRoomsString());
        qry.addBindValue(customer.getPaymentBalance());
        qry.addBindValue(customer.getPaymentStatus());

        if (!qry.exec()){ qDebug() << "Error adding to database" << qry.lastError().text(); }
        else { qDebug() << "Database modification complete"; }
    }

    void updateCustomerFullName (int id, QString FirstName, QString LastName)
    {
        // Updating process
        QString query = "UPDATE customerList set FirstName='" + FirstName + "', LastName='" + LastName
                        + "' where CustomerID=" + QString::number(id);
        QSqlQuery qry;
        qry.prepare(query);
        if (qry.exec()){ qDebug() << "Successfully updated FirstName and LastName"; }
        else { qDebug() << "Error updating customer name"; }
    }

    void updateCustomerFirstName (int id, QString FirstName)
    {
        // Updating process
        QString query = "UPDATE customerList set FirstName='" + FirstName + "' where CustomerID=" + QString::number(id);
        QSqlQuery qry;
        qry.prepare(query);
        if (qry.exec()){ qDebug() << "Successfully updated FirstName and LastName"; }
        else { qDebug() << "Error updating customer name"; }
    }

    void updateCustomerLastName (int id, QString LastName)
    {
        // Updating process
        QString query = "UPDATE customerList set LastName='" + LastName
                        + "' where CustomerID=" + QString::number(id);
        QSqlQuery qry;
        qry.prepare(query);
        if (qry.exec()){ qDebug() << "Successfully updated FirstName and LastName"; }
        else { qDebug() << "Error updating customer name"; }
    }

    void updateCustomerAge (int id, int Age)
    {
        QString query = "UPDATE customerList set Age=" + QString::number(Age) + " where CustomerID=" + QString::number(id);
        QSqlQuery qry;
        qry.prepare(query);
        if (qry.exec()){ qDebug() << "Successfully updated Age"; }
        else { qDebug() << "Error updating customer age"; }
    }

    void updateCustomerGender (int id, QString gender)
    {
        QString query = "UPDATE customerList set Gender='" + gender + "' where CustomerID=" + QString::number(id);
        QSqlQuery qry;
        qry.prepare(query);
        if (qry.exec()){ qDebug() << "Successfully updated Gender"; }
        else { qDebug() << "Error updating customer gender"; }
    }

    void updateCustomerDateJoined(int id, QString dateJoined)
    {
        // Date format: dd-mm-yyyy
        QString query = "UPDATE customerList set DateJoined='" + dateJoined + "' where CustomerID=" + QString::number(id);
        QSqlQuery qry;
        qry.prepare(query);
        if (qry.exec()){ qDebug() << "Successfully updated DateJoined"; }
        else {qDebug() << "Error updating customer dateJoined";}
    }

    void updateCustomerRentalDuration(int id, int rentalDuration)
    {
        QString query = "UPDATE customerList set RentalDuration=" + QString::number(rentalDuration) + " where CustomerID=" + QString::number(id);
        QSqlQuery qry;
        qry.prepare(query);
        if (qry.exec()){ qDebug() << "Successfully updated RentalDuration"; }
        else { qDebug() << "Error updating customer rentalDuration"; }
    }

    void updateCustomerRoomsRented(Customer customer)
    {
        // Updating process
        QString query = "UPDATE customerList set RoomsRented='" + customer.getCustomerRoomsString() + "' where CustomerID=" + QString::number(customer.getCustomerID());
        QSqlQuery qry;
        qry.prepare(query);
        if (qry.exec()){ qDebug() << "Successfully updated RoomsRented"; }
        else {qDebug() << "Error updating customer roomsRented";}
    }

    void updateCustomerPaymentBalance(int id, int paymentBalance)
    {
        // Updating process
        QString query = "UPDATE customerList set PaymentBalance=" + QString::number(paymentBalance) + " where CustomerID=" + QString::number(id);
        QSqlQuery qry;
        qry.prepare(query);
        if (qry.exec()){ qDebug() << "Successfully updated PaymentBalance"; }
        else {qDebug() << "Error updating customer paymentBalance";}
    }

    void updateCustomerPaymentStatus(int id, bool status)
    {
        // Updating process
        QString query = "UPDATE customerList set PaymentStatus=" + QString::number(status) + " where CustomerID=" + QString::number(id);
        QSqlQuery qry;
        qry.prepare(query);
        if (qry.exec()){ qDebug() << "Successfully updated PaymentStatus"; }
        else { qDebug() << "Error updating customer paymentStatus"; }
    }

    // Update Room Information Methods

    void addRoom(Room room)
    {
        QSqlQuery qry;
        qry.prepare("INSERT INTO roomList ("
                    "RoomLabel, Width, Length, Height,"
                    "Price, HasAC, isOccupied)"
                    "VALUES (?, ?, ?, ?, ?, ?, ?);");
        qry.addBindValue(room.getRoomLabel());
        qry.addBindValue(room.getRoomWidth());
        qry.addBindValue(room.getRoomLength());
        qry.addBindValue(room.getRoomHeight());
        qry.addBindValue(room.getRoomPrice());
        qry.addBindValue(room.getHasAC());
        qry.addBindValue(room.getIsOccupied());

        if (!qry.exec()){ qDebug() << "Error adding to database" << qry.lastError().text(); }
        else { qDebug() << "Database modification complete"; }
    }

    void updateRoomPrice(QString label, int price)
    {
        QString query = "UPDATE roomList set Price=" + QString::number(price) + " where RoomLabel='"
                            + label + "'";
        QSqlQuery qry;
        qry.prepare(query);
        if (qry.exec()){ qDebug() << "Successfully updated PaymentStatus"; }
        else {qDebug() << "Error updating customer paymentStatus";}
    }

    void updateRoomOccupancy(QString label, bool status)
    {
        QString query = "UPDATE roomList set isOccupied=" + QString::number(status) + " where RoomLabel='"
                            + label + "'";
        QSqlQuery qry;
        qry.prepare(query);
        if (qry.exec()){ qDebug() << "Successfully updated room Occupancy Status"; }
        else {qDebug() << "Error updating room Occupancy Status";}
    }

    // Updating Log Entry

    void addLogEntry(QString action)
    {
        QDate today = QDate::currentDate();
        QStringList todayList = today.toString().split(" ");
        QString month = todayList.at(1);
        QString day = todayList.at(2);
        QString year = todayList.at(3);
        QTime timeTime = QTime::currentTime();
        QString time = timeTime.toString();

        QSqlQuery qry;
        qry.prepare("INSERT INTO logBook (Year, Month, Day, Time, Action)"
                    "VALUES (?, ?, ?, ?, ?);");
        qry.addBindValue(year);
        qry.addBindValue(month);
        qry.addBindValue(day);
        qry.addBindValue(time);
        qry.addBindValue(action);

        if (!qry.exec()){ qDebug() << "Error adding log to database" << qry.lastError().text(); }
        else {qDebug() << "A log has been added to the log book";}
    }

    // Retrieve Customer Data from Database

    static std::unique_ptr<Customer> getCustomerFromId(int aId){
        QString query = "SELECT * FROM customerList WHERE CustomerID=" + QString::number(aId);
        QSqlQuery qry;
        qry.prepare(query);

        if (!qry.exec()){ qDebug() << "Error retrieving customer from database" << qry.lastError().text(); }
        else { qDebug() << "Customer Retrieved from Database"; }

        // Creating customer and room objects
        std::unique_ptr<Customer> tempCustomer = nullptr;
        while (qry.next())
        {
            QSqlRecord record = qry.record();
            QString first = record.value("FirstName").toString();
            QString last = record.value("LastName").toString();
            QString gender = record.value("Gender").toString();
            QString date = record.value("DateJoined").toString();
            QString room = record.value("RoomsRented").toString();
            int age = record.value("Age").toInt();
            int id = record.value("CustomerID").toInt();
            int rental = record.value("RentalDuration").toInt();
            int balance = record.value("PaymentBalance").toInt();
            bool status = record.value("PaymentStatus").toBool();

            if (room == ""){ continue; } // In case the customer has no rooms no need to proceed

            tempCustomer = std::make_unique<Customer>(first, last, age, gender, id, date, rental, status, balance);

            QStringList roomStringList = room.split("|");
            foreach(QString r, roomStringList)
            {
                QSqlQuery roomQry;
                roomQry.exec("SELECT * from roomList where RoomLabel='" + r + "'");
                std::unique_ptr<Room> tempRoom = nullptr;
                while (roomQry.next())
                {
                    QSqlRecord roomRecord = roomQry.record();
                    QString label = roomRecord.value("RoomLabel").toString();
                    int width = roomRecord.value("Width").toInt();
                    int length = roomRecord.value("Length").toInt();
                    int height = roomRecord.value("Height").toInt();
                    int price = roomRecord.value("Price").toInt();
                    bool hasAC = roomRecord.value("HasAC").toBool();
                    bool isOccupied = roomRecord.value("isOccupied").toBool();

                    tempRoom = std::make_unique<Room>(label, width, length, height, price, hasAC, isOccupied);
                    tempCustomer->addCustomerRooms(*tempRoom);
                }
            }
        }

        if (tempCustomer != nullptr){ return tempCustomer; }
        else { return nullptr; }
    }
};

#endif // DATABASEMANAGER_H
