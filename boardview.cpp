#include "boardview.h"
#include <QDebug>
#include <QGraphicsEllipseItem>

#define VIEWPORT_S 432

BoardView::BoardView(QObject *parent) :
    QGraphicsView((QWidget*)parent)
{
    board = NULL;
    setMouseTracking(true);
}

void BoardView::setup()
{
    scene = new QGraphicsScene(0, 0, VIEWPORT_S, VIEWPORT_S, this);
    setScene(scene);
    setRenderHint(QPainter::Antialiasing,true);
}

BoardItem* BoardView::setBoard(int size)
{
    scene->clear();

    // Add Bg
    board = new BoardItem(scene, size);
    scene->addItem(board);

    return board;
}

void BoardView::setPosition(QString &position)
{
    QStringList lines = position.split("|");

    // Set stone brush
    QBrush stoneBrush(Qt::gray);
    QPen stonePen(Qt::black);
    stonePen.setWidth(0);

    // Add stones
    for( int i=0; i<lines.length(); i++ ){
        QString line = lines.at(i);

        for( int j=0; j<line.length(); j++ ){
            // Look which color to paint
            if( line.at(j) == '-' ) continue;
            switch( line.at(j).toLatin1() ){
            case 'O': stoneBrush.setColor(Qt::white); break;
            case 'X': stoneBrush.setColor(Qt::black); break;
            }

            // Add stone
            QPoint st = board->getIntersectionPoint(j, i);
            scene->addEllipse(
                        st.x()-board->getStoneSize()/2,
                        st.y()-board->getStoneSize()/2, board->getStoneSize(),
                        board->getStoneSize(),
                        stonePen,
                        stoneBrush);
        }
    }
}

void BoardView::setTurn(int turn)
{
    board->setTurn(turn);
}
