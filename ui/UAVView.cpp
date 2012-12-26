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
    QGLWidget(QGLFormat(QGL::SampleBuffers),parent),
    pitch(0.0f),
    roll(0.0f),
    yaw(0.0f),
    refreshTimer(new QTimer(this))
{
    //this->setOption(QGLView::CameraNavigation,false);
    setAutoFillBackground(false);
    setMinimumSize(200, 200);

    refreshTimer->setInterval(40);
    connect(refreshTimer, SIGNAL(timeout()), this, SLOT(paintUAV()));
    refreshTimer->start();

}

UAVView::~UAVView()
{
    //delete scene;
}

void UAVView::initializeGL()
{

    QPainter p(this);
    p.beginNativePainting();

    QGLPainter painter;
    painter.begin();
    painter.setCamera(&camera);
    camera.setEye(QVector3D(-4.0f, 7.0f, 20.0f));

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);

    painter.glBlendColor(0, 0, 0, 0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);

    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glDisable(GL_LIGHTING);

    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

   /* m_scene = QGLAbstractScene::loadScene(":/files/obj/drone.3ds");
    m_rootNode = m_scene->mainNode();*/

    QGLBuilder builder;
    builder << QGL::Faceted << QGLCube(2.0f);
    cube = builder.finalizedSceneNode();

    p.endNativePainting();

//  /  qDebug() << "INIT GL";
}

void UAVView::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
}

void UAVView::paintUAV()
{
    qDebug() << "PAINT: " << this->width() << ", " << this->height();

    QPainter p(this);
    p.beginNativePainting();

    QGLPainter painter;
    painter.begin();
    painter.clearAttributes();

    makeCurrent();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glViewport(0, 0, this->width(), this->height());

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

   /* QMatrix4x4 projm;
    projm.ortho(rect());
    painter.projectionMatrix() = projm;
    painter.modelViewMatrix().setToIdentity();*/

    painter.projectionMatrix().push();
    painter.modelViewMatrix().push();

    QGLSubsurface surface(painter.currentSurface(), rect());
    painter.pushSurface(&surface);

    painter.setCamera(&camera);
    camera.setEye(QVector3D(-4.0f, 7.0f, 20.0f));

    painter.modelViewMatrix().rotate(pitch, 1.0f, 0.0f, 0.0f);
    painter.modelViewMatrix().rotate(roll, 0.0f, 1.0f, 0.0f);
    painter.modelViewMatrix().rotate(yaw, 0.0f,0.0f, 1.0f);

    //glTranslatef(-2.0f,2.0f,1.0f);
    //glRotatef(45.0f,1.0f,0.0f,0.0f);
    painter.setStandardEffect(QGL::LitMaterial);
    painter.setFaceColor(QGL::AllFaces, QColor(0, 202, 0));

    cube->draw(&painter);
   // m_rootNode->draw(&painter);


    painter.projectionMatrix().pop();
    painter.modelViewMatrix().pop();
    painter.popSurface();
    painter.end();

  /*  glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glEnable(GL_MULTISAMPLE);*/
    p.endNativePainting();

    p.setRenderHint(QPainter::Antialiasing);
    p.setRenderHint(QPainter::TextAntialiasing);
    p.setRenderHint(QPainter::HighQualityAntialiasing);
    p.setPen(Qt::white);
    p.drawText(20,20,"KIKOO LOL");
    p.end();

   //refreshTimer->stop();
}

void UAVView::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

/*void UAVView::paintGL()
{

    /*QImage image(rect().size(), QImage::Format_ARGB32);
    image.fill(0);
    QGLBuilder builder;
    builder.addPane(5.0f);
    QGLSceneNode* bg = builder.finalizedSceneNode();

    QPainter p(&image);
    p.setRenderHint(QPainter::Antialiasing);
    p.setRenderHint(QPainter::TextAntialiasing);
    p.setRenderHint(QPainter::HighQualityAntialiasing);
    //p.setPen(Qt::white);
    p.drawText(20,20,"KIKOO LOL");
    p.end();

    QGLTexture2D *tex = new QGLTexture2D();
    tex->setImage(image);
    QGLMaterial* mat = new QGLMaterial();
    mat->setTexture(tex);
    bg->setMaterial(mat);
    bg->setEffect(QGL::FlatDecalTexture2D);

    painter->clearAttributes();
    painter->modelViewMatrix().rotate(0, 1.0f, 0.0f, 0.0f);
    painter->modelViewMatrix().rotate(0, 0.0f, 1.0f, 0.0f);
    painter->modelViewMatrix().rotate(0, 0.0f, 0.0f, 1.0f);

    painter->setStandardEffect(QGL::FlatReplaceTexture2D);
    bg->draw(painter);

}*/

void UAVView::setPitch(float p)
{
    pitch = p;
}

void UAVView::setRoll(float r)
{
    roll = r;
}

void UAVView::setYaw(float y)
{
    yaw = y;
}

void UAVView::setAxis(float p, float r, float y)
{
    pitch   = p;
    roll    = r;
    yaw     = y;
}

QSize UAVView::minimumSizeHint() const
{
    return QSize(150, 150);
}

QSize UAVView::sizeHint() const
{
    return QSize(200, 200);
}
