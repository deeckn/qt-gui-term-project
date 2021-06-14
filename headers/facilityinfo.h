#ifndef FACILITYINFO_H
#define FACILITYINFO_H

#include <QtCore>
#include <QDialog>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlRecord>
#include <facilityinfo2.h>
#include <databaseManager.h>
#include <roominfobox.h>
#include <customer.h>
#include <room.h>

namespace Ui {
class FacilityInfo;
}

class FacilityInfo : public QDialog
{
    Q_OBJECT

public:
    explicit FacilityInfo(QWidget *parent = nullptr);
    ~FacilityInfo();

private slots:
    // Qt Built-in slots
    void on_backButton_clicked();
    void on_to2fButton_clicked();
    void on_a01_button_clicked();
    void on_a02_button_clicked();
    void on_a03_button_clicked();
    void on_a04_button_clicked();
    void on_a05_button_clicked();
    void on_a06_button_clicked();
    void on_b01_button_clicked();
    void on_b02_button_clicked();
    void on_b03_button_clicked();
    void on_b04_button_clicked();

private:
    Ui::FacilityInfo *ui;
    DatabaseManager manager;
    FacilityInfo2 *facilityinfo2;
    RoomInfoBox *infoBox;

    void updateRoomOccupancy();
    void showRoomInfo(Room r);
    void roomInfoFromLabel(QString roomLabel);
};

#endif // FACILITYINFO_H
