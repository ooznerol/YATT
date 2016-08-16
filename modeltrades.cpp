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

modelTrades::modelTrades(QObject *parent) : QAbstractTableModel(parent)
{
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

   //qDebug() <<  "rows="<<  rows;
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

    //qDebug() <<  "cols=" << cols;
    return cols;


    // FIXME: Implement me!
}

QVariant modelTrades::headerData(int section, Qt::Orientation orientation, int role) const
{

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
                return QString("instrumentName");
            case columnperiod:
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

                //qDebug() <<  "index.row()=" << index.row() <<"index.cols()=" << index.cols();
                switch (index.column())
                {
                    case columndate:
                        return QString(ListOfTrades.at(index.row()).date);
                    case columndateUTC:
                        return QString(ListOfTrades.at(index.row()).dateUTC);

                    case columnperiod:
                        return QString(ListOfTrades.at(index.row()).period);
                    case columninstrumentName:
                        return QString(ListOfTrades.at(index.row()).instrumentName);
                    case columnprofitAndLoss:
                        return QString(ListOfTrades.at(index.row()).profitAndLoss);
                    case columntransactionType:
                        return QString(ListOfTrades.at(index.row()).transactionType);
                    case columnreference:
                        return QString(ListOfTrades.at(index.row()).reference);
                    case columnopenLevel:
                        return QString(QString::number(ListOfTrades.at(index.row()).openLevel));
                    case columcloseLevel:
                        return QString(QString::number(ListOfTrades.at(index.row()).closeLevel));
                    case columsize:
                        return QString(QString::number(ListOfTrades.at(index.row()).size));
                    case columcurrency:
                        return QString(ListOfTrades.at(index.row()).currency);
                    case columcashTransaction:
                        return QString(ListOfTrades.at(index.row()).cashTransaction);

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

void modelTrades::Clear(  )
{
    beginResetModel();
    ListOfTrades.clear();
    endResetModel();
}
