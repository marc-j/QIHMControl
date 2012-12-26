#include "Cube3D.h"

#include <QDebug>
#include <cmath>

#include "qglbuilder.h"
#include "qglcube.h"

Cube3D::Cube3D(QWidget *parent) :
    QGLView(parent),
    pitch(45.0f),
    roll(45.0f),
    yaw(0.0f)
{

    this->setOption(QGLView::CameraNavigation,false);

    QGLBuilder builder;
    builder << QGL::Faceted;
    builder << QGLCube(1.5f);

    cube = builder.finalizedSceneNode();
}

Cube3D::~Cube3D()
{
    delete cube;
}

void Cube3D::setPitch(float p)
{
    pitch = p;
}

void Cube3D::setRoll(float r)
{
    roll = r;
}

void Cube3D::setYaw(float y)
{
    yaw = y;
}

void Cube3D::setAxis(float p, float r, float y)
{
    pitch   = p;
    roll    = r;
    yaw     = y;
}

void Cube3D::paintGL(QGLPainter *painter)
{
    painter->setStandardEffect(QGL::LitMaterial);
    painter->setFaceColor(QGL::AllFaces, QColor(170, 202, 0));

    painter->modelViewMatrix().rotate(pitch, 1.0f, 0.0f, 0.0f);
    painter->modelViewMatrix().rotate(roll, 0.0f, 1.0f, 0.0f);
    painter->modelViewMatrix().rotate(yaw, 0.0f,0.0f, 1.0f);

    cube->draw(painter);
}

QSize Cube3D::minimumSizeHint() const
{
    return QSize(150, 150);
}

QSize Cube3D::sizeHint() const
{
    return QSize(200, 200);
}
