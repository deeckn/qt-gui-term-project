#include "statusreportpage.h"
#include "ui_statusreportpage.h"

/*
    This page summarizes and displays all important information and calculations
    This page can provide a CSV report file to the user's computer
*/

StatusReportPage::StatusReportPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StatusReportPage)
{
    ui->setupUi(this);
    manager.connectDatabase();
    updateUnpaidScrollArea();
    updateUnusedScrollArea();
    calculateFinance();
    calculateSpace();
}

StatusReportPage::~StatusReportPage()
{
    delete ui;
    manager.db.close();
}

// Fills the Unpaid Customer Scroll Area
void StatusReportPage::updateUnpaidScrollArea()
{
    QVBoxLayout *vlay = new QVBoxLayout(ui->scrollAreaWidgetContents);
    QHBoxLayout *titleHlay = new QHBoxLayout(ui->scrollAreaWidgetContents);
    QLabel *title = new QLabel(ui->scrollAreaWidgetContents);
    title->setText("List of Unpaid Customers");
    title->setStyleSheet("* {font-size: 20px; background-color: #f2e9e4;}");
    title->setAlignment(Qt::AlignCenter);
    titleHlay->addWidget(title);
    vlay->addLayout(titleHlay);

    QSqlQuery qry;
    if (!qry.exec("SELECT CustomerID, FirstName, LastName, RoomsRented, PaymentBalance from customerList where PaymentStatus=0")){
        qDebug() << qry.lastError().text();
    }

    // Creating a row in the table
    while (qry.next())
    {
        QHBoxLayout *hlay = new QHBoxLayout(ui->scrollAreaWidgetContents);
        QSqlRecord record = qry.record();
        QLabel *id = new QLabel();
        QLabel *first = new QLabel();
        QLabel *last = new QLabel();
        QLabel *rooms = new QLabel();
        QLabel *balance = new QLabel();


        // Set label text
        id->setText(QString::number(record.value("CustomerID").toInt()));
        id->setAlignment(Qt::AlignCenter);
        first->setText(record.value("FirstName").toString());
        last->setText(record.value("LastName").toString());
        rooms->setText(record.value("RoomsRented").toString());
        rooms->setAlignment(Qt::AlignCenter);
        balance->setText(QString::number(record.value("PaymentBalance").toInt()));

        // Set label stylesheet. Font 16px.
        id->setStyleSheet("* {font-size: 16px;}");
        first->setStyleSheet("* {font-size: 16px;}");
        last->setStyleSheet("* {font-size: 16px;}");
        rooms->setStyleSheet("* {font-size: 16px;}");
        balance->setStyleSheet("* {font-size: 16px;}");

        // Adding label widget to horizontal layout container
        hlay->addWidget(id);
        hlay->addWidget(first);
        hlay->addWidget(last);
        hlay->addWidget(rooms);
        hlay->addWidget(balance);

        // Adding horizontal layout to the vertical layout container
        vlay->addLayout(hlay);
    }
}

void StatusReportPage::updateUnusedScrollArea()
{
    QVBoxLayout *vlay = new QVBoxLayout(ui->scrollAreaWidgetContents_2);
    QHBoxLayout *titleHlay = new QHBoxLayout(ui->scrollAreaWidgetContents_2);
    QLabel *title = new QLabel(ui->scrollAreaWidgetContents_2);
    title->setText("List of Unused Rooms");
    title->setStyleSheet("* {font-size: 20px; background-color: #f2e9e4;}");
    title->setAlignment(Qt::AlignCenter);
    titleHlay->addWidget(title);
    vlay->addLayout(titleHlay);

    // Column Titles
    QLabel *rTitle = new QLabel();
    QLabel *areaTitle = new QLabel();
    QLabel *volumeTitle = new QLabel();
    rTitle->setText("Room");
    areaTitle->setText("Area");
    volumeTitle->setText("Volume");

    rTitle->setStyleSheet("* {font-size: 16px;}");
    areaTitle->setStyleSheet("* {font-size: 16px;}");
    volumeTitle->setStyleSheet("* {font-size: 16px;}");

    QHBoxLayout *columnHlay = new QHBoxLayout(ui->scrollAreaWidgetContents_2);
    columnHlay->addWidget(rTitle);
    columnHlay->addWidget(areaTitle);
    columnHlay->addWidget(volumeTitle);
    vlay->addLayout(columnHlay);

    // Fill in data from database
    QSqlQuery qry;
    if (!qry.exec("SELECT * from roomList where isOccupied=0")){
        qDebug() << qry.lastError().text();
    }

    // Creating a row in the table
    while (qry.next())
    {
        QHBoxLayout *hlay = new QHBoxLayout(ui->scrollAreaWidgetContents_2);
        QSqlRecord record = qry.record();

        // Labels
        QLabel *rLabel = new QLabel();
        QLabel *areaLabel = new QLabel();
        QLabel *volumeLabel = new QLabel();

        // Calculating area and volume
        int width = record.value("Width").toInt();
        int length = record.value("Length").toInt();
        int height = record.value("Height").toInt();
        int area = width * length;
        int volume = width * length * height;

        // Set label text
        rLabel->setText(record.value("RoomLabel").toString());
        areaLabel->setText(QString::number(area));
        volumeLabel->setText(QString::number(volume));

        // Set label stylesheet. Font 16px.
        rLabel->setStyleSheet("* {font-size: 16px;}");
        areaLabel->setStyleSheet("* {font-size: 16px;}");
        volumeLabel->setStyleSheet("* {font-size: 16px;}");

        // Adding label widget to horizontal layout container
        hlay->addWidget(rLabel);
        hlay->addWidget(areaLabel);
        hlay->addWidget(volumeLabel);

        // Adding horizontal layout to the vertical layout container
        vlay->addLayout(hlay);
    }
}

