#ifndef FACILITYINFO2_H
#define FACILITYINFO2_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlRecord>
#include <customer.h>
#include <room.h>
#include <roominfobox.h>
#include <databaseManager.h>

namespace Ui {
class FacilityInfo2;
}

class FacilityInfo2 : public QDialog
{
    Q_OBJECT

public:
    explicit FacilityInfo2(QWidget *parent = nullptr);
    ~FacilityInfo2();

private slots:
    // Qt Built-in slots
    void on_backButton_clicked();
    void on_c01_button_clicked();
    void on_c02_button_clicked();
    void on_c03_button_clicked();
    void on_c04_button_clicked();
    void on_c05_button_clicked();
    void on_c06_button_clicked();
    void on_d01_button_clicked();
    void on_d02_button_clicked();

private:
    Ui::FacilityInfo2 *ui;
    RoomInfoBox *infoBox;
    DatabaseManager manager;

    void updateRoomOccupancy();
    void showRoomInfo(Room r);
    void roomInfoFromLabel(QString roomLabel);
};

#endif // FACILITYINFO2_H
