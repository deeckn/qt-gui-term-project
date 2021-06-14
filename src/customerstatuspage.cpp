#include "customerstatuspage.h"
#include "ui_customerstatuspage.h"

/*
    This page is for the modification of customer infomation in the database
*/

CustomerStatusPage::CustomerStatusPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CustomerStatusPage)
{
    ui->setupUi(this);

    //Connecting to database
    manager.connectDatabase();

    // Fill Combo Boxes
    fillScrollArea();
    fillAgeComboBox();
    fillCustomerIdComboBox();
}

// Destructor deletes pointers
CustomerStatusPage::~CustomerStatusPage()
{
    delete ui;
    delete vlay;
    manager.db.close();
}

// Back button clicked
void CustomerStatusPage::on_pushButton_clicked() { hide(); emit sendLogUpdateSignal(); }

// Filling in scroll area table
void CustomerStatusPage::fillScrollArea()
{
    // Filling in scrollbar area
    vlay = new QVBoxLayout(ui->scrollAreaWidgetContents);

    // Query to database
    QSqlQuery qry;
    QString query = "SELECT * from customerList";
    qry.exec(query);

    // Creating a row in the table
    while (qry.next())
    {
        QHBoxLayout *hlay = new QHBoxLayout(ui->scrollAreaWidgetContents);
        QSqlRecord record = qry.record();
        QLabel *id = new QLabel();
        QLabel *first = new QLabel();
        QLabel *last = new QLabel();
        QLabel *age = new QLabel();
        QLabel *gender = new QLabel();
        QLabel *date = new QLabel();
        QLabel *rent = new QLabel();
        QLabel *rooms = new QLabel();
        QLabel *balance = new QLabel();
        QLabel *status = new QLabel();

        // Set label text
        id->setText(QString::number(record.value("CustomerID").toInt()));
        first->setText(record.value("FirstName").toString());
        last->setText(record.value("LastName").toString());
        age->setText(QString::number(record.value("Age").toInt()));
        gender->setText(record.value("Gender").toString());
        date->setText(record.value("DateJoined").toString());
        rent->setText(QString::number(record.value("RentalDuration").toInt()));
        rooms->setText(record.value("RoomsRented").toString());
        balance->setText(QString::number(record.value("PaymentBalance").toInt()));
        if (record.value("PaymentStatus").toBool()) {status->setText("Paid");}
        else {status->setText("Unpaid");}

        // Set label stylesheet. Font 16px.
        id->setStyleSheet("* {font-size: 16px;}");
        first->setStyleSheet("* {font-size: 16px;}");
        last->setStyleSheet("* {font-size: 16px;}");
        age->setStyleSheet("* {font-size: 16px;}");
        gender->setStyleSheet("* {font-size: 16px;}");
        date->setStyleSheet("* {font-size: 16px;}");
        rent->setStyleSheet("* {font-size: 16px;}");
        rooms->setStyleSheet("* {font-size: 16px;}");
        balance->setStyleSheet("* {font-size: 16px;}");
        status->setStyleSheet("* {font-size: 16px;}");

        // Adding label widget to horizontal layout container
        hlay->addWidget(id);
        hlay->addWidget(first);
        hlay->addWidget(last);
        hlay->addWidget(age);
        hlay->addWidget(gender);
        hlay->addWidget(date);
        hlay->addWidget(rent);
        hlay->addWidget(rooms);
        hlay->addWidget(balance);
        hlay->addWidget(status);

        // Adding horizontal layout to the vertical layout container
        vlay->addLayout(hlay);
    }
}

// Filling age combobox contents, age 18-100
void CustomerStatusPage::fillAgeComboBox()
{
    ui->ageComboBox->addItem("");
    for (int i{18}; i <= 100; i++){
        ui->ageComboBox->addItem(QString::number(i));
    }
}

// Filling in combobox contents, all avaliable CustomerID
void CustomerStatusPage::fillCustomerIdComboBox()
{
    QSqlQuery qry;
    QString query = "SELECT CustomerID from customerList";
    qry.exec(query);

    while (qry.next()) {
        QSqlRecord record = qry.record();
        ui->idComboBox->addItem(record.value("CustomerID").toString());
    }
}

// User verification before updating database
void CustomerStatusPage::updateInfo()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confimation",
                                  "Are you sure you want to change this customer's info?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        PermissionPage *page = new PermissionPage();
        page->show();
        QObject::connect(page, &PermissionPage::accessGranted, this, &CustomerStatusPage::editDatabase);
    }
    else { return; }
}

// Modifies the database data
void CustomerStatusPage::editDatabase()
{
    qInfo() << "Permission is accepted from the user";
    int id = ui->idComboBox->currentText().toInt();

    // Sets data in database according the user inputs
    if (ui->firstLineEdit->text() != ""){ manager.updateCustomerFirstName(id, ui->firstLineEdit->text());}
    if (ui->lastLineEdit->text() != ""){ manager.updateCustomerLastName(id, ui->lastLineEdit->text());}
    if (ui->ageComboBox->currentText() != ""){ manager.updateCustomerAge(id, ui->ageComboBox->currentText().toInt());}
    if (ui->genderComboBox->currentText() != ""){ manager.updateCustomerGender(id, ui->genderComboBox->currentText());}
    if (ui->dateLineEdit->text() != ""){ manager.updateCustomerDateJoined(id, ui->dateLineEdit->text());}
    if (ui->durationLineEdit->text() != ""){ manager.updateCustomerRentalDuration(id, ui->durationLineEdit->text().toInt());}
    if (ui->balanceLineEdit->text() != "") { manager.updateCustomerPaymentBalance(id, ui->balanceLineEdit->text().toInt());}
    if (ui->statusComboBox->currentText() != "")
    {
        if (ui->statusComboBox->currentText() == "Paid"){ manager.updateCustomerPaymentStatus(id, true);}
        else { manager.updateCustomerPaymentStatus(id, false);}
    }
    updatePage();

    ui->idComboBox->setCurrentIndex(0);
    ui->firstLineEdit->clear();
    ui->lastLineEdit->clear();
    ui->ageComboBox->setCurrentIndex(0);
    ui->genderComboBox->setCurrentIndex(0);
    ui->dateLineEdit->clear();
    ui->durationLineEdit->clear();
    ui->balanceLineEdit->clear();
    ui->statusComboBox->setCurrentIndex(0);

    QString action = "Customer ID: " + ui->idComboBox->currentText() + " data has been modified";
    manager.addLogEntry(action);
    emit sendLogUpdateSignal();
}

// Update info button clicked
void CustomerStatusPage::on_updateInfoButton_clicked(){ updateInfo(); }

// Add-Remove rooms button clicked
void CustomerStatusPage::on_editRoomsButton_clicked()
{
    addRemoveRooms *room = new addRemoveRooms();
    room->show();
    QObject::connect(room, &addRemoveRooms::updateStatusPage, this, &CustomerStatusPage::updatePage);
}

// Add-Remove customer button clicked
void CustomerStatusPage::on_editCustomersButton_clicked()
{
    AddRemoveCustomer *customerEdit = new AddRemoveCustomer();
    customerEdit->show();
    QObject::connect(customerEdit, &AddRemoveCustomer::updateStatusPage, this, &CustomerStatusPage::updatePage);
}

// Update table contents
void CustomerStatusPage::updatePage()
{
    qDeleteAll(ui->scrollAreaWidgetContents->children());
    fillScrollArea();
}
