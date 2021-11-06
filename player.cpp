#include "player.h"

Goat::Goat(int x, int y)
{
    this->coords.setX(x);
    this->coords.setY(y);
}

QPoint Goat::getCoords(void)
{
    return coords;
}

void Goat::setPoint(QPoint point)
{
    this->coords = point;
}

Wolf::Wolf() : Goat(0, 0)
{
    
}