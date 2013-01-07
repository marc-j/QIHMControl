#ifndef HUD_H
#define HUD_H

#include <QWidget>
#include <QPainter>

class HUD : public QWidget
{
    Q_OBJECT
public:
    explicit HUD(QWidget *parent = 0);
    
    void paintEvent(QPaintEvent *);

signals:
    void visibilityChanged(bool);
    
public slots:

protected:
    void showEvent(QShowEvent* event);
    void hideEvent(QHideEvent* event);

    float xCenterOffset, yCenterOffset;
    float vwidth;
    float vheight;
    
};

#endif // HUD_H
