#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QWidget(parent)
{
    this->setFixedSize(600, 600);

    hexagon.moveTo(CELL_SIZE, 0);
    hexagon.lineTo(CELL_SIZE/2, -CELL_SIZE*1.732/2);
    hexagon.lineTo(-CELL_SIZE/2, -CELL_SIZE*1.732/2);
    hexagon.lineTo(-CELL_SIZE, 0);
    hexagon.lineTo(-CELL_SIZE/2, CELL_SIZE*1.732/2);
    hexagon.lineTo(CELL_SIZE/2, CELL_SIZE*1.732/2);
    hexagon.closeSubpath();

    bg.lineTo( QPointF(0, (2*N + 1) * CELL_SIZE*1.732) );
    bg.lineTo( QPointF( ( M + 1 ) * 2 * CELL_SIZE, ( 2*N + 1 ) * CELL_SIZE*1.732 ) );
    bg.lineTo( QPointF( ( M + 1 ) * 2 * CELL_SIZE, 0) );
    bg.closeSubpath();

    goat.setPoint(QPoint(M/2, TRUERAWS/2));

    for (int i = 0; i < (NWOLFS - 1); i++){
        wolfs[i].setPoint(QPoint(i, TRUERAWS - 1));
    }
    wolfs[4].setPoint(QPoint(M/2, TRUERAWS - 2));
}

int MainWindow::sheffer(int a, int b)
{
    return !(a && b);
}

void MainWindow::drawGrid(QPainter &qp)
{
    qp.fillPath(bg, Qt::yellow);

    for (int i = 0; i < (2*N + 1); i++){
        QPointF firstCenter = QPointF( CELL_SIZE * ( 2 - i%2), CELL_SIZE * 1.732/2 * (2*i + 1) );
        QPainterPath subHexagon = hexagon.translated(firstCenter);
        
        for (int j = 0; j < (M + i%2); j++){
            qp.fillPath(subHexagon, Qt::green);
            subHexagon.translate(QPointF(2 * CELL_SIZE, 0));
        }
    }
    qp.drawLine(QPoint(0, (2*N + 1) * CELL_SIZE*1.732), QPoint( ( M + 1 ) * 2 * CELL_SIZE, ( 2*N + 1 ) * CELL_SIZE*1.732 ) );
    qp.drawLine(QPoint(( M + 1 ) * 2 * CELL_SIZE, 0), QPoint( ( M + 1 ) * 2 * CELL_SIZE, ( 2*N + 1 ) * CELL_SIZE*1.732 ) );
}

void MainWindow::drawGoat(QPainter &qp)
{
    QPoint  goatCoords = this->goat.getCoords();
    QPointF center;
    center.setX( (goatCoords.x() * 2 + 2 - goatCoords.y()%2) * CELL_SIZE );
    center.setY( (goatCoords.y() * 2 + 1) * CELL_SIZE * 1.732/2);
    float x = 2 * 1.732 * CELL_SIZE / 2.732;
    QPointF ULCorner = center - QPointF(x/2, x/2);
    QPointF LRCorner = center + QPointF(x/2, x/2);

    qp.drawEllipse( QRectF( ULCorner, LRCorner ) );
}

