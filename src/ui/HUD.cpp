#include "HUD.h"

HUD::HUD(QWidget *parent) :
    QWidget(parent),
    xCenterOffset(0.0f),
    yCenterOffset(0.0f),
    vwidth(150.0f),
    vheight(150.0f)
{
    setAutoFillBackground(false);
    setMinimumSize(150, 150);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void HUD::showEvent(QShowEvent* event)
{
    Q_UNUSED(event);
    emit visibilityChanged(true);
}

void HUD::hideEvent(QHideEvent* event)
{
    Q_UNUSED(event);
    emit visibilityChanged(false);
}

void HUD::paintEvent(QPaintEvent *)
{

    double scalingFactor = this->width()/vwidth;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::HighQualityAntialiasing, true);

    /*      QPen linePen(Qt::SolidLine);
            linePen.setWidth(refLineWidthToPen(1.0f));
            linePen.setColor(defaultColor);
            painter.setBrush(Qt::NoBrush);
            painter.setPen(linePen);*/

    // Draw Background
    painter.setPen(QPen(Qt::black));
    painter.fillRect(rect(),Qt::black);

    painter.translate((this->width()/2.0+xCenterOffset), (this->height()/2.0+yCenterOffset));

    QPen linePen(Qt::SolidLine);
    linePen.setWidth(1.0f);
    linePen.setColor(Qt::green);

    // Draw Horizon
    int horizonW = 60*scalingFactor;
    int horizonH = 60*scalingFactor;
    QImage img = QImage(horizonW,horizonH,QImage::Format_ARGB32);
    QPainter p(&img);
    p.translate(horizonW/2,horizonH/2);

    //p.rotate(45.0f);
    //p.translate(0,10.0f);

    p.fillRect(-300,-300,600,300,QColor::fromRgb(0,153,204));
    p.fillRect(-300,0,600,300,QColor::fromRgb(179,102,0));
    p.end();

    QImage imgHorizon = QImage(horizonW,horizonH,QImage::Format_ARGB32);
    QPainter pHorizon(&imgHorizon);

    pHorizon.drawImage(0,0,img);


    pHorizon.setPen(linePen);
    pHorizon.translate(0,(horizonH/2.0));
    pHorizon.drawRect(0,-2*scalingFactor,horizonW,+4*scalingFactor);

    QPen linePenDashed(Qt::DashLine);
    linePenDashed.setWidth(1.0f);
    linePenDashed.setColor(Qt::green);
    pHorizon.setPen(linePenDashed);
    pHorizon.drawLine(0,0,horizonW,0);
    pHorizon.setPen(linePen);

    pHorizon.end();

    painter.drawImage(-(horizonW/2.0),-(horizonH/2.0),imgHorizon);

    painter.setPen(linePen);
    painter.setBrush(QColor(255,255,255,10));

    QRect altRect((horizonW/2.0)+(3*scalingFactor),-(horizonH/2.0)+(5*scalingFactor),10*scalingFactor,horizonH-(10*scalingFactor));
    painter.drawRect(altRect);

    QPolygon feetPolyFill(7);
    feetPolyFill.setPoint(0,altRect.right()+1,altRect.top());
    feetPolyFill.setPoint(1,altRect.right()+(10*scalingFactor)+1,altRect.top());
    feetPolyFill.setPoint(2,altRect.right()+(10*scalingFactor)+1,altRect.center().y()-(4*scalingFactor));
    feetPolyFill.setPoint(3,altRect.right()+1,altRect.center().y());
    feetPolyFill.setPoint(4,altRect.right()+(10*scalingFactor)+1,altRect.center().y()+(4*scalingFactor));
    feetPolyFill.setPoint(5,altRect.right()+(10*scalingFactor)+1,altRect.bottom()+1);
    feetPolyFill.setPoint(6,altRect.right()+1,altRect.bottom()+1);
    //painter.drawPolyline(feetPolyFill);
    painter.drawPolygon(feetPolyFill);

    painter.setBrush(QColor(0,0,0));
    painter.setPen(QPen(Qt::white));
    int feetPos = 0;
    if (feetPos < (altRect.top()+(1.5*scalingFactor)) ) {
        feetPos = altRect.top()+(1.5*scalingFactor);
    } else if( feetPos > (altRect.bottom()-(1.5*scalingFactor)) ) {
        feetPos = altRect.bottom()-(1.5*scalingFactor);
    }

    QPolygon feetPolyIndicator(6);
    feetPolyIndicator.setPoint(0,altRect.right()+1,feetPos+altRect.center().y());
    feetPolyIndicator.setPoint(1,altRect.right()+(4*scalingFactor)+1,feetPos+altRect.center().y()-(1.5*scalingFactor));
    feetPolyIndicator.setPoint(2,altRect.right()+(10*scalingFactor)+1,feetPos+altRect.center().y()-(1.5*scalingFactor));
    feetPolyIndicator.setPoint(3,altRect.right()+(10*scalingFactor)+1,feetPos+altRect.center().y()+(1.5*scalingFactor));
    feetPolyIndicator.setPoint(4,altRect.right()+(4*scalingFactor)+1,feetPos+altRect.center().y()+(1.5*scalingFactor));
    feetPolyIndicator.setPoint(5,altRect.right()+1,feetPos+altRect.center().y());
    //painter.drawPolyline(feetPolyFill);
    painter.drawPolygon(feetPolyIndicator);

    painter.setBrush(QColor(255,255,255,10));
    painter.setPen(linePen);

    QRect vSpeedRectText(altRect.left(),altRect.top()-(5*scalingFactor),altRect.width(),5*scalingFactor);
    painter.drawRect(vSpeedRectText);
    painter.drawText(vSpeedRectText,Qt::AlignCenter | Qt::TextWordWrap,"METER");

    QRect vSpeedRectText2(altRect.right()+1,altRect.top()-(5*scalingFactor),altRect.width(),5*scalingFactor);
    painter.drawRect(vSpeedRectText2);
    painter.drawText(vSpeedRectText2,Qt::AlignCenter | Qt::TextWordWrap,"/min");

    QRect altRectText(altRect.left(),altRect.bottom()+1,altRect.width(),5*scalingFactor);
    painter.drawRect(altRectText);
    painter.drawText(altRectText,Qt::AlignCenter | Qt::TextWordWrap,"ALT");

    painter.drawRect(-((horizonW/2.0)+(10*scalingFactor)+(3*scalingFactor)),-(horizonH/2.0),10*scalingFactor,horizonH);

    //

    // Draw Gauge

    painter.end();

}
