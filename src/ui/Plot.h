#ifndef PLOT_H
#define PLOT_H

#include <QWidget>
#include <QHideEvent>
#include <QShowEvent>
#include "ui/QtPlot/qcustomplot.h"

namespace Ui {
class Plot;
}

class Plot : public QWidget
{
    Q_OBJECT
    
public:
    explicit Plot(QWidget *parent = 0);
    ~Plot();

signals:
    void visibilityChanged(bool);
    
private:
    QCustomPlot *plot;

    void drawUI();
    void drawPlot();

    QList<QColor>* colors;

    void showEvent(QShowEvent* event);
    void hideEvent(QHideEvent* event);
};

#endif // PLOT_H
