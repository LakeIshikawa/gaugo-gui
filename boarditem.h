#ifndef BOARDITEM_H
#define BOARDITEM_H

#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QMouseEvent>

// Display parameters
#define BOARD_S 432
#define STAR_S 4

/**
 * @brief The Board item
 */
class BoardItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:
    explicit BoardItem(QGraphicsScene* scene, int size);

    // Set turn
    void setTurn(int turn);

    // Override
    QRectF boundingRect() const;

    // Override
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    // Get intersection screen coordinates
    QPoint getIntersectionPoint(int intX, int intY);

    // Get the stone display size
    int getStoneSize() const;

    // Move string representation
    static QString moveToString(int x, int y);

protected:
    // Mouse events
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *releaseEvent);

signals:
    void movePlayed(QString &move, QChar turn);

public slots:

private:
    // The scene
    QGraphicsScene* scene;

    // Board size
    int size;

    // Current turn
    int turn;

    // Hover stone (current turn stone)
    QPoint hoverPoint;

};

#endif // BOARDITEM_H
