#include "UAVView.h"
#include "qglabstractscene.h"
#include "qglscenenode.h"
#include "qglcamera.h"
#include "qglcube.h"
#include "qglpainter.h"
#include "QMatrix4x4"
#include <qglbuilder.h>
#include <qglsubsurface.h>

#include <QImage>
#include <QPainter>
#include <QDebug>

#ifndef GL_MULTISAMPLE
 #define GL_MULTISAMPLE  0x809D
 #endif

UAVView::UAVView(QWidget *parent) :
    QGLView(),
    pitch(0.0f),
    roll(0.0f),
    yaw(0.0f)
{
    setOption(QGLView::CameraNavigation,false);
    setAutoFillBackground(false);
    setMinimumSize(200, 200);

    resize(this->width(), this->height());

    uav = UAV::instance();

}

UAVView::~UAVView()
{
    delete m_scene;
}

void UAVView::showEvent(QShowEvent* event)
{
    Q_UNUSED(event);
    emit visibilityChanged(true);
    connect(uav, SIGNAL(eulerChange(double,double,double)), this, SLOT(setAxis(double,double,double)));
}

void UAVView::hideEvent(QHideEvent* event)
{
    Q_UNUSED(event);
    emit visibilityChanged(false);
    disconnect(uav, SIGNAL(eulerChange(double,double,double)), this, SLOT(setAxis(double,double,double)));
}

void UAVView::initializeGL(QGLPainter *painter)
{
    Q_UNUSED(painter);

    camera()->setEye(QVector3D(0.0f, 0.0f, 15.0f));

    m_scene = QGLAbstractScene::loadScene(":/files/obj/drone.3ds");
    m_rootNode = m_scene->mainNode();
}

void UAVView::paintGL(QGLPainter *painter)
{
    painter->modelViewMatrix().rotate(-yaw+45.0f, 0.0f,1.0f,0.0f);
    painter->modelViewMatrix().rotate(-pitch, 1.0f,0.0f,0.0f);
    painter->modelViewMatrix().rotate(-roll, 0.0f,0.0f,1.0f);
    m_rootNode->draw(painter);
}


void UAVView::setPitch(double p)
{
    pitch = p;
    updateGL();
}

void UAVView::setRoll(double r)
{
    roll = r;
    updateGL();
}

void UAVView::setYaw(double y)
{
    yaw = y;
    updateGL();
}

void UAVView::setAxis(double r, double p, double y)
{
    pitch   = p;
    roll    = r;
    yaw     = y;

    updateGL();
}

QSize UAVView::minimumSizeHint() const
{
    return QSize(150, 150);
}

QSize UAVView::sizeHint() const
{
    return QSize(200, 200);
}
