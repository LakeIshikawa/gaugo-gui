#include "uctnodestable.h"
#include "boardview.h"

UctNodesTable::UctNodesTable(QWidget *parent) :
    QTableWidget(parent)
{
    setSortingEnabled(true);
}

void UctNodesTable::setUCTTree(QList<UCTNode> treePos)
{
    setSortingEnabled(false);

    clearContents();
    setRowCount(treePos.length());

    int row = 0;
    foreach( const UCTNode &node, treePos ){
        QString moveName = BoardView::pointToName(node.pos);
        QTableWidgetItem *newItem = new QTableWidgetItem(moveName);
        newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);
        setItem(row, 0, newItem);

        newItem = new QTableWidgetItem();
        newItem->setData(Qt::EditRole, node.played);
        newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);
        setItem(row, 1, newItem);

        newItem = new QTableWidgetItem();
        newItem->setData(Qt::EditRole, (float)node.winsBlack/node.played);
        newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);
        setItem(row, 2, newItem);

        newItem = new QTableWidgetItem();
        newItem->setData(Qt::EditRole, (float)node.AMAFwinsBlack/node.AMAFplayed);
        newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);
        setItem(row, 3, newItem);

        newItem = new QTableWidgetItem();
        newItem->setData(Qt::EditRole, (float)(node.winsBlack+node.AMAFwinsBlack)/(node.played+node.AMAFplayed));
        newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);
        setItem(row, 4, newItem);

        newItem = new QTableWidgetItem();
        newItem->setData(Qt::EditRole, node.uctValue);
        newItem->setFlags(newItem->flags() ^ Qt::ItemIsEditable);
        setItem(row, 5, newItem);
        row++;
    }

    setSortingEnabled(true);
}
