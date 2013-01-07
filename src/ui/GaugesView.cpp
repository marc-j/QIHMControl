#include "GaugesView.h"

GaugesView::GaugesView(QString title, QWidget *parent) :
    QGraphicsView(parent),
    vwidth(80.0f),
    vheight(80.0f),
    refreshTimer(new QTimer(this)),
    gaugesList(new QStringList())
{
    setWindowTitle(title);
    setAutoFillBackground(true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setMinimumHeight(100);
    this->setMinimumWidth(250);

    scalingFactor = this->width()/vwidth;

    // Refresh timer
    refreshTimer->setInterval(180); //
    connect(refreshTimer, SIGNAL(timeout()), this, SLOT(triggerUpdate()));

    connectUAV();
}

GaugesView::~GaugesView()
{
    if(this->refreshTimer)
    {
        delete this->refreshTimer;
    }

    if(this->gaugesList) {
        delete this->gaugesList;
    }
}

void GaugesView::addGauge(QString name, float value, float min, float max)
{

    if (!gaugesList->contains(name) ) {
        Gauge* tmpGauge = new Gauge();
        tmpGauge->min = min;
        tmpGauge->max = max;
        tmpGauge->name = name;
        tmpGauge->value = value;

        gaugesMap.insert(name, tmpGauge);
        gaugesList->append(name);

        update();
    }
}

void GaugesView::updateValue(QString name, float value)
{
    if (gaugesMap.contains(name)) {
        gaugesMap.value(name)->value = value;
        update();
    }
}

void GaugesView::updateValue(QString name, double value)
{
    updateValue(name, (float) value);
}

void GaugesView::updateValue(QString name, int value)
{
    updateValue(name, (float) value);
}


QSize GaugesView::sizeHint() const
{
    return QSize(400, 400.0f*(vwidth/vheight)*1.2f);
}

void GaugesView::paintEvent(QPaintEvent * event)
{
    Q_UNUSED(event);
    renderOverlay();
}

void GaugesView::renderOverlay()
{
    if (!isVisible()) return;

    scalingFactor = this->width()/vwidth;

    double scalingFactorH = this->height()/vheight;
    if (scalingFactorH < scalingFactor) scalingFactor = scalingFactorH;

    QPainter painter(viewport());
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::HighQualityAntialiasing, true);

    int columns = 3;
    const float spacing = 0.4f; // 40% of width
    const float gaugeWidth = vwidth / (((float)columns) + (((float)columns+1) * spacing + spacing * 0.5f));
    const QColor gaugeColor = QColor(200, 200, 200);

    float leftSpacing = gaugeWidth * spacing;
    float xCoord = leftSpacing + gaugeWidth/2.0f;

    float topSpacing = leftSpacing;
    float yCoord = topSpacing + gaugeWidth/2.0f;

    for (int i = 0; i < gaugesList->length(); ++i)
    {
        if (!gaugesMap.contains(gaugesList->at(i))) {
            continue;
        }

        Gauge *tmpGauge = gaugesMap.value(gaugesList->at(i));
        drawGauge(xCoord, yCoord, gaugeWidth/2.0f, tmpGauge->min, tmpGauge->max, tmpGauge->name, tmpGauge->value, gaugeColor, &painter, false, qMakePair(0.0f, 0.5f), qMakePair(0.7f, 1.0f), true);
        xCoord += gaugeWidth + leftSpacing;

        // Move one row down if necessary
        if (xCoord + gaugeWidth*0.9f > vwidth)
        {
            yCoord += topSpacing + gaugeWidth;
            xCoord = leftSpacing + gaugeWidth/2.0f;
        }
    }
}

