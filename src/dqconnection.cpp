#include <QtCore>
#include <QSqlDatabase>
#include <QCoreApplication>
#include <QSqlError>

#include "dqmodel.h"
#include "dqconnection.h"
#include "dqsqlitestatement.h"
#include "dqsql.h"

class DQConnectionPriv : public QSharedData
{
  public:
    DQSql m_sql;

    /// Registered modeles
    QList<DQModelMetaInfo*> m_models;

    /// The last query being used.
    QSqlQuery lastQuery;
};

/// The default connection shared for all objects
DQConnection m_defaultConnection;

DQConnection::DQConnection()
{
    d = new DQConnectionPriv();
}

DQConnection::DQConnection(const DQConnection& other) : d(other.d){
}

DQConnection & DQConnection::operator=(const DQConnection &rhs){
    if (this != &rhs)
        d.operator=(rhs.d);
    return *this;
}

DQConnection::~DQConnection(){
}

bool DQConnection::open(QSqlDatabase db){
    Q_ASSERT(db.isOpen());

    if (db.driverName() != "QSQLITE") {
        qWarning() << "Only QSQLITE dirver is supported.";
        return false;
    }

    if (!m_defaultConnection.isOpen()
        && this != &m_defaultConnection
        ) {

        d.operator = (m_defaultConnection.d); // It become the default connection

    }

    d->m_sql.setStatement(new DQSqliteStatement());
    d->m_sql.setDatabase(db);

    return true;
}

bool DQConnection::isOpen(){
    return d->m_sql.database().isOpen();
}

void DQConnection::close(){
    d->m_sql.setDatabase(QSqlDatabase());
}

bool DQConnection::addModel(DQModelMetaInfo* metaInfo){
    bool res = false;
    if (!metaInfo) {
        return res;
    }

    if (!d->m_models.contains(metaInfo)) {
        d->m_models << metaInfo;
        res = true;
    }
    return res;
}

DQConnection DQConnection::defaultConnection(){
    return m_defaultConnection;
}

bool DQConnection::createTables(){

    bool res = true;
    foreach (DQModelMetaInfo* info ,d->m_models) {

        if (!d->m_sql.exists(info)) {

            if (!d->m_sql.createTableIfNotExists(info)){
                qWarning() << QString("DQConnection::createTables() - Failed to create table for %1 . Error : %2").arg(info->className())
                        .arg( d->m_sql.lastQuery().lastError().text());
                qWarning() << d->m_sql.lastQuery().lastQuery();
                res = false;
                d->lastQuery = d->m_sql.lastQuery();
                break;
            }

            DQSharedList initialData = info->initialData();
            int n = initialData.size();
            for (int i = 0 ; i< n;i++) {
                initialData.at(i)->save();
            }
        }
    }

    return res;
}

bool DQConnection::dropTables() {
    bool res = true;

    foreach (DQModelMetaInfo* info ,d->m_models) {
        if (!d->m_sql.exists(info))
            continue;

        if (!d->m_sql.dropTable(info) ) {
            res = false;
            d->lastQuery = d->m_sql.lastQuery();
            break;
        }

    }

    return res;
}

DQSql& DQConnection::sql(){
    return d->m_sql;
}

QSqlQuery DQConnection::query(){
    return d->m_sql.query();
}
