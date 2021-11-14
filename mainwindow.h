#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "player.h"

#include <QWidget>
#include <QKeyEvent>
#include <QPainter>
#include <QTime>
#include <QImage>
#include <QDebug>
#include <QPainterPath>
#include <QPointF>
#include <QBrush>

#include <cmath>

// QT_BEGIN_NAMESPACE
// namespace Ui { class MainWindow; }
// QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    const int N = 3;
    const int M = 4;
    const int TRUERAWS = 2 * N + 1;
    const int CELL_SIZE = 40;
    static const int NWOLFS = 5;
    Goat goat = Goat(N/2 - 1, 0);
    Wolf wolfs[NWOLFS];
    int WIDTH = 800;
    int HEIGHT = 600;
    int turnMade = 0;
    int gameOver = 0;
    int numOfDirs = 2;
    int depthLim = 3;

protected:
    void paintEvent(QPaintEvent *) override;
    void timerEvent(QTimerEvent *);
    void keyPressEvent(QKeyEvent *);

private:
    void drawGrid(QPainter &);
    void drawGoat(QPainter &);
    void drawWolf(QPainter &, Wolf);
    void goatTurn(int);
    void goatTurn(void);
    float wolfsTurn(int);
    float goatTurnMM(int);
    int wolfTurn(int , int);
    int checkWin(void);
    void endGame(void);
    char** createAllMoves(void);
    int sheffer(int, int);

    QPainterPath hexagon;
    QPainterPath bg;
};
#endif // MAINWINDOW_H
