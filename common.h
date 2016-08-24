#ifndef COMMON_H
#define COMMON_H

#define LOG_DEBUG_FULL      0x01
#define LOG_DEBUG           0x02
#define LOG_NORMAL          0x04
#define LOG_WARNING         0x08
#define LOG_ERROR           0x10
#define LOG_SUCCESS         0x20

struct TransactionStruct
{
    QString date;
    QString dateUTC;
    QString instrumentName;
    QString period;
    QString profitAndLoss;
    QString transactionType;
    QString reference;
    double openLevel;
    double closeLevel;
    double deltaPoint;
    QString size;
    QString currency;
    bool cashTransaction;
};


struct AccountInfo
{
    QString CashAvailable;
    QString Balance;
    QString ProfitLoss;
    QString ID;
};


enum RequestType{
    NoRequest,
    RequestConnection,
    RequestGetTransactionList,
    RequestConnectionLightStreamer,

};
#endif // COMMON_H
