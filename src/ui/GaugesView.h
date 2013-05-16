#ifndef GAUGESVIEW_H
#define GAUGESVIEW_H

//#include <QtGui/QGraphicsView>
#include <QGraphicsView>
#include <QColor>
#include <QTimer>
#include <QMap>
#include <QContextMenuEvent>
#include <QMap>
#include <QPair>
#include <QStringList>
#include <cmath>

#include "UAV.h"

namespace Ui {
    class GaugesView;
}

class GaugesView : public QGraphicsView
{
    Q_OBJECT
    
public:
    explicit GaugesView(QString title="", QWidget *parent = 0);
    ~GaugesView();


    struct Gauge {
        float min;
        float max;
        QString name;
        float value;
    };

    void addGauge(QString name, float value, float min, float max);

public slots:
    void updateValue(QString name, float);
    void updateValue(QString name, double);
    void updateValue(QString name, int);

protected slots:
    void renderOverlay();
    void triggerUpdate();

protected:
    QSize sizeHint() const;
    void paintEvent(QPaintEvent* event);
    void showEvent(QShowEvent* event);
    void hideEvent(QHideEvent* event);
    float refLineWidthToPen(float line);
    float refToScreenX(float x);
    float refToScreenY(float y);
    float screenToRefX(float x);
    float screenToRefY(float y);
    void rotatePolygonClockWiseRad(QPolygonF& p, float angle, QPointF origin);
    void drawPolygon(QPolygonF refPolygon, QPainter* painter);
    void drawLine(float refX1, float refY1, float refX2, float refY2, float width, const QColor& color, QPainter* painter);
    void drawEllipse(float refX, float refY, float radiusX, float radiusY, float lineWidth, const QColor& color, QPainter* painter);
    void drawCircle(float refX, float refY, float radius, float lineWidth, const QColor& color, QPainter* painter);
    void drawGauge(float xRef, float yRef, float radius, float min, float max, const QString name, float value, const QColor& color, QPainter* painter, bool symmetric, QPair<float, float> goodRange, QPair<float, float> criticalRange, bool solid=true);
    void paintText(QString text, QColor color, float fontSize, float refX, float refY, QPainter* painter);

    void connectUAV();

    double scalingFactor;
    float vwidth;
    float vheight;

    QTimer* refreshTimer;      ///< The main timer, controls the update rate
    static const int updateInterval = 300; ///< Update interval in milliseconds

    QMap<QString,Gauge*> gaugesMap;
    QStringList* gaugesList;

    UAV* uav;
};

#endif // GAUGESVIEW_H
