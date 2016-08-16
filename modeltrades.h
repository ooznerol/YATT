#ifndef MODELTRADES_H
#define MODELTRADES_H

#include <QAbstractTableModel>

#include "common.h"

class modelTrades : public QAbstractTableModel
{
    

public:
    modelTrades(QObject *parent);


    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
     // Header:
   QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;


    void addOneEntry( TransactionStruct ListData );
    QList <TransactionStruct> ListOfTrades;

public slots:
    void Clear();
private:
};

#endif // MODELTRADES_H
