#include "addremovecustomer.h"
#include "ui_addremovecustomer.h"

AddRemoveCustomer::AddRemoveCustomer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddRemoveCustomer)
{
    ui->setupUi(this);
    manager.connectDatabase();
    fillAge();
    fillCustomerID();
    fillRooms();
}

AddRemoveCustomer::~AddRemoveCustomer()
{
    delete ui;
}

void AddRemoveCustomer::fillAge()
{
    for (int i{18}; i <= 100; i++){
        ui->ageComboBox->addItem(QString::number(i));
    }
}

void AddRemoveCustomer::fillCustomerID()
{
    QSqlQuery searchId;
    searchId.exec("SELECT CustomerID from customerList");

    while (searchId.next())
    {
        QSqlRecord record = searchId.record();
        ui->idComboBox->addItem(record.value("CustomerID").toString());
    }
}

void AddRemoveCustomer::fillRooms()
{
    QSqlQuery searchRooms;
    searchRooms.exec("SELECT * from roomList where isOccupied=0");

    while (searchRooms.next())
    {
        QSqlRecord record = searchRooms.record();
        ui->roomComboBox->addItem(record.value("RoomLabel").toString());
    }
}

void AddRemoveCustomer::on_addRadioButton_clicked()
{
    ui->firstLineEdit->setEnabled(true);
    ui->lastLineEdit->setEnabled(true);
    ui->ageComboBox->setEnabled(true);
    ui->genderComboBox->setEnabled(true);
    ui->dateLineEdit->setEnabled(true);
    ui->durationLineEdit->setEnabled(true);
    ui->balanceLineEdit->setEnabled(true);
    ui->statusComboBox->setEnabled(true);
    ui->roomComboBox->setEnabled(true);
    ui->idComboBox->setEnabled(false);
}

void AddRemoveCustomer::on_removeRadioButton_clicked()
{
    ui->firstLineEdit->setEnabled(false);
    ui->lastLineEdit->setEnabled(false);
    ui->ageComboBox->setEnabled(false);
    ui->genderComboBox->setEnabled(false);
    ui->dateLineEdit->setEnabled(false);
    ui->durationLineEdit->setEnabled(false);
    ui->balanceLineEdit->setEnabled(false);
    ui->statusComboBox->setEnabled(false);
    ui->roomComboBox->setEnabled(false);
    ui->idComboBox->setEnabled(true);
}

void AddRemoveCustomer::on_submitButton_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confimation",
                                  "Are you sure you want change infomation in the database?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes && ui->addRadioButton->isChecked())
    {
        if (ui->firstLineEdit->text() == "" ||
            ui->lastLineEdit->text() == "" ||
            ui->ageComboBox->currentText() == "" ||
            ui->genderComboBox->currentText() == "" ||
            ui->dateLineEdit->text() == "" ||
            ui->durationLineEdit->text() == "" ||
            ui->balanceLineEdit->text() == "" ||
            ui->statusComboBox->currentText() == "" ||
            ui->roomComboBox->currentText() == "")
        {
            QMessageBox::warning(this, "Error", "All fields are required");
            return;
        }

        PermissionPage *page = new PermissionPage();
        page->show();
        QObject::connect(page, &PermissionPage::accessGranted, this, &AddRemoveCustomer::addCustomer);
    }
    else if (reply == QMessageBox::Yes && ui->removeRadioButton->isChecked())
    {
        if (ui->idComboBox->currentText() == "")
        {
            QMessageBox::warning(this, "Error", "Please enter the customer ID");
            return;
        }

        PermissionPage *page = new PermissionPage();
        page->show();
        QObject::connect(page, &PermissionPage::accessGranted, this, &AddRemoveCustomer::removeCustomer);
    }
    else { return; }
}

void AddRemoveCustomer::addCustomer()
{
    // Determining new ID for new customer
    int id{0};
    QSqlQuery findId;
    findId.exec("SELECT CustomerID from customerList");
    while (findId.next())
    {
        QSqlRecord record = findId.record();
        id = record.value("CustomerID").toInt();
    } id++;

    // Retreive data from input fields and create customer object
    QString first = ui->firstLineEdit->text();
    QString last = ui->lastLineEdit->text();
    int age = ui->ageComboBox->currentText().toInt();
    QString gender = ui->genderComboBox->currentText();
    QString date = ui->dateLineEdit->text();
    int duration = ui->durationLineEdit->text().toInt();
    int balance = ui->balanceLineEdit->text().toInt();
    bool status;
    if (ui->statusComboBox->currentText() == "Paid") { status = true; }
    else { status = false; }
    Customer customer{first, last, age, gender, id, date, duration, status, balance};

    // Making room object from input field
    QString roomLabel = ui->roomComboBox->currentText();
    QSqlQuery findRoom;
    findRoom.exec("SELECT * from roomList where RoomLabel='" + roomLabel + "'");
    Room *tempRoom = nullptr;
    while (findRoom.next())
    {
        QSqlRecord record = findRoom.record();
        QString label = record.value("RoomLabel").toString();
        int width = record.value("Width").toInt();
        int length = record.value("Length").toInt();
        int height = record.value("Height").toInt();
        int price = record.value("Price").toInt();
        bool hasAC = record.value("HasAC").toBool();
        bool isOccupied = record.value("isOccupied").toBool();
        tempRoom = new Room(label, width, length, height, price, hasAC, isOccupied);
    }

    // Adding room to customer and adding customer to database
    if (tempRoom != nullptr) { customer.addCustomerRooms(*tempRoom); }
    else { return; }
    manager.addCustomer(customer);
    manager.updateRoomOccupancy(roomLabel, true);

    qInfo() << "Customer ID=" + QString::number(id) + " is added to the database";
    delete tempRoom;
    emit updateStatusPage();

    // Adding log entry
    QString action = "New Customer ID: " + QString::number(id) + " has been added to the database";
    manager.addLogEntry(action);
}

void AddRemoveCustomer::removeCustomer()
{
    QString id = ui->idComboBox->currentText();

    // Free rooms the customer occupied
    QSqlQuery findRooms;
    findRooms.exec("SELECT RoomsRented from customerList where CustomerID=" + id);
    QString roomLabelString;

    while (findRooms.next())
    {
        QSqlRecord record = findRooms.record();
        roomLabelString = record.value("RoomsRented").toString();
    }
    QStringList roomList = roomLabelString.split("|");

    foreach(QString r, roomList) {
        manager.updateRoomOccupancy(r, false);
    }

    // Deleting customer from database
    QSqlQuery deleteCustomer;
    deleteCustomer.exec("DELETE from customerList where CustomerID=" + id);
    qInfo() << "Customer ID=" + id + " is deleted";
    emit updateStatusPage();

    QString action = "Customer ID: " + id + " has been removed from the database";
    manager.addLogEntry(action);
}

void AddRemoveCustomer::on_backButton_clicked() { close(); }