void StatusReportPage::calculateFinance()
{
    QSqlQuery paidCustomers;
    paidCustomers.exec("SELECT PaymentBalance from customerList where PaymentStatus=1");
    while (paidCustomers.next())
    {
        QSqlRecord record = paidCustomers.record();
        totalRevenue += record.value("PaymentBalance").toInt();
        totalPaid++;
        totalCustomer++;
    }

    QSqlQuery unpaidCustomers;
    unpaidCustomers.exec("SELECT PaymentBalance from customerList where PaymentStatus=0");
    while (unpaidCustomers.next())
    {
        QSqlRecord record = unpaidCustomers.record();
        totalRevenue += record.value("PaymentBalance").toInt();
        totalUnpaid++;
        totalCustomer++;
    }

    ui->totalCustomerLabel->setText("Total Customers: " + QString::number(totalCustomer));
    ui->paidCustomerLabel->setText("# of Paid Customers: " + QString::number(totalPaid));
    ui->unpaidCustomerLabel->setText("# of Unpaid Customers: " + QString::number(totalUnpaid));
    ui->revenueLabel->setText("Total Revenue: " + QString::number(totalRevenue));
}

void StatusReportPage::calculateSpace()
{
    QSqlQuery utilized;
    utilized.exec("SELECT * from roomList where isOccupied=1");
    while (utilized.next())
    {
        QSqlRecord record = utilized.record();
        int width = record.value("Width").toInt();
        int length = record.value("Length").toInt();
        int area = width * length;
        totalUtilizedArea += area;
        totalArea += area;
    }

    QSqlQuery unutilized;
    unutilized.exec("SELECT * from roomList where isOccupied=0");
    while (unutilized.next())
    {
        QSqlRecord record = unutilized.record();
        int width = record.value("Width").toInt();
        int length = record.value("Length").toInt();
        int height = record.value("Height").toInt();
        int area = width * length;
        int volume = area * height;
        totalUnusedArea += area;
        totalUnusedVolume += volume;
        totalArea += area;
    }

    capacityUtilization = ((double)totalUtilizedArea / (double)totalArea) * 100;

    // Updating Labels
    ui->areaUtilizedLabel->setText("Area Utilized: " + QString::number(totalUtilizedArea) + " SQM");
    ui->unusedAreaLabel->setText("Unused Area: " + QString::number(totalUnusedArea) + " SQM");
    ui->unusedVolumeLabel->setText("Unused Volume: " + QString::number(totalUnusedVolume) + " SQM");
    ui->capacityLabel->setText("Capacity Utilization: " + QString::number(capacityUtilization, 'f', 2) + "%");
}

void StatusReportPage::on_backButton_clicked() { hide(); }

void StatusReportPage::on_exportButton_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confimation",
                                  "Are you sure you want to get a CSV report?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) { createCSV(); }
    else { return; }
}

