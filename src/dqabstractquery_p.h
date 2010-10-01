#ifndef DQABSTRACTQUERY_P_H
#define DQABSTRACTQUERY_P_H

#include <QSqlQuery>
#include "dqconnection.h"
#include "dqmodel.h"
#include "dqmodelmetainfo.h"
#include "dqwhere.h"
#include "dqexpression.h"

/// DQAbstractQuery private data

class DQAbstractQueryPriv : public QSharedData {
public:
    inline DQAbstractQueryPriv() {
        metaInfo = 0;
        limit = -1; // No limit
    }

    DQConnection connection;

    /// The function to be called on result column.
    QString func;

    DQModelMetaInfo *metaInfo;
    int limit;

    QSqlQuery query;

    DQExpression expression;

    /// select(fields)
    QStringList fields;
};

#endif // DQABSTRACTQUERY_P_H
