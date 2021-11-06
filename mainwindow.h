#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "player.h"

#include <QWidget>
#include <QKeyEvent>
#include <QGridLayout>
#include <QPainter>
#include <QTime>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    const int N = 12;
    const int CELL_SIZE = 50;
    static const int NWOLFS = 5;
    Goat goat = Goat(N/2 - 1, 0);
    Wolf wolfs[NWOLFS];
    int WIDTH = 800;
    int HEIGHT = 600;
    int turnMade = 0;

protected:
    void paintEvent(QPaintEvent *) override;
    void timerEvent(QTimerEvent *);
    void keyPressEvent(QKeyEvent *);

private:
    void drawGrid(QPainter &);
    void drawGoat(QPainter &);
    void drawWolf(QPainter &, Wolf);
    void goatTurn(int);
    void wolfsTurn(void);
    int wolfTurn(int , int);
    int checkWin(void);
    void endGame(void);
    QGridLayout *grid;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
