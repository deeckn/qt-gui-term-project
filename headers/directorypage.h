#ifndef DIRECTORYPAGE_H
#define DIRECTORYPAGE_H

#include <QMainWindow>
#include <databaseManager.h>
#include <facilityinfo.h>
#include <customerstatuspage.h>
#include <statusreportpage.h>

namespace Ui {
class DirectoryPage;
}

class DirectoryPage : public QMainWindow
{
    Q_OBJECT

public:
    explicit DirectoryPage(QWidget *parent = nullptr);
    ~DirectoryPage();

private slots:
    // Qt Built-in slots
    void on_exitButton_clicked();
    void on_facilityInfoButton_clicked();
    void on_customerStatusButton_clicked();
    void on_showStatusButton_clicked();

    // User made slots
    void updateActivityLog();

private:
    Ui::DirectoryPage *ui;
    DatabaseManager manager;
    FacilityInfo *facilityInfo;
    CustomerStatusPage *statusPage;
    StatusReportPage *reportPage;
    QVBoxLayout *vlay;

    void fillLog();
};

#endif // DIRECTORYPAGE_H
