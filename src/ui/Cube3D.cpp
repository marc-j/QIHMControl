#include "Cube3D.h"

#include <QDebug>
#include <cmath>

#include "qglbuilder.h"
#include "qglcube.h"

Cube3D::Cube3D(QWidget *parent) :
    QGLView(parent),
    pitch(0.0f),
    roll(0.0f),
    yaw(0.0f)
{

    this->setOption(QGLView::CameraNavigation,false);

    QGLBuilder builder;
    builder << QGL::Faceted;
    builder << QGLCube(1.5f);

    cube = builder.finalizedSceneNode();

    uav = UAV::instance();

    connect(uav, SIGNAL(eulerChange(double,double,double)), this, SLOT(setAxis(double,double,double)));
}

Cube3D::~Cube3D()
{
    delete cube;
}

void Cube3D::setPitch(double p)
{
    pitch = p;
    updateGL();
}

void Cube3D::setRoll(double r)
{
    roll = r;
    updateGL();
}

void Cube3D::setYaw(double y)
{
    yaw = y;
    updateGL();
}

void Cube3D::setAxis(double r, double p, double y)
{
    pitch   = p;
    roll    = r;
    yaw     = y;

    updateGL();
}

void Cube3D::paintGL(QGLPainter *painter)
{
    painter->setStandardEffect(QGL::LitMaterial);
    painter->setFaceColor(QGL::AllFaces, QColor(170, 202, 0));

    camera()->setEye(QVector3D(4.0f, 7.0f, 5.0f));

    painter->modelViewMatrix().rotate(pitch, 1.0f, 0.0f, 0.0f);
    painter->modelViewMatrix().rotate(roll, 0.0f, 0.0f, 1.0f);
    painter->modelViewMatrix().rotate(yaw, 0.0f,1.0f, 0.0f);

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
