#include <QPen>
#include <QPainter>

#include "Led.h"

Led::Led(QWidget *parent) :
    QWidget(parent)
{

    params.darkerFactor = 300;
    params.color = Qt::green;
    params.isOn = false;
}

Led::~Led()
{

}

void Led::setColor(const QColor &color)
{
    if (params.color == color)
        return;
    params.color = color;
    update();
}

QSize Led::sizeHint() const
{
    return QSize(20, 20);
}

QSize Led::minimumSizeHint() const
{
    return QSize(16, 16);
}

void Led::toggle()
{
    params.isOn = !params.isOn;
    update();
}

void Led::turnOn()
{
    params.isOn = true;
    update();
}

void Led::turnOff()
{
    params.isOn = false;
    update();
}

void Led::turn(bool on)
{
    params.isOn = on;
    update();
}

void Led::paintEvent(QPaintEvent * /*event*/)
{
    int width = ledWidth();

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QColor color = params.isOn ? params.color
                             : params.color.darker(params.darkerFactor);

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(color);
    painter.setBrush(brush);
    // draw plain
    painter.drawEllipse(1, 1, width-1, width-1);

    QPen pen;
    pen.setWidth(2);

    int pos = width / 5 + 1;
    int lightWidth = width * 2 / 3;
    int lightQuote = 130 * 2 / (lightWidth ? lightWidth : 1) + 100;

    // draw bright spot
    while (lightWidth) {
        color = color.lighter(lightQuote);
        pen.setColor(color);
        painter.setPen(pen);
        painter.drawEllipse(pos, pos, lightWidth, lightWidth);
        lightWidth--;

        if (!lightWidth)
            break;

        painter.drawEllipse(pos, pos, lightWidth, lightWidth);
        lightWidth--;

        if (!lightWidth)
            break;

        painter.drawEllipse(pos, pos, lightWidth, lightWidth);
        pos++;
        lightWidth--;
    }

    //draw border
    painter.setBrush(Qt::NoBrush);

    int angle = -720;
    color = palette().color(QPalette::Light);

    for (int arc=120; arc<2880; arc+=240) {
        pen.setColor(color);
        painter.setPen(pen);
        int w = width - pen.width()/2;
        painter.drawArc(pen.width()/2, pen.width()/2, w, w, angle+arc, 240);
        painter.drawArc(pen.width()/2, pen.width()/2, w, w, angle-arc, 240);
        color = color.darker(110);
    }
}

int Led::ledWidth() const
{
    int width = qMin(this->width(), this->height());
    width -= 2;
    return width > 0 ? width : 0;
}
