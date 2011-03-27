#ifndef DQEXPRESSION_H
#define DQEXPRESSION_H

#include <dqwhere.h>
#include <QMap>

/// Construct an expression based on DQWhere clause
/**
   @remarks It is a private in implementation. User should not use this class.
 */
class DQExpression
{
public:
    DQExpression();
    DQExpression(DQWhere where);

    /// Get the expression in string
    QString string();

    /// A map of values to find with QSqlQuery
    QMap<QString,QVariant> bindValues();

    bool isNull();

private:
    void process(DQWhere& where);

    /// A recusrive verison of process()
    QString _process(DQWhere& where);

    /// A recursive function to prcess the operand in DQWhere
    QString _process(QVariant v);


    /// The string expression
    QString m_string;

    QMap<QString,QVariant> m_values;

    int m_num;
    bool m_null;

};

#endif // DQEXPRESSION_H
