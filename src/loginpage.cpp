#include "loginpage.h"
#include "ui_loginpage.h"

MainPage::MainPage(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainPage)
{
    ui->setupUi(this);
    ui->passwordEntry->setEchoMode(QLineEdit::Password);
    manager.connectDatabase();
}

MainPage::~MainPage() { delete ui; manager.db.close(); }

void MainPage::on_pushButton_clicked() { verifyLogIn(); }

void MainPage::verifyLogIn() {
    // Retrieve data from user input
    QString usernameEntry = ui->usernameEntry->text();
    QString passwordEntry = ui->passwordEntry->text();

    // Searching match
    QSqlQuery qry;
    QString query = "SELECT password FROM userPass where username='"+usernameEntry+"' and password='"+passwordEntry+"'";
    bool accessGranted{false};
    if (qry.exec(query)) {
        int count{0};
        while(qry.next()){ count++; }
        if (count == 1) { accessGranted = true; }
        else { accessGranted = false; }
    }

    // Adding login access to log book
    if (accessGranted) {
        QString action = usernameEntry + " has logged in to the system";
        manager.addLogEntry(action);
    }

    // Response to user
    if (accessGranted) {
        QMessageBox::information(this, "Log In", "Log in successful: Welcome " + usernameEntry);
        close();
        directoryPage = std::make_unique<DirectoryPage>(this);
        directoryPage->show();
    } else { QMessageBox::information(this, "Log In", "Wrong Username/Password"); }
}
