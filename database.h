#ifndef DATABASE_H
#define DATABASE_H

#include "DataBaseRow.h"

#include <QObject>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QFile>
#include <QDate>
#include <QDebug>
#include <QModelIndex>

//Directives table names, table fields and databases
#define DATABASE_HOSTNAME   "ExampleDataBase"
#define DATABASE_NAME       "DataBase.db"

#define TABLE                   "TableExample"
#define TABLE_YEAR              "Year"
#define TABLE_MAKE              "Make"
#define TABLE_MODEL             "Model"
#define TABLE_CLASS             "Class"
#define TABLE_TYPECOUPE          "TypeCoupe"

class DataBase : public QObject
{
    Q_OBJECT
public:
    explicit DataBase(QObject *parent = 0);
    ~DataBase();
    //methods for working directly with the class
    //connect to a database and insert data into a table
    bool insertIntoTable(const QVariantList &data);
    bool setDefaultDataToTable(const QString &jsonName);
    void insertRow(const DataBaseRow &r);

    void connectToDataBase();
    bool dataBaseExists;
    QModelIndex selectedIndex;

private:
    // the database object
    QSqlDatabase    db;

private:
    //internal methods for working with the database
    bool openDataBase();
    bool restoreDataBase();
    void closeDataBase();
    bool createTable();
};

#endif // DATABASE_H
