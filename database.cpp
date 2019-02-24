#include "database.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>


DataBase::DataBase(QObject *parent) :
    QObject(parent),
    dataBaseExists(false){}

DataBase::~DataBase(){}

void DataBase::connectToDataBase() {
    //if database exists it will be opened and restored otherwise
    dataBaseExists = QFile("DataBase/" DATABASE_NAME).exists();
    if(!dataBaseExists){
        this->restoreDataBase();
    } else {
        this->openDataBase();
    }
}

bool DataBase::restoreDataBase() {
    if(this->openDataBase()) {
        if(!this->createTable()) {
            return false;
        } else {
            return true;
        }
    } else {
        qDebug() << "Could not restore database";
        return false;
    }
    return false;
}

bool DataBase::openDataBase() {
    //opening the database by specific path
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName(DATABASE_HOSTNAME);
    db.setDatabaseName("DataBase/" DATABASE_NAME);
    if(db.open()){
        return true;
    } else {
        return false;
    }
}

void DataBase::closeDataBase() {
    db.close();
}

bool DataBase::createTable() {
    QSqlQuery query;
    if(!query.exec( "CREATE TABLE " TABLE " ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            TABLE_YEAR      " INTEGER(4)            NOT NULL,"
                            TABLE_MAKE      " VARCHAR(255)          NOT NULL,"
                            TABLE_MODEL     " VARCHAR(255)          NOT NULL,"
                            TABLE_CLASS     " VARCHAR(255)          NOT NULL,"
                            TABLE_TYPECOUPE  " VARCHAR(255)          NOT NULL"
                        " )"
                    )){
        qDebug() << "DataBase: error of create " << TABLE;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

bool DataBase::insertIntoTable(const QVariantList &data) {
    //QVariantList contains data to inserting to table using SQL query
    QSqlQuery query;   
    query.prepare("INSERT INTO " TABLE " ( " TABLE_YEAR  ", "
                                             TABLE_MAKE  ", "
                                             TABLE_MODEL ", "
                                             TABLE_CLASS ", "
                                             TABLE_TYPECOUPE " ) "
                  "VALUES (:Year, :Model, :Make, :Class, :TypeCoupe )");
    query.bindValue(":Year",        data[0].toInt());
    query.bindValue(":Make",        data[1].toString());
    query.bindValue(":Model",       data[2].toString());
    query.bindValue(":Class",       data[3].toString());
    query.bindValue(":TypeCoupe",    data[4].toString());
    // query is performed with method exec()
    if(!query.exec()){
        qDebug() << "error insert into " << TABLE;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}
bool DataBase::setDefaultDataToTable(const QString &jsonName) {
    //json file "jsonName" contains default data
    QJsonParseError  parseError;
    //converting .json file to QFile
    QFile configFile(jsonName);

    if(configFile.open(QIODevice::ReadWrite)) {
        //reading file data to QString
         QString inputData = configFile.readAll();
         //converting QString data to QJsonDocument
         QJsonDocument jsonDoc = QJsonDocument::fromJson(inputData.toUtf8(), &parseError);

         if(parseError.error == QJsonParseError::NoError) {
             //getting QJsonObject all data of json file
             QJsonObject jo_allData = jsonDoc.object();
             //checking the existence of the key "cars" in jo_allData
             if(jo_allData.contains("cars") ) {
                 //getting value the value of the key "cars" and converting it to QJsonArray
                 QJsonArray jo_cars = jo_allData.begin().value().toArray();

                 for (int index = 0; index < jo_cars.size(); ++index) {
                     DataBaseRow dbRow;
                     //getting QJsonObject element by infex of the array
                     QJsonObject jo_arrayElement = jo_cars[index].toObject();
                     //writing the values of each key into the structure DataBaseRow
                     dbRow.year = jo_arrayElement["Year"].toInt();
                     dbRow.make = jo_arrayElement["Make"].toString();
                     dbRow.model = jo_arrayElement["Model"].toString();
                     dbRow.autoClass = jo_arrayElement["Class"].toString();
                     dbRow.typeCoupe = jo_arrayElement["Type Coupe"].toString();
                     //inserting the row into table
                     insertRow(dbRow);
                 }
                 return true;

             } else {
                qDebug() << "Json oobject does not contain cars";
                return false;
             }
         } else {
             qDebug() << "Parse Error: " << parseError.errorString();
              return false;
         }

        } else {
            qDebug() << "File is not opened";
            return false;
        }

    return false;
}

void DataBase::insertRow(const DataBaseRow &r)
{
    QVariantList data;

    data.append(r.year);
    data.append(r.make);
    data.append(r.model);
    data.append(r.autoClass);
    data.append(r.typeCoupe);

    insertIntoTable(data);
}
