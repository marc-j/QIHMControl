#include "UAVView.h"
#include "qglabstractscene.h"
#include "qglscenenode.h"

#include <QImage>
#include <QPainter>

UAVView::UAVView(QWidget *parent) :
    QGLView(parent)
{
    this->setOption(QGLView::CameraNavigation,false);
}

UAVView::~UAVView()
{
    delete m_scene;
}

void UAVView::initializeGL(QGLPainter *painter)
{
    Q_UNUSED(painter);

    camera()->setEye(QVector3D(0.0f, 7.0f, 25.0f));

    m_scene = QGLAbstractScene::loadScene(":/files/obj/drone.3ds");
    m_rootNode = m_scene->mainNode();
}

void UAVView::paintGL(QGLPainter *painter)
{
    //painter->modelViewMatrix().rotate(45.0f,1.0f,0.0f,0.0f);
    m_rootNode->draw(painter);


    /*QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.end();*/

}
