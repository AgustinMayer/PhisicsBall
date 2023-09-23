#include "ball.h"
#include "qdebug.h"
#include <cmath>
#include <QDateTime>

Ball::Ball(QObject *parent): QObject{parent}
{

}

Ball::Ball(uint32_t newRadio, QPixmap newImagen)
    :  radio(newRadio), imagen(newImagen)
{

    isLaunched = false;
    V.i = 0;
    V.y = 0;
    V.x = 0;
    V.yi = 0;
    coefChoque = 0.7;
    Resolucion = 1.0;
    isRoz = false;
}

Ball::~Ball()
{

}

void Ball::physicsTask()
{
    if(!isLaunched){
        return;
    }
    diffTime = (QDateTime::currentMSecsSinceEpoch() - diffTime)/1000.0; //dejo diffTime en segundos

    verifyDinamycs();
    checkColision();
    calculate(diffTime);
    verifyStop();
    V.yi = V.y;
    diffTime = QDateTime::currentMSecsSinceEpoch();
}

double Ball::toRad(float angle)
{
    return (angle * M_PI / 180);
}

double Ball::toDeg(float angle)
{
    return (angle * 180 / M_PI);
}


void Ball::verifyDinamycs()
{
    if(V.y <= 0.09 && V.y >= -0.09){
        if(pos.y <= limit.height-radio+1 && pos.y >=limit.height-radio-1){
            isRoz=true;
            return;
        }
    }
    isRoz = false;
}

void Ball::verifyStop()
{
    if(V.x <=0.02 && V.x >= -0.02){
        if(V.y <= 0.05 && V.y >= -0.05){
            isLaunched = false;
            V.x = 0;
            V.y = 0;
            drawNow(this, true);
        }
    }
}

Pos Ball::getPos() const
{
    return pos;
}

void Ball::setPos(const Pos &newPos)
{
    pos = newPos;
}

void Ball::setPos(double newX, double newY)
{
    pos.lastX = pos.x;
    pos.lastY = pos.y;
    pos.x = newX;
    pos.y = newY;
}

Vel Ball::getV() const
{
    return V;
}

void Ball::setV(const Vel &newV)
{
    V = newV;
}

void Ball::setVi(const double newVi)
{
    V.i = newVi;
}

double Ball::getAngulo() const
{
    return 1;
}

void Ball::setAngulo(float newAngulo)
{
    angulo = abs(360-newAngulo);
}

QPixmap Ball::getImagen() const
{
    return imagen;
}

void Ball::setImagen(const QPixmap &newImagen)
{
    imagen = newImagen;
}

Limits Ball::getLimit() const
{
    return limit;
}

void Ball::setLimit(const Limits &newLimit)
{
    limit = newLimit;
}

void Ball::setLimit(const uint32_t newWidth, const uint32_t newHeight)
{
    limit.height = newHeight;
    limit.width = newWidth;
}

bool Ball::getIsLaunched() const
{
    return isLaunched;
}

void Ball::setIsLaunched(bool newIsLaunched)
{
    isLaunched = newIsLaunched;
    if(isLaunched){
        diffTime = QDateTime::currentMSecsSinceEpoch();
        V.i *= Resolucion;
        V.x = V.i * cos(toRad(angulo));
        V.yi = V.i * sin(toRad(angulo));
        aceleracion = muD*GRAVITY; /* a=Fr/m  Fr=mu*m*Grav   */
    }

}

double Ball::getCoefChoque() const
{
    return coefChoque;
}

void Ball::setCoefChoque(double newCoefChoque)
{
    coefChoque = newCoefChoque;
}

uint32_t Ball::getRadio() const
{
    return radio;
}

void Ball::setRadio(uint32_t newRadio)
{
    radio = newRadio;
}

double Ball::getResolucion() const
{
    return Resolucion;
}

void Ball::setResolucion(double newResolucion)
{
    Resolucion = newResolucion;
}

double Ball::getMuD() const
{
    return muD;
}

void Ball::setMuD(double newMuD)
{
    muD = newMuD;
}

void Ball::checkColision()
{
    double d=0, t=0, tReal=0, Vyf=0;
    if(pos.x <= radio){
        d = pos.lastX - radio;
        tReal = d / abs(V.x);
        t = diffTime - tReal;
        pos.x = pos.lastX;
        pos.y = pos.lastY;
        calculate(tReal);
        emit drawNow(this, false);
        V.yi = V.y;
        V.x = -V.x* coefChoque;
        calculate(t);
        emit drawNow(this, false);
    }

    if(pos.x >= limit.width-radio){
        d = limit.width - pos.lastX - radio;
        tReal = d / abs(V.x);
        t = diffTime - tReal;
        pos.x = pos.lastX;
        pos.y = pos.lastY;
        calculate(tReal);
        emit drawNow(this, false);
        V.yi = V.y;
        V.x = -V.x* coefChoque;
        calculate(t);
        emit drawNow(this, false);
    }
    if(!isRoz){
        if(pos.y <= radio){
            d = pos.lastY - radio;
            Vyf = sqrt(pow(V.yi, 2.0) + 2.0 * GRAVITY * d);
            tReal = (Vyf+V.yi)/GRAVITY;
            t = diffTime - tReal;
            pos.x = pos.lastX;
            pos.y = pos.lastY;
            calculate(tReal);
            emit drawNow(this, false);
            V.yi = -V.y * coefChoque;
            V.x = V.x;
            calculate(t);
            emit drawNow(this, false);
        }

        if(pos.y >= limit.height-radio){
            d = limit.height - pos.lastY - radio;
            Vyf = sqrt(pow(V.yi, 2.0) + 2.0 * GRAVITY * d);
            tReal = (Vyf-V.yi)/GRAVITY;
            t = diffTime - tReal;
            pos.x = pos.lastX;
            pos.y = pos.lastY;
            calculate(tReal);
            emit drawNow(this, false);
            V.yi = -V.y * coefChoque;
            V.x = V.x;
            calculate(t);
            emit drawNow(this, false);
        }
    }
}

void Ball::calculate(double time)
{
    pos.lastX = pos.x;
    pos.lastY = pos.y;
    if(isRoz){
        V.y = 0;
        pos.y = limit.height - radio;
        if(V.x > 0)
            V.x = V.x - aceleracion * time;
        else
            V.x = V.x + aceleracion * time;
        pos.x += V.x * time - (aceleracion * pow(time, 2.0))/2.0;
    }else{
        V.y = V.yi + GRAVITY * time;
        pos.x += V.x * time;
        pos.y += V.y * time - (GRAVITY * pow(time, 2.0))/2.0;
    }
}

