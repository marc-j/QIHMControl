#ifndef UAVVIEW_H
#define UAVVIEW_H

//#include "qglview.h"
#include <Qt3D/qglview.h>
#include <Qt3D/qglcamera.h>
#include <QPaintEvent>
#include <QTimer>
#include <QShowEvent>
#include <QHideEvent>

#include "UAV.h"

class QGLAbstractScene;
class QGLSceneNode;

class UAVView : public QGLView
{
    Q_OBJECT
public:
    UAVView(QWidget *parent = 0);
    ~UAVView();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

signals:
    void visibilityChanged(bool);

public slots:
    void setPitch(double);
    void setRoll(double);
    void setYaw(double);
    void setAxis(double, double, double);
    
protected:
    void initializeGL(QGLPainter *painter);
    void paintGL(QGLPainter *painter);

    void showEvent(QShowEvent* event);
    void hideEvent(QHideEvent* event);

private:
    QGLAbstractScene *m_scene;
    QGLSceneNode *m_rootNode;

    double pitch;
    double roll;
    double yaw;

    UAV* uav;
};

#endif // UAVVIEW_H
