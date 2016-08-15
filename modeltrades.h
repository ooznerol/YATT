#ifndef MODELTRADES_H
#define MODELTRADES_H

#include <QAbstractItemModel>

#include "common.h"

class modelTrades : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit modelTrades(QObject *parent = 0);

#if 1
    // Header:
   QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
   QModelIndex parent(const QModelIndex &index) const override;
#endif
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    //bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;


    void addOneEntry( TransactionStruct ListData );
    QList <TransactionStruct> ListOfTrades;

private:
};

#endif // MODELTRADES_H
