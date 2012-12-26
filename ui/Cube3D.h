#ifndef CUBE3D_H
#define CUBE3D_H

#include "qglview.h"

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

    void setPitch(float);
    void setRoll(float);
    void setYaw(float);
    void setAxis(float, float, float);

protected:
    void paintGL(QGLPainter *painter);

private:
    QGLSceneNode *cube;

    float pitch;
    float roll;
    float yaw;
    
};

#endif // CUBE3D_H
