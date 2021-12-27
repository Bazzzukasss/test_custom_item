#include "main_window.h"
#include "ui_main_window.h"
#include <QDebug>
#include <QHeaderView>
#include "CustomItemTableModel.h"
#include "CustomItemDelegate.h"
#include "CustomItemData.h"
#include "CustomItem.h"
#include "CustomItemTypes.h"
#include "CustomItemTableMonitor.h"
#include "CustomItemTreeMonitor.h"
#include "CustomItemTableView.h"
#include "CustomItemTreeView.h"
#include "CustomItemTreeModel.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mTimer = new QTimer(this);

#if(1)
    connect(mTimer,             SIGNAL(timeout()),   this,   SLOT(slotSetItems()));
#else
    connect(mTimer,             SIGNAL(timeout()),   this,   SLOT(slotUpdateItems()));
#endif

    connect(this,               SIGNAL(signalStatusMonitorUpdate(CustomItem*)),                     ui->tableMonitor,       SLOT(slotUpdateItems(CustomItem*)));
    connect(this,               SIGNAL(signalStatusMonitorUpdate(const QList<CustomItem>&)),        ui->tableMonitor,       SLOT(slotUpdateItems(const QList<CustomItem>&)));
    connect(this,               SIGNAL(signalBuildListMonitorUpdate(CustomItem*)),                  ui->treeMonitor,        SLOT(slotUpdateItems(CustomItem*)));
    connect(ui->tableMonitor,   SIGNAL(signalCurrentChanged(CustomItemData&,int,int)),              this,                   SLOT(slotCurrentChanged(CustomItemData&,int,int)));
    connect(ui->treeMonitor,    SIGNAL(signalCurrentChanged(CustomItem*,int,int)),                  this,                   SLOT(slotCurrentChanged(CustomItem*,int,int)));
    connect(ui->mButtonPrint,   SIGNAL(clicked(bool)),                                              this,                   SLOT(slotPrintItems()));
    connect(ui->mButtonReset,   SIGNAL(clicked(bool)),                                              this,                   SLOT(slotResetItems()));
    connect(ui->mButtonDefault, SIGNAL(clicked(bool)),                                              this,                   SLOT(slotDefaultItems()));
    slotSetItems();
    mTimer->start(200);

    ui->tableMonitor->getView()->setAutoResizeToContents(false);
    ui->tableMonitor->getView()->horizontalHeader()->setStretchLastSection(false);
    ui->tableMonitor->getView()->setDragEnabled(true);
    ui->tableMonitor->getView()->setDragDropMode(QAbstractItemView::DragDrop);

    ui->treeMonitor->getView()->setAutoResizeToContents(true);
    //ui->treeMonitor->getView()->getModel()->setFlags(Qt::ItemIsEditable);

    ui->treeMonitor->getView()->setDragDropMode(QAbstractItemView::InternalMove);
    ui->treeMonitor->getView()->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->treeMonitor->getView()->setDragEnabled(true);
    ui->treeMonitor->getView()->setAcceptDrops(true);
    ui->treeMonitor->getView()->setDropIndicatorShown(true);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotSetItems()
{
    static bool stage = true;
    int vecSize = std::rand()/1000;

    std::vector<double> stdVector;
    stdVector.resize(vecSize);
    std::generate_n(stdVector.begin(),vecSize,std::rand);

    int integerValue(std::rand()/100);
    double doubleValue (std::rand()/100.0);
    bool booleanValue (stage);
    QString stringValue;
    stringValue="str:"+QString().number(integerValue);
    QStringList list({"lin1","lin2","lin3","lin4","lin5"});
    QString textValue;
    textValue="text1\ntext2\ntext3";

    SIText si_text(textValue,"default","reset");
    SIString si_string(stringValue,"default","reset");
    SIProgress si_progress(integerValue,0,100);
    SILcd si_lcd (std::rand()/300.0,SILcd::HEX);
    SIInteger si_integer(integerValue,0,500,100,200);
    SIDouble si_double(doubleValue,0,500,100,200);
    SIBoolean si_boolean(booleanValue,true,false);
    SIBars si_bars(QVector<double>().fromStdVector(stdVector));
    SIGraph si_graph(QVector<double>().fromStdVector(stdVector));
    SIStringList si_list(list,2,3,4);
    SIStringList siList({ "No","Yes" }, 0,0,0);
    CustomItem* rootItem = mRootItem = new CustomItem();

    CustomItem* item = new CustomItem("SIString:",QVariant().fromValue(si_string),"desc1");
    item->addItem( new CustomItem("SIDouble:",QVariant().fromValue(si_double),"desc2",Qt::ItemIsEditable));
    item->addItem( new CustomItem("SIInteger:",QVariant().fromValue(si_integer),"desc4",Qt::ItemIsEditable));
    item->addItem( new CustomItem("SIProgress:",QVariant().fromValue(si_progress),"desc3"));

    rootItem->addItem( new CustomItem("SIList:",QVariant().fromValue( siList ),"desc9",Qt::ItemIsEditable));
    rootItem->addItem( item );
    rootItem->addItem( new CustomItem("SIString:",QVariant().fromValue(si_string),"desc1",Qt::ItemIsEditable));
    rootItem->addItem( new CustomItem("SIDouble:",QVariant().fromValue(si_double),"desc2",Qt::ItemIsEditable));
    rootItem->addItem( new CustomItem("SIProgress:",QVariant().fromValue(si_progress),"desc3",Qt::ItemIsEditable));
    rootItem->addItem( new CustomItem("SIInteger:",QVariant().fromValue(si_integer),"desc4",Qt::ItemIsEditable));
    rootItem->addItem( new CustomItem("SILCD:",QVariant().fromValue(si_lcd),"desc5",Qt::ItemIsEditable));
    rootItem->addItem( new CustomItem("SIBoolean:",QVariant().fromValue(si_boolean),"desc6",Qt::ItemIsEditable));
    rootItem->addItem( new CustomItem("SIBars:",QVariant().fromValue( si_bars ),"desc7",Qt::ItemIsEditable));
    rootItem->addItem( new CustomItem("SIGraph:",QVariant().fromValue( si_graph ),"desc7",Qt::ItemIsEditable));
    rootItem->addItem( new CustomItem("SIList:",QVariant().fromValue( siList ),"desc9",Qt::ItemIsEditable));
    auto tItem = rootItem->addItem( new CustomItem("SIText:",QVariant().fromValue( si_text ),"desc10",Qt::ItemIsEditable) );
    rootItem->addItem( new CustomItem("Integer:",integerValue,"desc8",Qt::ItemIsEditable));
    rootItem->addItem( new CustomItem("Double:",doubleValue,"desc9",Qt::ItemIsEditable));
    rootItem->addItem( new CustomItem("Boolean:",booleanValue,"desc10",Qt::ItemIsEditable));
    rootItem->addItem( new CustomItem("String:",stringValue,"desc11",Qt::ItemIsEditable));

    rootItem->setSizes({{0,QSize(80,20)},{1,QSize(50,20)},{1,QSize(50,20)}});
    //tItem->setSize(QSize(200,80),true);
    item->setSizes({{1,QSize(200,100)}},CustomItem::ForChildren);
    QList<CustomItem> itemsList;
    itemsList   << CustomItem("SIString:",QVariant().fromValue(si_string),"desc1",Qt::ItemIsEditable)
                << CustomItem("SIDouble:",QVariant().fromValue(si_double),"desc2",Qt::ItemIsEditable)
                << CustomItem("SIProgress:",QVariant().fromValue(si_progress),"desc3",Qt::ItemIsEditable)
                << CustomItem("SIInteger:",QVariant().fromValue(si_integer),"desc4",Qt::ItemIsEditable)
                << CustomItem("SILCD:",QVariant().fromValue(si_lcd),"desc5",Qt::ItemIsEditable)
                << CustomItem("SIBoolean:",QVariant().fromValue(si_boolean),"desc6",Qt::ItemIsEditable)
                << CustomItem("SIBars:",QVariant().fromValue( si_bars ),"desc7",Qt::ItemIsEditable)
                << CustomItem("SIGraph:",QVariant().fromValue( si_graph ),"desc7",Qt::ItemIsEditable)
                << CustomItem("SIList:",QVariant().fromValue( si_list ),"desc9",Qt::ItemIsEditable)
                << CustomItem("Integer:",integerValue,"desc8",Qt::ItemIsEditable)
                << CustomItem("Double:",doubleValue,"desc9",Qt::ItemIsEditable)
                << CustomItem("Boolean:",booleanValue,"desc10",Qt::ItemIsEditable)
                << CustomItem("String:",stringValue,"desc11",Qt::ItemIsEditable);

    emit signalStatusMonitorUpdate(itemsList);
    emit signalBuildListMonitorUpdate(rootItem);

    stage = !stage;
}

