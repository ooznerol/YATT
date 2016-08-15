#include "modeltrades.h"

#include "QDebug"

//#define TEST
enum{
    columndate,
    columndateUTC,
    columninstrumentName,
    columnperiod,
    columnprofitAndLoss,
    columntransactionType,
    columnreference,
    columnopenLevel,
    columcloseLevel,
    columsize,
    columcurrency,
    columcashTransaction,



    column_MAX
};

modelTrades::modelTrades(QObject *parent)
    : QAbstractItemModel(parent)
{
}
#
QVariant modelTrades::headerData(int section, Qt::Orientation orientation, int role) const
{

qDebug() <<  "headerDataorientation ="<<  orientation;
#ifdef TEST

#else

if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section)
        {
            case columndate:
                return QString("date");
            case columndateUTC:
                return QString("dateUTC");
            case columninstrumentName:
                return QString("period");
            case columnprofitAndLoss:
                return QString("profitAndLoss");
            case columntransactionType:
                return QString("transactionType");
            case columnreference:
                return QString("reference");
            case columnopenLevel:
                return QString("openLevel");
            case columcloseLevel:
                return QString("closeLevel");
            case columsize:
                return QString("size");
            case columcurrency:
                return QString("currency");
            case columcashTransaction:
                return QString("cashTransaction");

            default:
                return QVariant();
        }
    }
    return QVariant();


#endif
}


QModelIndex modelTrades::index(int row, int column, const QModelIndex &parent) const
{
    // FIXME: Implement me!
}

QModelIndex modelTrades::parent(const QModelIndex &index) const
{
    // FIXME: Implement me!
}

int modelTrades::rowCount(const QModelIndex &parent) const
{
//    if (!parent.isValid())
//        return 0;
//    else
    int rows ;


#ifdef TEST
   rows = 2;
#else
   rows = ListOfTrades.size();
#endif

   qDebug() <<  "rows="<<  rows;
   return rows;

    // FIXME: Implement me!
}

int modelTrades::columnCount(const QModelIndex &parent) const
{
//    if (!parent.isValid())
//        return 0;
//    else

    int cols;

#ifdef TEST
   cols = 3;
#else
   cols = column_MAX;
#endif

    qDebug() <<  "cols=" << cols;
    return cols;


    // FIXME: Implement me!
}

QVariant modelTrades::data(const QModelIndex &index, int role) const
{
#ifdef TEST
    if (role == Qt::DisplayRole)
        {
           return QString("Row%1, Column%2")
                       .arg(index.row() + 1)
                       .arg(index.column() +1);
        }
#else
    if (!index.isValid())
        return QVariant();
    else
        if (role == Qt::DisplayRole)
        {

                switch (index.row())
                {
                    case columndate:
                        return QString("date");
                    case columndateUTC:
                        return QString("dateUTC");
                    case columninstrumentName:
                        return QString("period");
                    case columnprofitAndLoss:
                        return QString("profitAndLoss");
                    case columntransactionType:
                        return QString("transactionType");
                    case columnreference:
                        return QString("reference");
                    case columnopenLevel:
                        return QString("openLevel");
                    case columcloseLevel:
                        return QString("closeLevel");
                    case columsize:
                        return QString("size");
                    case columcurrency:
                        return QString("currency");
                    case columcashTransaction:
                        return QString("cashTransaction");

                }

        }

    // FIXME: Implement me!
#endif



        return QVariant();

}



void modelTrades::addOneEntry(TransactionStruct Data)
{

        beginInsertRows(QModelIndex(), ListOfTrades.size(), ListOfTrades.size() );

        ListOfTrades.append(Data);

        endInsertRows();


}
