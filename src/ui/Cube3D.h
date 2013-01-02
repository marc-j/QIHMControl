#ifndef CUBE3D_H
#define CUBE3D_H

#include "qglview.h"

#include "UAV.h"

class Cube3D : public QGLView
{
    Q_OBJECT
public:
    explicit Cube3D(QWidget *parent = 0);
    ~Cube3D();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    
signals:
    
public slots:

    void setPitch(double);
    void setRoll(double);
    void setYaw(double);
    void setAxis(double, double, double);

protected:
    void paintGL(QGLPainter *painter);

private:
    QGLSceneNode *cube;

    double pitch;
    double roll;
    double yaw;

    UAV* uav;
    
};

#endif // CUBE3D_H
