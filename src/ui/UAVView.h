#ifndef UAVVIEW_H
#define UAVVIEW_H

#include <QGLWidget>
#include <qglcamera.h>
#include <QPaintEvent>
#include <QTimer>
#include <QShowEvent>
#include <QHideEvent>

class QGLAbstractScene;
class QGLSceneNode;

class UAVView : public QGLWidget
{
    Q_OBJECT
public:
    UAVView(QWidget *parent = 0);
    ~UAVView();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

public slots:
    void setPitch(float);
    void setRoll(float);
    void setYaw(float);
    void setAxis(float, float, float);

    void paintUAV();
    
protected:
    //void initializeGL(QGLPainter *painter);
   // void paintGL(QGLPainter *painter);
    void initializeGL();
    void paintEvent(QPaintEvent *event);
    void resizeGL(int w, int h);

    void showEvent(QShowEvent* event);
    void hideEvent(QHideEvent* event);

private:
    QGLAbstractScene *m_scene;
    QGLSceneNode *m_rootNode;
    QGLCamera camera;
    QGLSceneNode *cube;

    float pitch;
    float roll;
    float yaw;

    QTimer* refreshTimer;
};

#endif // UAVVIEW_H
