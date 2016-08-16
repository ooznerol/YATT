#ifndef COMMON_H
#define COMMON_H


struct TransactionStruct
{
    QString date;
    QString dateUTC;
    QString instrumentName;
    QString period;
    QString profitAndLoss;
    QString transactionType;
    QString reference;
    float openLevel;
    float closeLevel;
    float size;
    QString currency;
    bool cashTransaction;
};

#endif // COMMON_H
