#ifndef GROUPMEMBER_H
#define GROUPMEMBER_H

#include <QObject>

class GroupMember : public QObject
{
    Q_OBJECT
public:
    explicit GroupMember(QObject *parent = nullptr);

    int groupId;
    QString username;
signals:

};

#endif // GROUPMEMBER_H
