#ifndef UCTNODE_H
#define UCTNODE_H

#include <QPoint>

class UCTNode
{
public:
    UCTNode(QPoint pos, int wb, int p, int amafwb, int amafp, float uct);

    QPoint pos;
    int winsBlack;
    int played;
    int AMAFwinsBlack;
    int AMAFplayed;
    float uctValue;
};

#endif // UCTNODE_H
