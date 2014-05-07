#ifndef UCTNODETABLE_H
#define UCTNODETABLE_H

#include "uctnode.h"
#include <QTableWidget>

class UctNodesTable : public QTableWidget
{
    Q_OBJECT
public:
    explicit UctNodesTable(QWidget *parent = 0);

    /**
     * @brief Set UCT tree node info about computed moves
     * @param treePos All legal children evaluated in previous genmove
     */
    void setUCTTree(QList<UCTNode> treePos);

signals:

public slots:

};

#endif // UCTNODETABLE_H