void MainWindow::drawWolf(QPainter &qp, Wolf wolf)
{
    QPoint  wolfCoords = wolf.getCoords();

    int a = CELL_SIZE;

    QPointF  cellCenter; 
    cellCenter.setX( (wolfCoords.x() * 2 + 2 - wolfCoords.y()%2) * CELL_SIZE );
    cellCenter.setY( (wolfCoords.y() * 2 + 1) * CELL_SIZE * 1.732/2);

    QPainterPath wolfTriang;
    wolfTriang.moveTo( QPointF( 0,   -a/1.732  ) );
    wolfTriang.lineTo( QPointF( a/2,  a*1.732/6 ) );
    wolfTriang.lineTo( QPointF( -a/2, a*1.732/6 ) );
    wolfTriang.closeSubpath();

    qp.drawPath(wolfTriang.translated(cellCenter));
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
    if ( (key==Qt::Key_Q) && ( goatCoords.y() != 0 ) && sheffer(goatCoords.x()==0, goatCoords.y()%2 == 1)  ) {
        goatCoords.rx() -= goatCoords.y()%2;
        goatCoords.ry()--;
        this->turnMade = 1;
    }

    if ( (key==Qt::Key_E) && ( goatCoords.y() != 0 ) && sheffer(goatCoords.x() == M, goatCoords.y()%2 == 1) ) {
        goatCoords.rx() += !(goatCoords.y()%2);
        goatCoords.ry()--;
        this->turnMade = 1;
    }

    if ( (key==Qt::Key_Z) && (goatCoords.y() != (TRUERAWS - 1) ) && sheffer(goatCoords.x()==0, goatCoords.y()%2 == 1 ) ) {
        goatCoords.rx() -= goatCoords.y()%2;
        goatCoords.ry()++;
        this->turnMade = 1;
    }

    if ( (key==Qt::Key_C) && (goatCoords.y() != (TRUERAWS - 1) ) && sheffer(goatCoords.x() == M, goatCoords.y()%2 == 1) ) {
        goatCoords.rx() += !(goatCoords.y()%2); 
        goatCoords.ry()++;
        this->turnMade = 1;
    }

    if ( (key==Qt::Key_D) && (goatCoords.x() != ( M + goatCoords.y()%2 ) ) ) {
        goatCoords.rx()++;
        this->turnMade = 1;
    }

    if ( (key==Qt::Key_A) && (goatCoords.x() != 0 ) ) {
        goatCoords.rx()--;
        this->turnMade = 1;
    }
    this->goat.setPoint(goatCoords);
}

