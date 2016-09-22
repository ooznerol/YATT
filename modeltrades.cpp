//    This file is part of YATT.

//    YATT is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.

//    YATT is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.

//    You should have received a copy of the GNU General Public License
//    along with YATT.  If not, see <http://www.gnu.org/licenses/>. 2


#include "modeltrades.h"

#include "QDebug"
#include "QColor"

//#define TEST
enum{
    columndate,
    columndateUTC,
    columninstrumentName,
    //columnperiod,
    columnprofitAndLoss,
    //columntransactionType,
    //columnreference,
    columnopenLevel,
    columcloseLevel,
    columdeltapoint,
    columsize,
    //columcurrency,
    //columcashTransaction,



    column_MAX
};

modelTrades::modelTrades(QObject *parent) : QAbstractTableModel(parent)
{

    precision = 1;
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
//            case columnperiod:
//                return QString("period");
            case columnprofitAndLoss:
                return QString("profitAndLoss");
//            case columntransactionType:
//                return QString("transactionType");
//            case columnreference:
//                return QString("reference");
            case columnopenLevel:
                return QString("openLevel");
            case columcloseLevel:
                return QString("closeLevel");
            case columdeltapoint:
                return QString("delta");
            case columsize:
                return QString("size");
//            case columcurrency:
//                return QString("currency");
//            case columcashTransaction:
//                return QString("cashTransaction");

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
    {
        if (role == Qt::DisplayRole)
        {

                //qDebug() <<  "index.row()=" << index.row() <<"index.cols()=" << index.cols();
                switch (index.column())
                {
                    case columndate:
                        return QString(ListOfTrades.at(index.row()).date);
                    case columndateUTC:
                        return QString(ListOfTrades.at(index.row()).dateUTC);

//                    case columnperiod:
//                        return QString(ListOfTrades.at(index.row()).period);
                    case columninstrumentName:
                        return QString(ListOfTrades.at(index.row()).instrumentName);
                    case columnprofitAndLoss:
                        return QString(ListOfTrades.at(index.row()).profitAndLoss);
//                    case columntransactionType:
//                        return QString(ListOfTrades.at(index.row()).transactionType);
//                    case columnreference:
//                        return QString(ListOfTrades.at(index.row()).reference);
                    case columnopenLevel:
                        //return QString(ListOfTrades.at(index.row()).openLevel);
                        return QString(QString::number(ListOfTrades.at(index.row()).openLevel,'f',precision));
                    case columcloseLevel:
                        //return QString(ListOfTrades.at(index.row()).closeLevel);
                        return QString(QString::number(ListOfTrades.at(index.row()).closeLevel,'f',precision));
                    case columdeltapoint:

                    //return QString(ListOfTrades.at(index.row()).deltaPoint);
                        return QString(QString::number(ListOfTrades.at(index.row()).deltaPoint,'f',precision));
                    case columsize:
                        //return QString(QString::number(ListOfTrades.at(index.row()).size));
                        return QString((ListOfTrades.at(index.row()).size));
//                    case columcurrency:
//                        return QString(ListOfTrades.at(index.row()).currency);
//                    case columcashTransaction:
//                        return QString(ListOfTrades.at(index.row()).cashTransaction);

                }

        }
        if (role == Qt::BackgroundColorRole)
        {
            if ((ListOfTrades.at(index.row()).profitAndLoss).contains("-"))
            {
                return QVariant(QColor(Qt::red));
            }
            else if ((ListOfTrades.at(index.row()).closeLevel) == (ListOfTrades.at(index.row()).openLevel))
            {
                return QVariant(QColor(Qt::gray));
            }
            else
            {
                return QVariant(QColor(Qt::green));
            }

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
