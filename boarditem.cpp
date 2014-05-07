#include "boarditem.h"
#include <QPainter>
#include <QGraphicsSceneHoverEvent>

BoardItem::BoardItem(QGraphicsScene* scene, int size)
{
    this->scene = scene;
    this->size = size;

    // No hovering at first
    hoverPoint.setX(-999);
    hoverPoint.setY(-999);

    // Receive events from hovering
    setAcceptHoverEvents(true);
}

void BoardItem::setTurn(int turn)
{
    this->turn = turn;
}

QPoint BoardItem::getIntersectionPoint(int intX, int intY)
{
    float oneCell = (float)BOARD_S/(size-1);
    QPoint ret(intX*oneCell, intY*oneCell);
    return ret;
}

QRectF BoardItem::boundingRect() const
{
    int edge = getStoneSize()/2 + 26;
    return QRectF(-edge, -edge, BOARD_S+edge*2, BOARD_S+edge*2);
}

int BoardItem::getStoneSize() const
{
    float oneCell = (float)BOARD_S/(size-1);
    return oneCell-4;
}

// Override
void BoardItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // Draw background rect
    QPen pen(Qt::black);
    pen.setWidth(6);
    QBrush brush(QColor(204, 153, 0));

    painter->setPen(pen);
    painter->setBrush(brush);
    painter->drawRect(boundingRect());

    // Draw lines
    pen.setWidth(1);
    painter->setPen(pen);
    float oneCell = (float)BOARD_S/(size-1);
    for( int x=0; x<size; x++ ){
        painter->drawLine(x*oneCell, 0, x*oneCell, BOARD_S);
        // Label
        painter->drawText(QPoint(x*oneCell-5, -20), QString(QChar('A'+x+(x>7))));
    }
    for( int y=0; y<size; y++){
        painter->drawLine(0, y*oneCell, BOARD_S, y*oneCell);
        // Label
        painter->drawText(QPoint(-30, y*oneCell+5), QString::number(y+1));
    }

    // Draw stars
    QBrush starBrush(Qt::black);
    painter->setBrush(starBrush);
    // TENGEN!
    if( size%2 == 1){
        painter->drawEllipse(BOARD_S/2-STAR_S/2, BOARD_S/2-STAR_S/2, STAR_S, STAR_S);
    }

    // 19x19 stars
    if( size == 19 ){
        int starPosX[] = {3, 9, 15, 3, 15, 3, 9, 15};
        int starPosY[] = {3, 3, 3, 9, 9,  15,15, 15};
        for( int s=0; s<8; s++){
            painter->drawEllipse(starPosX[s]*oneCell-STAR_S/2, starPosY[s]*oneCell-STAR_S/2, STAR_S, STAR_S);
        }
    }

    // Turn stone
    if( hoverPoint.x() != -999 ){
        painter->setOpacity(0.5);
        QBrush stoneBrush(turn == 0 ? Qt::black : Qt::white);
        painter->setBrush(stoneBrush);
        painter->drawEllipse(
                    hoverPoint.x()*oneCell-getStoneSize()/2,
                    hoverPoint.y()*oneCell-getStoneSize()/2,
                    getStoneSize(),
                    getStoneSize());
    }
}

void BoardItem::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    hoverPoint.setX(-999);
    hoverPoint.setY(-999);
    QGraphicsItem::hoverLeaveEvent(event);
}

void BoardItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    float oneCell = (float)BOARD_S/(size-1);
    int int_x = (event->pos().x()+getStoneSize()/2) / oneCell;
    int int_y = (event->pos().y()+getStoneSize()/2) / oneCell;

    hoverPoint.setX(int_x);
    hoverPoint.setY(int_y);
    update(boundingRect());
}

void BoardItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

}

void BoardItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    float oneCell = (float)BOARD_S/(size-1);
    int int_x = (event->pos().x()+getStoneSize()/2) / oneCell;
    int int_y = (event->pos().y()+getStoneSize()/2) / oneCell;

    if( int_x>=0 && int_y>=0 && int_x<size && int_y<size ){
        QString move = moveToString(int_x, int_y);
        emit movePlayed(move, turn==0?'b':'w');
    }
}

QString BoardItem::moveToString(int x, int y)
{
    QString ret;
    ret.append('A'+x+(x>7));
    ret.append(QString("%1").arg(y+1));
    return ret;
}
