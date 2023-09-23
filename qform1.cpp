#include "qform1.h"
#include "ui_qform1.h"

/*Public*/
QForm1::QForm1()
    :ui(new Ui::QForm1)
{
    ui->setupUi(this);
    setCentralWidget(ui->widget);
    QPaintBox1 = new QPaintBox(0, 0, ui->widget);
    connect(QPaintBox1, &QPaintBox::OnMousePress, this, &QForm1::onQPB_MousePress);
    connect(QPaintBox1, &QPaintBox::OnMouseMove, this, &QForm1::onQPB_MouseMove);
    connect(QPaintBox1, &QPaintBox::OnMouseRelease, this, &QForm1::onQPB_MouseRelease);

    QTimer1 = new QTimer(this);
    connect(QTimer1, &QTimer::timeout, this, &QForm1::onQTimer1);
    QTimer1->start(5);

    QPixmap pepe(":/recursos/blue_ball.bmp");
    QBitmap mask = pepe.createMaskFromColor(Qt::white, Qt::MaskInColor);
    pepe.setMask(mask);

    Ball1 = new Ball(pepe.width()/2, pepe);
    connect(Ball1, &Ball::drawNow, this, &QForm1::drawBall);
    Ball1->setMuD(0.35);
    Ball1->setAngulo(45);
    Ball1->setResolucion(200);
    Ball1->setPos(201, 201);
    Ball1->setVi(3);
    Ball1->setLimit(800, 600);
    Ball1->setCoefChoque(0.70);

    dev = new Debug(this);
}

QForm1::~QForm1()
{
    delete Ball1;
    delete QPaintBox1;
    delete ui;
}

/*Private slots*/
void QForm1::onQPB_MouseMove(QMouseEvent *event)
{
    unsigned int eX = event->pos().x(), eY = event->pos().y();
    if(ballPressed){
        if(eX<Ball1->getLimit().width-Ball1->getRadio() && eX > Ball1->getRadio()){
            if(eY<Ball1->getLimit().height-Ball1->getRadio() && eY > Ball1->getRadio()){
                Ball1->setPos(eX, eY);
            }
        }

    }
    drawBall(Ball1, true);
}

void QForm1::onQPB_MousePress(QMouseEvent *event)
{
    if(event->button() == Qt::MouseButton::LeftButton){
        int cX = event->pos().x(), cY = event->pos().y();
        if(cX>=Ball1->getPos().x-Ball1->getRadio() && cX <= Ball1->getPos().x+Ball1->getRadio()){
            if(cY>=Ball1->getPos().y-Ball1->getRadio() && cY <= Ball1->getPos().y+Ball1->getRadio()){
                qDebug()<<"clicked";
                ballPressed = true;
            }
        }
    }
}

void QForm1::onQPB_MouseRelease(QMouseEvent *event)
{
    Q_UNUSED(event);
    if(event->button() == Qt::MouseButton::LeftButton){
        ballPressed = false;
        qDebug()<<"unclicked";
    }
}

void QForm1::onQTimer1()
{
    static uint8_t dos = 2;
    if(Ball1->getIsLaunched()){
        Ball1->physicsTask();
    }
    dos--;
    if(dos == 0){
        dos = 2;
        drawBall(Ball1, true);
    }
    //qDebug()<<Ball1->getIsLaunched()<<" Vx"<<Ball1->getV().x<<" Vy"<<Ball1->getV().y<<" Px"<<Ball1->getPos().x<<" Py"<<Ball1->getPos().y;
}

void QForm1::drawBall(Ball *ball, bool update)
{
    QPainter painter(QPaintBox1->getCanvas());
    QPen pen;
    QBrush brush;
    static QPoint lastDraw;
    pen.setWidth(4);
    pen.setColor(Qt::black);
    brush.setColor(Qt::black);
    brush.setStyle(Qt::BrushStyle::SolidPattern);
    painter.setPen(pen);
    painter.setBrush(brush);
    painter.drawRect(lastDraw.x()-ball->getRadio(), lastDraw.y()-ball->getRadio(), ball->getRadio()*2, ball->getRadio()*2);
    painter.drawPixmap(ball->getPos().x-ball->getRadio(), ball->getPos().y-ball->getRadio(), ball->getImagen());
    lastDraw = QPoint(ball->getPos().x, ball->getPos().y);
    if(update)
        QPaintBox1->update();
}

void QForm1::resizeEvent(QResizeEvent *event)
{
    if(event->type() == QEvent::Resize){
        QPaintBox1->resize(ui->widget->width(), ui->widget->height());
        Ball1->setLimit(ui->widget->width(), ui->widget->height());
        QPB_initialize();
    }
}

/*Private*/
void QForm1::QPB_initialize()
{
    QPen pen;
    QBrush brush;
    QPaintBox1->resize(ui->widget->width(), ui->widget->height());
    QPainter painter(QPaintBox1->getCanvas());
    pen.setWidth(4);
    pen.setColor(Qt::black);
    brush.setColor(Qt::black);
    brush.setStyle(Qt::BrushStyle::SolidPattern);
    painter.setPen(pen);
    painter.setBrush(brush);
    painter.drawRect(0, 0, ui->widget->width(), ui->widget->height());
    basePixmap = QPaintBox1->getCanvas();
    QPaintBox1->setPixelCanvas(basePixmap);
    QPaintBox1->update();
}


void QForm1::on_actionDeveloper_Mode_triggered()
{
    if(dev->isHidden()){
        dev->show();
    }else{
        dev->hide();
    }
}


void QForm1::on_actionLaunch_triggered()
{
    if(ui->actionLaunch->isChecked()){
        ui->actionLaunch->setIcon(QIcon(":/recursos/stop.png"));
        Ball1->setIsLaunched(true);
    }else{
        ui->actionLaunch->setIcon(QIcon(":/recursos/play-button.png"));
        Ball1->setIsLaunched(false);
    }
}

