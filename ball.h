#ifndef BALL_H
#define BALL_H

#include <QObject>
#include <QPixmap>
#include <cstdint>

#define GRAVITY 9.81 //m/s2

typedef struct{
    double i;
    double x;
    double y;
    double yi;
}Vel;
typedef struct{
    double x;
    double y;
    double lastX;
    double lastY;
}Pos;

typedef struct{
    uint32_t width;
    uint32_t height;
}Limits;

class Ball : public QObject
{
    Q_OBJECT
public:
    explicit Ball(QObject *parent = nullptr);

    Ball(uint32_t newRadio, QPixmap newImagen);

    ~Ball();

    void physicsTask();

    double toRad(float angle);
    double toDeg(float angle);

    /*GETTERS & SETTERS*/
    Pos getPos() const;
    void setPos(const Pos &newPos);
    void setPos(double newX, double newY);

    Vel getV() const;
    void setV(const Vel &newV);
    void setVi(const double newVi);

    double getAngulo() const;
    void setAngulo(float newAngulo);

    QPixmap getImagen() const;
    void setImagen(const QPixmap &newImagen);

    Limits getLimit() const;
    void setLimit(const Limits &newLimit);
    void setLimit(const uint32_t, const uint32_t);

    bool getIsLaunched() const;
    void setIsLaunched(bool newIsLaunched);

    double getCoefChoque() const;
    void setCoefChoque(double newCoefChoque);

    uint32_t getRadio() const;
    void setRadio(uint32_t newRadio);

    double getResolucion() const;
    void setResolucion(double newResolucion);

    double getMuD() const;
    void setMuD(double newMuD);

    /*FIN GETTERS & SETTERS*/
signals:

    void drawNow(Ball*, bool);

private:
    uint32_t radio;
    QPixmap imagen;
    Vel V;
    Pos pos;
    double angulo;
    double coefChoque;
    double muD;

    bool isLaunched, isRoz;
    double diffTime;
    Limits limit;
    double aceleracion;
    double Resolucion;

    void checkColision();
    void calculate(double time);
    void verifyDinamycs();
    void verifyStop();
};

#endif // BALL_H
