#ifndef BOARDVIEW_H
#define BOARDVIEW_H

#include "boarditem.h"
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
     * @brief Set current turn (to display hovering stone)
     * @param turn 0-Black 1-White
     */
    void setTurn(int turn);

signals:

public slots:

private:
    // The scene of the view
    QGraphicsScene* scene;

    // Board
    BoardItem* board;
};

#endif // BOARDVIEW_H
