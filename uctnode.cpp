#include "uctnode.h"

UCTNode::UCTNode(QPoint pos, int wb, int p, int amafwb, int amafp, float uct)
{
    this->pos = pos;
    this->winsBlack = wb;
    this->played = p;
    this->AMAFplayed = amafwb;
    this->AMAFplayed = amafp;
    this->uctValue = uct;
}