void StatusReportPage::createCSV()
{
    // User select a folder location to save the report
    QString folderPath = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "C:/Users/user/Desktop",
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    QString fileName = folderPath + "/ss_report.csv";
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)){ qDebug() << "Unable to open file"; }

    // Writing process
    // Customer List
    QTextStream out(&file);
    out << "Customer List\nCustomer ID,First Name,Last Name,Age,Gender,Date Joined,Rooms Rented,Rental Duration,Payment Balance,Payment Status\n";
    QSqlQuery customerList;
    customerList.exec("SELECT * from customerList");

    while (customerList.next())
    {
        QSqlRecord record = customerList.record();
        QString id = record.value("CustomerID").toString();
        QString first = record.value("FirstName").toString();
        QString last = record.value("LastName").toString();
        QString age = record.value("Age").toString();
        QString gender = record.value("Gender").toString();
        QString date = record.value("DateJoined").toString();
        QString room = record.value("RoomsRented").toString();
        QString rental = record.value("RentalDuration").toString();
        QString balance = record.value("PaymentBalance").toString();
        bool statusBool = record.value("PaymentStatus").toBool();
        QString status;
        if (statusBool) { status = "Paid"; }
        else { status = "Unpaid"; }

        out << id << "," << first << "," << last << "," << age << "," << gender << "," << date << ",";
        out << room << "," << rental << "," << balance << "," << status << "\n";
    }
    // Finance Details
    out << "Total Customers," << totalCustomer << "\n";
    out << "Number of Paid Customers," << totalPaid << "\n";
    out << "Number of Unpaid Customers," << totalUnpaid << "\n";
    out << "Total Revenue," << totalRevenue << "\n";

    // Unpaid Customer List
    out << "\nList of Unpaid Customers\n";
    out << "Customer ID,First Name,Last Name,Rooms Rented,Balance\n";

    QSqlQuery unpaidList;
    unpaidList.exec("SELECT CustomerID, FirstName, LastName, RoomsRented, PaymentBalance from customerList where PaymentStatus=0");

    while (unpaidList.next())
    {
        QSqlRecord record = unpaidList.record();
        QString id = record.value("CustomerID").toString();
        QString first = record.value("FirstName").toString();
        QString last = record.value("LastName").toString();
        QString rooms = record.value("RoomsRented").toString();
        QString balance = record.value("PaymentBalance").toString();
        out << id << "," << first << "," << last << "," << rooms << "," << balance << "\n";
    }

    // Room List
    out << "\nRoom List\n";
    out << "Room,Width,Length,Height,Price,AC,Occupied\n";

    QSqlQuery roomList;
    roomList.exec("SELECT * from roomList");

    while (roomList.next())
    {
        QSqlRecord record = roomList.record();
        QString label = record.value("RoomLabel").toString();
        QString width = record.value("Width").toString();
        QString length = record.value("Length").toString();
        QString height = record.value("Height").toString();
        QString price = record.value("Price").toString();
        QString hasAC, isOccupied;
        if (record.value("HasAC").toBool()) {hasAC = "Yes"; } else {hasAC = "No";}
        if (record.value("isOccupied").toBool()) {isOccupied = "Yes";} else {isOccupied = "No";}

        out << label << "," << width << "," << length << "," << height << "," << price << "," << hasAC << "," << isOccupied << "\n";
    }

    // Space Details
    out << "Total Utilized Space," << totalUtilizedArea << " SQM\n";
    out << "Unused Space," << totalUnusedArea << " SQM\n";
    out << "Total Area," << totalArea << " SQM\n";
    out << "Capacity Utilization Rate," << QString::number(capacityUtilization, 'f', 2) << "%\n";

    // Unused Room List
    out << "\nUnused Room List\n";
    out << "Room,Area,Volume,AC,Occupied\n";

    QSqlQuery unusedRoomList;
    unusedRoomList.exec("SELECT * from roomList where isOccupied=0");
    while (unusedRoomList.next())
    {
        QSqlRecord record = unusedRoomList.record();
        QString roomLabel = record.value("RoomLabel").toString();
        int width = record.value("Width").toInt();
        int length = record.value("Length").toInt();
        int height = record.value("Height").toInt();
        QString area = QString::number(width * length);
        QString volume = QString::number(width * length * height);
        QString hasAC, isOccupied;
        if (record.value("HasAC").toBool()) {hasAC = "Yes"; } else {hasAC = "No";}
        if (record.value("isOccupied").toBool()) {isOccupied = "Yes";} else {isOccupied = "No";}

        out << roomLabel << "," << area << "," << volume << "," << hasAC << "," << isOccupied << "\n";
    }

    file.flush();
    file.close();
    QMessageBox::information(this, "File Saved", "Report file is saved at " + fileName);

    QString action = "Report file has been created at " + fileName;
    manager.addLogEntry(action);
    emit sendLogUpdateSignal();
}
