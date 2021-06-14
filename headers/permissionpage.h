#ifndef PERMISSIONPAGE_H
#define PERMISSIONPAGE_H

#include <QtCore>
#include <QDialog>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QMessageBox>

namespace Ui {
class PermissionPage;
}

class PermissionPage : public QDialog
{
    Q_OBJECT

public:
    explicit PermissionPage(QWidget *parent = nullptr);
    ~PermissionPage();

signals:
    void accessGranted();

private slots:
    void on_pushButton_clicked();

private:
    Ui::PermissionPage *ui;
};

#endif // PERMISSIONPAGE_H
