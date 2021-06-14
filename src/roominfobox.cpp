#include "roominfobox.h"
#include "ui_roominfobox.h"

RoomInfoBox::RoomInfoBox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RoomInfoBox)
{
    ui->setupUi(this);
}

RoomInfoBox::~RoomInfoBox() {
    delete ui;
}

void RoomInfoBox::updateInfo(){
    ui->infoBox->setText(currentRoom->getAllData());
}

void RoomInfoBox::setRoom(Room *r)
{
    currentRoom = r;
    updateInfo();
}

void RoomInfoBox::on_backButton_clicked(){ hide(); }
