#include "facilityinfo.h"
#include "ui_facilityinfo.h"

/*
    This page gives a visual representation of the facility and displays the infomation of every room
    This page is the 1st floor of the facility
*/

FacilityInfo::FacilityInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FacilityInfo)
{
    ui->setupUi(this);
    manager.connectDatabase();
    updateRoomOccupancy();
}

FacilityInfo::~FacilityInfo()
{
    delete ui;
    delete facilityinfo2;
    delete infoBox;
    manager.db.close();
}

// Updates the color of each room based on opccupancy
void FacilityInfo::updateRoomOccupancy() {

        std::vector<Customer> customerList;
        std::vector<int> idList;

        // Fill idList with available IDs
        QString query = "SELECT CustomerID from customerList";
        QSqlQuery qry;
        qry.exec(query);
        while (qry.next()){
            QSqlRecord record = qry.record();
            int id = record.value("CustomerID").toInt();
            idList.push_back(id);
        }

        // Arranges all customers into a vector
        for (int id : idList) {
            std::unique_ptr<Customer> tempCustomer = DatabaseManager::getCustomerFromId(id);
            if (tempCustomer != nullptr){
                customerList.push_back(*tempCustomer);
            } else { continue; }
        }

        // Color change. Occupied = Red, Unoccupied = Green
        for (Customer c : customerList) {
            for (Room r : c.getCustomerRoomsVector()) {
                if (r.getIsOccupied()) {
                    QString roomLabel = r.getRoomLabel();
                    QString toRed = "* {border-radius: 0; background-color: #f94144;}";
                    if (roomLabel == "A01"){ ui->a01_button->setStyleSheet(toRed); }
                    else if (roomLabel == "A02"){ ui->a02_button->setStyleSheet(toRed); }
                    else if (roomLabel == "A03"){ ui->a03_button->setStyleSheet(toRed); }
                    else if (roomLabel == "A04"){ ui->a04_button->setStyleSheet(toRed); }
                    else if (roomLabel == "A05"){ ui->a05_button->setStyleSheet(toRed); }
                    else if (roomLabel == "A06"){ ui->a06_button->setStyleSheet(toRed); }
                    else if (roomLabel == "B01"){ ui->b01_button->setStyleSheet(toRed); }
                    else if (roomLabel == "B02"){ ui->b02_button->setStyleSheet(toRed); }
                    else if (roomLabel == "B03"){ ui->b03_button->setStyleSheet(toRed); }
                    else if (roomLabel == "B04"){ ui->b04_button->setStyleSheet(toRed); break; }
                }
            }
        }
}

// Back button is clicked
void FacilityInfo::on_backButton_clicked() { close(); }

// Opens the 2nd floor page
void FacilityInfo::on_to2fButton_clicked()
{
    facilityinfo2 = new FacilityInfo2(nativeParentWidget());
    facilityinfo2->show();
}

// Shows an info box of the room clicked
void FacilityInfo::showRoomInfo(Room r)
{
    infoBox = new RoomInfoBox();
    infoBox->setRoom(&r);
    infoBox->show();
}

// Retrieves data from database and generate a Room object from the label
void FacilityInfo::roomInfoFromLabel(QString roomLabel)
{
    QString query = "SELECT * from roomList where RoomLabel='" + roomLabel + "'";
    QSqlQuery qry;
    qry.exec(query);
    Room *temp;

    while (qry.next())
    {
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
void FacilityInfo::on_a01_button_clicked(){ roomInfoFromLabel(ui->a01_button->text()); }
void FacilityInfo::on_a02_button_clicked(){ roomInfoFromLabel(ui->a02_button->text()); }
void FacilityInfo::on_a03_button_clicked(){ roomInfoFromLabel(ui->a03_button->text()); }
void FacilityInfo::on_a04_button_clicked(){ roomInfoFromLabel(ui->a04_button->text()); }
void FacilityInfo::on_a05_button_clicked(){ roomInfoFromLabel(ui->a05_button->text()); }
void FacilityInfo::on_a06_button_clicked(){ roomInfoFromLabel(ui->a06_button->text()); }
void FacilityInfo::on_b01_button_clicked(){ roomInfoFromLabel(ui->b01_button->text()); }
void FacilityInfo::on_b02_button_clicked(){ roomInfoFromLabel(ui->b02_button->text()); }
void FacilityInfo::on_b03_button_clicked(){ roomInfoFromLabel(ui->b03_button->text()); }
void FacilityInfo::on_b04_button_clicked(){ roomInfoFromLabel(ui->b04_button->text()); }