void GaugesView::drawGauge(float xRef, float yRef, float radius, float min, float max, QString name, float value, const QColor& color, QPainter* painter, bool symmetric, QPair<float,float> goodRange, QPair<float,float> criticalRange, bool solid)
{
    QPen circlePen(Qt::SolidLine);

    float zeroRotation;
    if (symmetric) {
        zeroRotation = 1.35f;
    } else {
        zeroRotation = 0.49f;
    }

    // Scale the rotation so that the gauge does one revolution
    // per max. change
    float rangeScale;
    if (symmetric) {
        rangeScale = ((2.0f * M_PI) / (max - min)) * 0.57f;
    } else {
        rangeScale = ((2.0f * M_PI) / (max - min)) * 0.72f;
    }

    const float scaledValue = (value-min)*rangeScale;

    float nameHeight = radius / 2.6f;
    paintText(name.toUpper(), color, nameHeight*0.7f, xRef-radius, yRef-radius, painter);

    // Ensure some space
    nameHeight *= 1.2f;

    if (!solid) {
        circlePen.setStyle(Qt::DotLine);
    }
    circlePen.setWidth(refLineWidthToPen(radius/12.0f));
    circlePen.setColor(color);

    if (symmetric) {
        circlePen.setStyle(Qt::DashLine);
    }
    painter->setBrush(Qt::NoBrush);
    painter->setPen(circlePen);
    drawCircle(xRef, yRef+nameHeight, radius, 0.0f, color, painter);

    QString label;
    label.sprintf("% 06.1f", value);

    // Text
    // height
    const float textHeight = radius/2.1f;
    const float textX = xRef-radius/3.0f;
    const float textY = yRef+radius/2.0f;

    // Draw background rectangle
    QBrush brush(QColor(51, 51, 51), Qt::SolidPattern);
    painter->setBrush(brush);
    painter->setPen(Qt::NoPen);

    if (symmetric) {
        painter->drawRect(refToScreenX(xRef-radius), refToScreenY(yRef+nameHeight+radius/4.0f), refToScreenX(radius+radius), refToScreenY((radius - radius/4.0f)*1.2f));
    } else {
        painter->drawRect(refToScreenX(xRef-radius/2.5f), refToScreenY(yRef+nameHeight+radius/4.0f), refToScreenX(radius+radius/2.0f), refToScreenY((radius - radius/4.0f)*1.2f));
    }

    paintText(label, QColor(255, 255, 255), textHeight, textX, textY+nameHeight, painter);
    // Draw the needle

    const float maxWidth = radius / 6.0f;
    const float minWidth = maxWidth * 0.3f;

    QPolygonF p(6);

    p.replace(0, QPointF(xRef-maxWidth/2.0f, yRef+nameHeight+radius * 0.05f));
    p.replace(1, QPointF(xRef-minWidth/2.0f, yRef+nameHeight+radius * 0.89f));
    p.replace(2, QPointF(xRef+minWidth/2.0f, yRef+nameHeight+radius * 0.89f));
    p.replace(3, QPointF(xRef+maxWidth/2.0f, yRef+nameHeight+radius * 0.05f));
    p.replace(4, QPointF(xRef,               yRef+nameHeight+radius * 0.0f));
    p.replace(5, QPointF(xRef-maxWidth/2.0f, yRef+nameHeight+radius * 0.05f));


    rotatePolygonClockWiseRad(p, scaledValue+zeroRotation, QPointF(xRef, yRef+nameHeight));

    QBrush indexBrush;
    indexBrush.setColor(color);
    indexBrush.setStyle(Qt::SolidPattern);
    painter->setPen(Qt::NoPen);
    painter->setBrush(indexBrush);
    drawPolygon(p, painter);

}

/**
 * Rotate a polygon around a point
 *
 * @param p polygon to rotate
 * @param origin the rotation center
 * @param angle rotation angle, in radians
 * @return p Polygon p rotated by angle around the origin point
 */
void GaugesView::rotatePolygonClockWiseRad(QPolygonF& p, float angle, QPointF origin)
{
    // Standard 2x2 rotation matrix, counter-clockwise
    //
    //   |  cos(phi)   sin(phi) |
    //   | -sin(phi)   cos(phi) |
    //
    for (int i = 0; i < p.size(); i++) {
        QPointF curr = p.at(i);

        const float x = curr.x();
        const float y = curr.y();

        curr.setX(((cos(angle) * (x-origin.x())) + (-sin(angle) * (y-origin.y()))) + origin.x());
        curr.setY(((sin(angle) * (x-origin.x())) + (cos(angle) * (y-origin.y()))) + origin.y());
        p.replace(i, curr);
    }
}

void GaugesView::drawPolygon(QPolygonF refPolygon, QPainter* painter)
{
    // Scale coordinates
    QPolygonF draw(refPolygon.size());
    for (int i = 0; i < refPolygon.size(); i++) {
        QPointF curr;
        curr.setX(refToScreenX(refPolygon.at(i).x()));
        curr.setY(refToScreenY(refPolygon.at(i).y()));
        draw.replace(i, curr);
    }
    painter->drawPolygon(draw);
}


