#ifndef STATUSREPORTPAGE_H
#define STATUSREPORTPAGE_H

#include <QDialog>
#include <QFile>
#include <QFileDialog>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <databaseManager.h>
#include <permissionpage.h>

namespace Ui {
class StatusReportPage;
}

class StatusReportPage : public QDialog
{
    Q_OBJECT

public:
    explicit StatusReportPage(QWidget *parent = nullptr);
    ~StatusReportPage();

signals:
    void sendLogUpdateSignal();

private slots:
    void on_backButton_clicked();
    void on_exportButton_clicked();
    void createCSV();

private:
    Ui::StatusReportPage *ui;
    DatabaseManager manager;
    int totalCustomer{0}, totalPaid{0}, totalUnpaid{0}, totalRevenue{0};
    int totalUtilizedArea{0}, totalUnusedArea{0}, totalUnusedVolume{0}, totalArea{0};
    double capacityUtilization;

    void updateUnpaidScrollArea();
    void updateUnusedScrollArea();
    void calculateFinance();
    void calculateSpace();
};

#endif // STATUSREPORTPAGE_H
