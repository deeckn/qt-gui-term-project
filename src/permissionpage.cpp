#include "permissionpage.h"
#include "ui_permissionpage.h"

/*
    This class / page is used as a protection layer before allowing the database to be modified
    The admin's username and password must be correctly inputted to continue the opertion
    A successful verification will emit the signal - accessGranted() to the connected slots using it
*/

PermissionPage::PermissionPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PermissionPage)
{
    ui->setupUi(this);
    ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
}

PermissionPage::~PermissionPage()
{
    delete ui;
}

void PermissionPage::on_pushButton_clicked()
{
    // Retrieve data from user input
    QString usernameEntry = ui->usernameLineEdit->text();
    QString passwordEntry = ui->passwordLineEdit->text();

    // Searching match
    QSqlQuery qry;
    QString query = "SELECT password FROM userPass where username='"+usernameEntry+"' and password='"+passwordEntry+"'";
    bool access{false};
    if (qry.exec(query))
    {
        int count{0};
        while(qry.next()){ count++; }
        if (count == 1) { access = true; }
        else {access = false;}
    }

    if (access)
    {
        emit accessGranted();
        QMessageBox::information(this, "Permission Request", "Access Granted");
        close();
    }
    else { QMessageBox::information(this, "Log In", "Wrong Username / Password"); }
}