void MainWindow::goatTurn( void )
{
    
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
    if (newPosition.y()){

        if ( (dir) && sheffer(!newPosition.x(), newPosition.y()%2 == 1)){
            newPosition -= QPoint(newPosition.y()%2, 1);
        } else  if ( (!dir) && sheffer(newPosition.x()==(M + newPosition.y()%2 -1), newPosition.y()%2 == 1 ) ) {
            newPosition -= QPoint(newPosition.y()%2 - 1, 1);
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

    } else 
        return 3;
    this->wolfs[num].setPoint(newPosition);
    return 0;
}

char** MainWindow::createAllMoves()
{
    int numOfElements = pow(numOfDirs, NWOLFS);
    char **currDirs = new char*[numOfElements];
    for (int i = 0; i < numOfElements; i++){
        currDirs[i] = new char[NWOLFS];
        char x = i;
        for (int j = 0; j < NWOLFS; j++){
            currDirs[i][j] = x - ((x>>1)<<1);
            x = x>>1;
        }
    }
    return currDirs;
}

float MainWindow::wolfsTurn(int depth = 0)
{
    if (depth == depthLim){
        float cost = this->checkWin();
        if (cost == 0){
            int dist = 0;
            QPoint goatCoords = goat.getCoords();
            for (int i = 0; i < NWOLFS; i++){
                QPoint wolfCoords = wolfs[i].getCoords();
                dist += abs(wolfCoords.x() - goatCoords.x()) + abs(wolfCoords.y() - goatCoords.y());
            }
            cost = (float)dist/(NWOLFS * 4);
        }
        return cost;
    }

    char **allDirs = this->createAllMoves();
    int numOfElements = pow(numOfDirs, NWOLFS);
    char *deleted = new char[numOfElements];
    float min = 30;
    int minId = -1;
    int first = 1;

    Wolf currWolfs[NWOLFS];

    for (int i = 0; i < NWOLFS; i++){
            currWolfs[i] = wolfs[i];
    }

    for (int i = 0; i < numOfElements; i++){
        int badMove = 0;
        for (int j = 0; j < NWOLFS; j++){
            int code = this->wolfTurn(j, allDirs[i][j]);
            if (code){
                code = this->wolfTurn(j, !allDirs[i][j]);
                if (!code){
                    badMove = 1;
                    delete allDirs[i];
                    deleted[i] = 1;
                    break;
                }
            }
        }
        if (!badMove){
            deleted[i] = 0;
            float status = this->checkWin();
            if (!status){
                status = this->goatTurnMM(depth);
            }
            if (first){
                min = status;
                minId = i;
                first = 0;
            } else if (status < min){
                min = status;
                delete allDirs[minId];
                deleted[minId] = 1;
                minId = i;
            } else if (status == min){
                if (!(rand()%4)){
                    delete allDirs[minId];
                    deleted[minId] = 1;
                    minId = i;
                }
            } else {
                delete allDirs[i];
                deleted[i] = 1;
            }
        }
        for (int j = 0; j < NWOLFS; j++){
            wolfs[j] = currWolfs[j];
        }
    }
    
    if ( (!depth) /* && (minId > 0)  */){
        for (int i = 0; i < NWOLFS; i++){
            this->wolfTurn(i, allDirs[minId][i]);
        }
    }

    for (int i = 0; i < numOfElements; i++){
        if (!deleted[i])
            delete allDirs[i];
    }

    delete allDirs;
    delete deleted;
    return min;
}

float MainWindow::goatTurnMM(int depth = 0)
{
    if (depth == depthLim)
        return this->checkWin();
    Goat currGoat = goat;
    float maxMove = -2;
    for (int i = 0; i < 6; i ++){
        QPoint currCoords = currGoat.getCoords();

        int *x, *y;
        x = &currCoords.rx();
        y = &currCoords.ry();

        switch (i){
            case 0:
                if (*x != 0){
                    *x -= 1;
                }
                break;
            case 1:
                if (*x != (M - *y % 2 - 1) ) {
                    *x += 1;
                }
                break;
            case 2:
                if ( (*y) && sheffer(!(*x), *y % 2) ){
                    *x -= *y % 2;
                    *y -= 1;
                }
                break;
            case 3:
                if ((*y) && sheffer(*x == (M - *y % 2 - 1), *y % 2) ){
                    *x += 1 - *y % 2;
                    *y -= 1;
                }
                break;
            case 4:
                if ( (*y != ( TRUERAWS - 1 ) ) && sheffer(*x, *y%2) ) {
                    *x -= *y % 2;
                    *y += 1;
                }
                break;
            case 5:
                if ( (*y != ( TRUERAWS - 1 ) ) && sheffer(*x == (M - *y % 2 - 1), *y%2) ) {
                    *x += 1 - *y % 2;
                    *y += 1;
                }
                break;
        }
        if ( (currGoat.getCoords() != currCoords) ){
            goat.setPoint(currCoords);
            float status = this->checkWin();
            if (!status){
                status = this->wolfsTurn(depth + 1);
            }
            if (status > maxMove){
                maxMove = status;
            }
        }
        goat = currGoat;
    }
    return maxMove;
}

int MainWindow::checkWin( void )
{
    QPoint goatCoords = this->goat.getCoords();
    int nothing = 0;
    for (int i = 0; i < NWOLFS; i++){
        QPoint wolfCoords = this->wolfs[i].getCoords();
        if (goatCoords == wolfCoords){
            return -1;
        }
        if (goatCoords.y() < wolfCoords.y()){
            nothing = 1;
        }
    }
    if (nothing)
        return 0;
    return 1;
}

void MainWindow::endGame( void )
{
    if (gameOver)
        this->close();
    gameOver = 1;
}

void MainWindow::timerEvent(QTimerEvent *)
{
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{   
    if (gameOver){
        this->endGame();
    }
    int key = e->key();

    if (key == Qt::Key_Escape){
        this->close();
    }

    this->goatTurn(key);

    int gameState = this->checkWin();
    if (!gameState){
        if (this->turnMade){
            this->turnMade = 0;
            this->wolfsTurn();
            this->update();
        }
        gameState = this->checkWin();
    }

    if (gameState && !gameOver){
        if (gameState == 1){
            qDebug() << "win";
        } else {
            qDebug() << "lose";
        }
        this->endGame();
    }
}

MainWindow::~MainWindow()
{
}

