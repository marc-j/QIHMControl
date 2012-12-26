#ifndef UAVVIEW_H
#define UAVVIEW_H

#include "qglview.h"

class QGLAbstractScene;
class QGLSceneNode;

class UAVView : public QGLView
{
    Q_OBJECT
public:
    UAVView(QWidget *parent = 0);
    ~UAVView();
    
protected:
    void initializeGL(QGLPainter *painter);
    void paintGL(QGLPainter *painter);

private:
    QGLAbstractScene *m_scene;
    QGLSceneNode *m_rootNode;
    //void paintEvent(QPaintEvent * /* event */);
    
};

#endif // UAVVIEW_H