void MainWindow::slotUpdateItems()
{
    static bool stage = true;
    int vecSize = std::rand()/1000;

    std::vector<double> stdVector;
    stdVector.resize(vecSize);
    std::generate_n(stdVector.begin(),vecSize,std::rand);

    int integerValue(std::rand()/100);
    double doubleValue (std::rand()/100.0);
    bool booleanValue (stage);
    QString stringValue;
    stringValue="str:"+QString().number(integerValue);
    QString textValue;
    textValue="text1\ntext2\ntext3";

    SIString si_string(stringValue);
    SIText si_text(textValue);
    SIProgress si_progress(integerValue,0,100);
    SILcd si_lcd (std::rand()/300.0,SILcd::HEX);
    SIInteger si_integer(integerValue,0,500);
    SIDouble si_double(doubleValue,10,90);
    SIBoolean si_boolean(booleanValue);
    SIBars si_bars(QVector<double>().fromStdVector(stdVector));
    SIGraph si_graph(QVector<double>().fromStdVector(stdVector));

    *mRootItem->getItems()[4] = CustomItem("SIIntegerU:",QVariant().fromValue(si_integer),"desc4U");
    ui->treeMonitor->refresh();

}

void MainWindow::slotCurrentChanged(CustomItem* item, int row, int column)
{
    qDebug()<<"row="<<row<<"column="<<column<<"data:"<<item->getData().getProperty("name").mValue;
}
void MainWindow::slotCurrentChanged(CustomItemData& data, int row, int column)
{
    qDebug()<<"row="<<row<<"column="<<column<<"data:"<<data.getProperty("name").mValue;
}

void MainWindow::slotPrintItems()
{
    for(CustomItem* item : mRootItem->getItems())
        qDebug()<<item->getPropertyValue(1);
}

void MainWindow::slotResetItems()
{
    mRootItem->resetPropertyValue(1,CustomItem::ForALL);
    ui->treeMonitor->refresh();
}

void MainWindow::slotDefaultItems()
{
    mRootItem->setPropertyToDefault(1,CustomItem::ForALL);
    ui->treeMonitor->refresh();
}
