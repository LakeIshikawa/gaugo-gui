#ifndef BOARDVIEW_H
#define BOARDVIEW_H

#include "boarditem.h"
#include "uctnode.h"
#include <QGraphicsView>

class BoardView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit BoardView(QObject *parent = 0);

    /**
     * @brief Initialize ui element
     */
    void setup();

    /**
     * @brief setBoard
     * @param size The side of the board (9, 13, 19...)
     */
    BoardItem* setBoard(int size);

    /**
     * @brief Set the board to the specified position
     * @param position String representing position (O=white X=black -=empty)
     */
    void setPosition(QString &position);

    /**
     * @brief Set UCT tree node info about computed moves
     * @param treePos All legal children evaluated in previous genmove
     */
    void setUCTTree(QList<UCTNode> treePos);

    /**
     * @brief Set current turn (to display hovering stone)
     * @param turn 0-Black 1-White
     */
    void setTurn(int turn);

    /**
     * @brief Transform a move string to a pair of coordinates
     * @param intersection The move string representation
     * @return The board coordinate
     */
    static QPoint nameToPoint(const QString &intersection);

    /**
     * @brief Transform a pair of coordinates to a move string
     * @param The board coordinate
     * @return The move string representation
     */
    static QString pointToName(const QPoint& point);


signals:

public slots:

private:
    // The scene of the view
    QGraphicsScene* scene;

    // Board
    BoardItem* board;
};

#endif // BOARDVIEW_H
