#include "directorypage.h"
#include "ui_directorypage.h"
#include "facilityinfo.h"

/*
    This class / window is the directory page of the program
    It links to 4 different operations of the program
        - Check facility information
        - Check and modify customer information
        - View and overview of the whole system
        - Exiting the program
*/

DirectoryPage::DirectoryPage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DirectoryPage)
{
    ui->setupUi(this);
    manager.connectDatabase();
    fillLog();
}

DirectoryPage::~DirectoryPage()
{
    delete ui;
    delete facilityInfo;
    delete statusPage;
    delete reportPage;
    delete vlay;
    manager.db.close();
}

void DirectoryPage::on_exitButton_clicked() { QCoreApplication::quit(); }

// Opens the Facility Infomation page
void DirectoryPage::on_facilityInfoButton_clicked()
{
    facilityInfo = new FacilityInfo(this);
    facilityInfo->show();
}

// Opens the Customer Status page
void DirectoryPage::on_customerStatusButton_clicked()
{
    statusPage = new CustomerStatusPage();
    statusPage->show();
    connect(statusPage, &CustomerStatusPage::sendLogUpdateSignal, this, &DirectoryPage::updateActivityLog);
}

void DirectoryPage::on_showStatusButton_clicked()
{
    reportPage = new StatusReportPage();
    reportPage->show();
    connect(reportPage, &StatusReportPage::sendLogUpdateSignal, this, &DirectoryPage::updateActivityLog);
}

void DirectoryPage::updateActivityLog()
{
    qDeleteAll(ui->scrollAreaWidgetContents->children());
    fillLog();
}

void DirectoryPage::fillLog()
{
    vlay = new QVBoxLayout(ui->scrollAreaWidgetContents);
    QSqlQuery logs;
    logs.exec("SELECT * from logBook");

    while (logs.next())
    {
        QSqlRecord record = logs.record();
        QString year = record.value("Year").toString();
        QString month = record.value("Month").toString();
        QString day = record.value("Day").toString();
        QString time = record.value("Time").toString();
        QString action = record.value("Action").toString();

        QHBoxLayout *hlay = new QHBoxLayout(ui->scrollAreaWidgetContents);
        QLabel *yearLabel = new QLabel();
        QLabel *monthLabel = new QLabel();
        QLabel *dayLabel = new QLabel();
        QLabel *timeLabel = new QLabel();
        QLabel *actionLabel = new QLabel();

        yearLabel->setText(year);
        monthLabel->setText(month);
        dayLabel->setText(day);
        timeLabel->setText(time);
        actionLabel->setText(action);

        yearLabel->setStyleSheet("* {font-size: 16px;}");
        monthLabel->setStyleSheet("* {font-size: 16px;}");
        dayLabel->setStyleSheet("* {font-size: 16px;}");
        timeLabel->setStyleSheet("* {font-size: 16px;}");
        actionLabel->setStyleSheet("* {font-size: 16px;}");

        yearLabel->setMaximumWidth(40);
        monthLabel->setMaximumWidth(40);
        dayLabel->setMaximumWidth(30);
        timeLabel->setMaximumWidth(60);
        actionLabel->setMinimumWidth(120);

        hlay->addWidget(yearLabel);
        hlay->addWidget(monthLabel);
        hlay->addWidget(dayLabel);
        hlay->addWidget(timeLabel);
        hlay->addWidget(actionLabel);

        vlay->addLayout(hlay);
    }
}
