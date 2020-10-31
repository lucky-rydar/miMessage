#ifndef DATABASECONTROLLER_H
#define DATABASECONTROLLER_H

#include <QObject>
#include <QtSql/QtSql>

class DatabaseController : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseController(QObject *parent = nullptr);

    void addUser(QString username, QString hashedPassword);
    bool userExist(QString username);
    bool isRegistered(QString username, QString hashedPassword);
private:
    QSqlDatabase connection;
signals:

};

#endif // DATABASECONTROLLER_H