float GaugesView::refLineWidthToPen(float line)
{
    return line * 2.50f;
}

/**
 * @param y coordinate in pixels to be converted to reference mm units
 * @return the screen coordinate relative to the QGLWindow origin
 */
float GaugesView::refToScreenX(float x)
{
    return (scalingFactor * x);
}

/**
 * @param x coordinate in pixels to be converted to reference mm units
 * @return the screen coordinate relative to the QGLWindow origin
 */
float GaugesView::refToScreenY(float y)
{
    return (scalingFactor * y);
}

float GaugesView::screenToRefX(float x)
{
    return x/scalingFactor;
}

float GaugesView::screenToRefY(float y)
{
    return y/scalingFactor;
}

void GaugesView::drawLine(float refX1, float refY1, float refX2, float refY2, float width, const QColor& color, QPainter* painter)
{
    QPen pen(Qt::SolidLine);
    pen.setWidth(refLineWidthToPen(width));
    pen.setColor(color);
    painter->setPen(pen);
    painter->drawLine(QPoint(refToScreenX(refX1), refToScreenY(refY1)), QPoint(refToScreenX(refX2), refToScreenY(refY2)));
}

void GaugesView::drawEllipse(float refX, float refY, float radiusX, float radiusY, float lineWidth, const QColor& color, QPainter* painter)
{
    QPen pen(painter->pen().style());
    pen.setWidth(refLineWidthToPen(lineWidth));
    pen.setColor(color);
    painter->setPen(pen);
    painter->drawEllipse(QPointF(refToScreenX(refX), refToScreenY(refY)), refToScreenX(radiusX), refToScreenY(radiusY));
}

void GaugesView::drawCircle(float refX, float refY, float radius, float lineWidth, const QColor& color, QPainter* painter)
{
    drawEllipse(refX, refY, radius, radius, lineWidth, color, painter);
}

/**
 * Paint text on top of the image and OpenGL drawings
 *
 * @param text chars to write
 * @param color text color
 * @param fontSize text size in mm
 * @param refX position in reference units (mm of the real instrument). This is relative to the measurement unit position, NOT in pixels.
 * @param refY position in reference units (mm of the real instrument). This is relative to the measurement unit position, NOT in pixels.
 */
void GaugesView::paintText(QString text, QColor color, float fontSize, float refX, float refY, QPainter* painter)
{
    QPen prevPen = painter->pen();
    float pPositionX = refToScreenX(refX) - (fontSize*scalingFactor*0.072f);
    float pPositionY = refToScreenY(refY) - (fontSize*scalingFactor*0.212f);

    QFont font("Bitstream Vera Sans");
    // Enforce minimum font size of 5 pixels
    int fSize = qMax(5, (int)(fontSize*scalingFactor*1.26f));
    font.setPixelSize(fSize);

    QFontMetrics metrics = QFontMetrics(font);
    int border = qMax(4, metrics.leading());
    QRect rect = metrics.boundingRect(0, 0, width() - 2*border, int(height()*0.125),
                                      Qt::AlignLeft | Qt::TextWordWrap, text);
    painter->setPen(color);
    painter->setFont(font);
    painter->setRenderHint(QPainter::TextAntialiasing);
    painter->drawText(pPositionX, pPositionY,
                      rect.width(), rect.height(),
                      Qt::AlignCenter | Qt::TextWordWrap, text);
    painter->setPen(prevPen);
}

void GaugesView::connectUAV()
{
    uav = UAV::instance();
    connect(uav, SIGNAL(updateValue(QString,double)), this, SLOT(updateValue(QString,double)));
    connect(uav, SIGNAL(updateValue(QString,float)), this, SLOT(updateValue(QString,float)));
    connect(uav, SIGNAL(updateValue(QString,int)), this, SLOT(updateValue(QString,int)));
}

void GaugesView::showEvent(QShowEvent* event)
{
    // React only to internal (pre-display)
    // events
    Q_UNUSED(event);
    refreshTimer->start(updateInterval);
}

void GaugesView::hideEvent(QHideEvent* event)
{
    // React only to internal (pre-display)
    // events
    Q_UNUSED(event);
    refreshTimer->stop();
    //saveState();
}

void GaugesView::triggerUpdate()
{
    // Only repaint the regions necessary
    update(this->geometry());
}

