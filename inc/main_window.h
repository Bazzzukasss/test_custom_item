#pragma once

#include <QMainWindow>
#include <QTimer>

#include "CustomItemClasses.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void slotSetItems();
    void slotUpdateItems();
    void slotCurrentChanged(CustomItem* item,int row,int column);
    void slotCurrentChanged(CustomItemData &data, int row, int column);
    void slotPrintItems();
    void slotResetItems();
    void slotDefaultItems();

signals:
    void signalStatusMonitorUpdate(const QList<CustomItem>& items);
    void signalStatusMonitorUpdate(CustomItem* rootItem);
    void signalBuildListMonitorUpdate(CustomItem* rootItem);

private:
    void initModel();
    Ui::MainWindow *ui;
    QTimer* mTimer;
    CustomItem* mRootItem;
};
