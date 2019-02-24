#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    db = new DataBase();
    db->connectToDataBase();
    if(!db->dataBaseExists) {
        db->setDefaultDataToTable("cars.json");
    }


     this->setupModel(TABLE,
                     QStringList() << trUtf8("id")
                                   << trUtf8("Year")
                                   << trUtf8("Model")
                                   << trUtf8("Make")
                                   << trUtf8("Class")
                                   << trUtf8("Type Coupe")
               );
     this->createUI();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setupModel(const QString &tableName, const QStringList &headers) {
    //initialization of the data representation model by table name
    model = new QSqlTableModel(this);
    model->setTable(tableName);

    //setting column names of the table
    for(int i = 0, j = 0; i < model->columnCount(); i++, j++){
        model->setHeaderData(i,Qt::Horizontal,headers[j]);
    }
}

void MainWindow::createUI() {
    // setting model for TableView
    ui->tableView->setModel(model);
    //hiding column with "id"
    ui->tableView->setColumnHidden(0, true);
    // allow row selection
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    // set the mode to select only one line
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    // set column size by content
    ui->tableView->resizeColumnsToContents();
    //set column resize mode to stretch columns by table size
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //making the data sampling from the table
    model->select();
}

void MainWindow::on_addPushButton_clicked() {
    DataBaseRow dbRow;

    dbRow.year = 0;
    dbRow.make = "";
    dbRow.model = "";
    dbRow.autoClass = "";
    dbRow.typeCoupe = "";

    db->insertRow(dbRow);
    model->select();
    ui->tableView->setModel(model);
}


void MainWindow::on_deletePushButton_clicked() {
    model->removeRow(db->selectedIndex.row());
    model->select();
    ui->tableView->setModel(model);
}

void MainWindow::on_tableView_clicked(const QModelIndex &index) {
    db->selectedIndex = index;
}
