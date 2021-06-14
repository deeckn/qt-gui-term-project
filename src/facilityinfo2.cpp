#include "facilityinfo2.h"
#include "ui_facilityinfo2.h"

/*
    This page gives a visual representation of the facility and displays the infomation of every room
    This page is the 2nd floor of the facility
*/

FacilityInfo2::FacilityInfo2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FacilityInfo2)
{
    ui->setupUi(this);

    manager.connectDatabase();
    updateRoomOccupancy();
}

FacilityInfo2::~FacilityInfo2()
{
    delete ui;
    delete infoBox;
    manager.db.close();
}

void FacilityInfo2::updateRoomOccupancy()
{
    // Customer vector
    std::vector<Customer> customerList;
    std::vector<int> idList;
    QString query = "SELECT CustomerID from customerList";
    QSqlQuery qry;
    qry.exec(query);
    while (qry.next()){
        QSqlRecord record = qry.record();
        int id = record.value("CustomerID").toInt();
        idList.push_back(id);
    }

    // Create customer from id list
    for (int id : idList) {
        std::unique_ptr<Customer> tempCustomer = DatabaseManager::getCustomerFromId(id);
        if (tempCustomer != nullptr){
            customerList.push_back(*tempCustomer);
        } else { continue; }
    }

    // Checking process if room is occupied then the color of the room will change to red
    for (Customer c : customerList)
    {
        for (Room r : c.getCustomerRoomsVector())
        {
            if (r.getIsOccupied())
            {
                QString roomLabel = r.getRoomLabel();
                QString toRed = "* {border-radius: 0; background-color: #f94144;}";
                if (roomLabel == "C01"){ ui->c01_button->setStyleSheet(toRed); }
                else if (roomLabel == "C02"){ ui->c02_button->setStyleSheet(toRed); }
                else if (roomLabel == "C03"){ ui->c03_button->setStyleSheet(toRed); }
                else if (roomLabel == "C04"){ ui->c04_button->setStyleSheet(toRed); }
                else if (roomLabel == "C05"){ ui->c05_button->setStyleSheet(toRed); }
                else if (roomLabel == "C06"){ ui->c06_button->setStyleSheet(toRed); }
                else if (roomLabel == "D01"){ ui->d01_button->setStyleSheet(toRed); }
                else if (roomLabel == "D02"){ ui->d02_button->setStyleSheet(toRed); }
            }
        }
    }
}

// Shows an info box of the room clicked
void FacilityInfo2::showRoomInfo(Room r)
{
    infoBox = new RoomInfoBox();
    infoBox->setRoom(&r);
    infoBox->show();
}

// Retrieves data from database and generate a Room object from the label
void FacilityInfo2::roomInfoFromLabel(QString roomLabel)
{
    QString query = "SELECT * from roomList where RoomLabel='" + roomLabel + "'";
    QSqlQuery qry;
    qry.exec(query);
    Room *temp;
    while (qry.next()) {
        QSqlRecord roomRecord = qry.record();
        QString label = roomRecord.value("RoomLabel").toString();
        int width = roomRecord.value("Width").toInt();
        int length = roomRecord.value("Length").toInt();
        int height = roomRecord.value("Height").toInt();
        int price = roomRecord.value("Price").toInt();
        bool hasAC = roomRecord.value("HasAC").toBool();
        bool isOccupied = roomRecord.value("isOccupied").toBool();
        temp = new Room(label, width, length, height, price, hasAC, isOccupied);
        showRoomInfo(*temp);
    }
}

// All rooms in the page are buttons, shows the corresponding info to the room
void FacilityInfo2::on_c01_button_clicked() { roomInfoFromLabel(ui->c01_button->text()); }
void FacilityInfo2::on_c02_button_clicked() { roomInfoFromLabel(ui->c02_button->text()); }
void FacilityInfo2::on_c03_button_clicked() { roomInfoFromLabel(ui->c03_button->text()); }
void FacilityInfo2::on_c04_button_clicked() { roomInfoFromLabel(ui->c04_button->text()); }
void FacilityInfo2::on_c05_button_clicked() { roomInfoFromLabel(ui->c05_button->text()); }
void FacilityInfo2::on_c06_button_clicked() { roomInfoFromLabel(ui->c06_button->text()); }
void FacilityInfo2::on_d01_button_clicked() { roomInfoFromLabel(ui->d01_button->text()); }
void FacilityInfo2::on_d02_button_clicked() { roomInfoFromLabel(ui->d02_button->text()); }

// Back button is clicked
void FacilityInfo2::on_backButton_clicked() { close(); }
