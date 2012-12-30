#ifndef LED_H
#define LED_H

#include <QWidget>
#include <QColor>

class Led : public QWidget
{
    Q_OBJECT
public:
    explicit Led(QWidget *parent = 0);
    ~Led();

    QSize sizeHint() const;
    QSize minimumSizeHint() const;

protected:
    void paintEvent(QPaintEvent *);
    int ledWidth() const;

private:
       struct Parameters {
          int darkerFactor;
          QColor color;
          bool isOn;
       };

       struct Parameters params;

public slots:
       void setColor(const QColor &color);
       void toggle();
       void turnOff();
       void turnOn();
       void turn(bool);
};

#endif // LED_H
