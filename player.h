#pragma once
#include <QPoint>

class Goat
{
public:
    Goat(int, int);
    QPoint getCoords(void);
    void setPoint(QPoint);

private:
    QPoint coords;
};

class Wolf : public Goat
{
public:
    Wolf(void);

private:
    QPoint coords;

};