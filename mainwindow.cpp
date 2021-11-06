#include "mainwindow.h"
#include "player.h"

MainWindow::MainWindow(QWidget *parent): QWidget(parent)
{
    this->setFixedSize(800, 600);
    grid = new QGridLayout;
    for (int i = 0; i < NWOLFS; i++){
        wolfs[i].setPoint(QPoint (i+1, N-1));
    }
    setLayout(grid);
}

void MainWindow::drawGrid(QPainter &qp)
{
    for (int i = 1; i < N + 1; i++){
        qp.drawLine(0, i * CELL_SIZE, N * CELL_SIZE, i * CELL_SIZE);
    }
    for (int i = 1; i < N + 1; i++){
        qp.drawLine(i * CELL_SIZE, 0,  i * CELL_SIZE, N * CELL_SIZE);
    }
}

void MainWindow::drawGoat(QPainter &qp)
{
    QPoint  goatCoords1 = this->goat.getCoords() * CELL_SIZE;
    QPoint  goatCoords2 = goatCoords1 + QPoint( CELL_SIZE, CELL_SIZE );
    qp.drawEllipse( QRect( goatCoords1, goatCoords2 ) );
}

void MainWindow::drawWolf(QPainter &qp, Wolf wolf)
{
    QPoint  wolfCoords = wolf.getCoords();
    int a = CELL_SIZE*2/3;
    QPoint  cellCenter = wolfCoords * CELL_SIZE + QPoint( CELL_SIZE/2, CELL_SIZE/2 );
    QPoint  firstPoint = cellCenter - QPoint( 0, (int) (a/1.732) );
    QPoint  secondPoint = firstPoint + QPoint( a/2, (int) (a*1.732/2) );
    QPoint  thirdPoint = secondPoint - QPoint( a, 0 );
    qp.drawLine(firstPoint, secondPoint);qp.drawLine(secondPoint, thirdPoint);qp.drawLine(thirdPoint, firstPoint);
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter qp(this);

    /* Draw Grid */
    this->drawGrid(qp);

    /* Draw goat */
    this->drawGoat(qp);

    /* Draw wolf */
    for (int i = 0; i < NWOLFS; i++){
        Wolf wolf = wolfs[i];
        this->drawWolf(qp, wolf);
    }
}

void MainWindow::goatTurn(int key)
{
    QPoint  goatCoords = this->goat.getCoords();
    if ( (key==Qt::Key_S) && (goatCoords.y() != (N - 1) ) ) {
        goatCoords.ry()++;
        this->turnMade = 1;
    }

    if ( (key==Qt::Key_W) && (goatCoords.y() != 0 ) ) {
        goatCoords.ry()--;
        this->turnMade = 1;
    }

    if ( (key==Qt::Key_D) && (goatCoords.x() != (N - 1) ) ) {
        goatCoords.rx()++;
        this->turnMade = 1;
    }

    if ( (key==Qt::Key_A) && (goatCoords.x() != 0 ) ) {
        goatCoords.rx()--;
        this->turnMade = 1;
    }
    this->goat.setPoint(goatCoords);
}

int MainWindow::wolfTurn(int num, int dir)
{
    /* exit codes:
       0 - no errors,
       1 - wrong num,
       2 - wrong dir,
       3 - can`t go to this dir
    */
    if ( (num < 0) || (num > NWOLFS) ){
        return 1;
    }

    //check dir
    if ( (dir != 0) && (dir != 1) ){
        return 2;
    }

    QPoint newPosition = this->wolfs[num].getCoords();
    if ( (dir) && (newPosition.x()) ){
        newPosition -= QPoint(1, 1);
    } else  if ( (!dir) && ( newPosition.x()!=(N-1) ) ) {
        newPosition -= QPoint(-1, 1);
    } else {
        return 3;
    }
    for (int i = 0; i < NWOLFS; i++){
        if (i!=num){
            if (newPosition == this->wolfs[i].getCoords()){
                return 3;
            }
        }
    }
    this->wolfs[num].setPoint(newPosition);
    return 0;
}

void MainWindow::wolfsTurn( void )
{
    for (int i = 0; i < NWOLFS; i++){
        int dir = rand()%2;
        int code = this->wolfTurn(i, dir);
        if (code){
            this->wolfTurn(i, !dir);
        }
    }
}

int MainWindow::checkWin( void )
{
    return 0;
}

void MainWindow::endGame( void )
{
    this->close();
}

void MainWindow::timerEvent(QTimerEvent *)
{
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{   
    int key = e->key();

    if (key == Qt::Key_Q){
        this->endGame();
    }

    this->goatTurn(key);
    if (this->turnMade){
        this->turnMade = 0;
        this->wolfsTurn();
        this->update();
    }

    if (this->checkWin()){
        this->endGame();
    }
}

MainWindow::~MainWindow()
{
}

