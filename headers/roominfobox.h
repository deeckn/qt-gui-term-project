#ifndef ROOMINFOBOX_H
#define ROOMINFOBOX_H

#include <QDialog>
#include <QtCore>
#include <room.h>

namespace Ui {
class RoomInfoBox;
}

class RoomInfoBox : public QDialog
{
    Q_OBJECT

public:
    explicit RoomInfoBox(QWidget *parent = nullptr);
    ~RoomInfoBox();
    void setRoom(Room *r);

private slots:
    void on_backButton_clicked();

private:
    Ui::RoomInfoBox *ui;
    Room *currentRoom;
    void updateInfo();
};

#endif // ROOMINFOBOX_H
