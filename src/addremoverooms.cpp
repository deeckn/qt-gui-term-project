#include "addremoverooms.h"
#include "ui_addremoverooms.h"

/*
    This page is an extention from the Customer Status Page
    Allowing the admin to add or remove rooms from an existing customer in the database
*/

addRemoveRooms::addRemoveRooms(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addRemoveRooms)
{
    ui->setupUi(this);

    manager.connectDatabase();
    fillCustomerIdComboBox();
    on_addRadio_clicked();
}

addRemoveRooms::~addRemoveRooms()
{
    delete ui;
    manager.db.close();
}

// Back button is clicked
void addRemoveRooms::on_backButton_clicked() { hide(); }

// Filling in combobox contents, all avaliable CustomerID.
void addRemoveRooms::fillCustomerIdComboBox()
{
    QSqlQuery qry;
    QString query = "SELECT CustomerID from customerList";
    qry.exec(query);

    while (qry.next())
    {
        QSqlRecord record = qry.record();
        ui->idComboBox->addItem(record.value("CustomerID").toString());
    }
}

// Updates the "customerNameLabel" when the "idComboBox" is updated
void addRemoveRooms::on_idComboBox_currentTextChanged(const QString &arg1)
{
    QSqlQuery qry;
    QString query = "SELECT FirstName, LastName from customerList where CustomerID=" + arg1;
    qry.exec(query);
    while (qry.next()){
        QSqlRecord record = qry.record();
        QString first = record.value("FirstName").toString();
        QString last = record.value("LastName").toString();
        QString fullName = first + " " + last;
        ui->customerNameLabel->setText(fullName);
     }

    if (ui->addRadio->isChecked()){ on_addRadio_clicked(); }
    if (ui->removeRadio->isChecked()){ on_removeRadio_clicked(); }
}

// Fills in the non-occupied rooms into the "roomComboBox"
void addRemoveRooms::on_addRadio_clicked()
{
    ui->roomComboBox->clear();
    QSqlQuery qry;
    QString query = "SELECT * from roomList where isOccupied=0";
    qry.exec(query);
    while (qry.next())
    {
        QSqlRecord record = qry.record();
        ui->roomComboBox->addItem(record.value("RoomLabel").toString());
    }
}

// User verification
void addRemoveRooms::on_updateButton_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confimation",
                                  "Are you sure you want to change this customer's info?",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        PermissionPage *page = new PermissionPage();
        page->show();
        QObject::connect(page, &PermissionPage::accessGranted, this, &addRemoveRooms::editDatabase);
    }
    else {return;}
}

// Fills in the customer occupied rooms into the "roomComboBox"
void addRemoveRooms::on_removeRadio_clicked()
{
    ui->roomComboBox->clear();
    QSqlQuery qry;
    QString query = "SELECT RoomsRented from customerList where CustomerID=" + ui->idComboBox->currentText();
    qry.exec(query);
    while (qry.next())
    {
        QSqlRecord record = qry.record();
        QStringList roomList = record.value("RoomsRented").toString().split("|");
        foreach(QString r, roomList) { ui->roomComboBox->addItem(r); }
    }
}

// Adds or deletes a customer room in the database
void addRemoveRooms::editDatabase()
{
    QString id = ui->idComboBox->currentText();
    QSqlQuery customerQry;
    customerQry.exec("SELECT * from customerList where CustomerID=" + id);

    std::unique_ptr<Customer> tempCustomer = DatabaseManager::getCustomerFromId(id.toInt());
    QString room = tempCustomer->getCustomerRoomsString();
    QStringList roomStringList = room.split("|");
    foreach(QString r, roomStringList)
    {
        QSqlQuery roomQry;
        roomQry.exec("SELECT * from roomList where RoomLabel='" + r + "'");
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
            std::unique_ptr<Room> tempRoom = std::make_unique<Room>(label, width, length, height, price, hasAC, isOccupied);
            tempCustomer->addCustomerRooms(*tempRoom);
        }
     }

    // Add Rooms
    if (ui->addRadio->isChecked()){
        QString roomChosen = ui->roomComboBox->currentText();
        QSqlQuery qryAdd;
        qryAdd.exec("SELECT * from roomList where RoomLabel='" + roomChosen + "'");
        while (qryAdd.next())
        {
            QSqlRecord roomRecord = qryAdd.record();
            QString label = roomRecord.value("RoomLabel").toString();
            int width = roomRecord.value("Width").toInt();
            int length = roomRecord.value("Length").toInt();
            int height = roomRecord.value("Height").toInt();
            int price = roomRecord.value("Price").toInt();
            bool hasAC = roomRecord.value("HasAC").toBool();
            bool isOccupied = roomRecord.value("isOccupied").toBool();
            std::unique_ptr<Room> tempRoom = std::make_unique<Room>(label, width, length, height, price, hasAC, isOccupied);
            tempCustomer->addCustomerRooms(*tempRoom);
        }
        manager.updateCustomerRoomsRented(*tempCustomer);
        manager.updateRoomOccupancy(ui->roomComboBox->currentText(), true);
        emit updateStatusPage();

        // Adding log entry
        QString action = "Customer ID: " + ui->idComboBox->currentText() + " Room: " + roomChosen + " has been added to the customer";
        manager.addLogEntry(action);
    }

    if (ui->removeRadio->isChecked())
    {
        QString roomChosen = ui->roomComboBox->currentText();
        tempCustomer->removeCustomerRooms(roomChosen);
        manager.updateCustomerRoomsRented(*tempCustomer);
        manager.updateRoomOccupancy(roomChosen, false);
        emit updateStatusPage();

        // Adding log entry
        QString action = "Customer ID: " + ui->idComboBox->currentText() + " Room: " + roomChosen + " has been remove from the customer";
        manager.addLogEntry(action);
    }
}
