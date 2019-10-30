#ifndef PLOTCALIBR_H
#define PLOTCALIBR_H

#include <qwt_plot.h>
#include <qwt_interval.h>
#include <qwt_system_clock.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_directpainter.h>
#include "helpers.h"

class PlotCalibr : public QwtPlot
{
public:
    PlotCalibr(QString title,
               QString xAxisTitle,
               QString yAxisTitle,
               QColor beginColor,
               QColor endColor,
               QWidget * parent = NULL);
    void UpdateCurves(double dataX[1024], double dataY[2][1024]);
    // Установка масштаба
    void SetScale(double Xmin, double Xmax,
                  double Ymin, double Ymax);

private:
    QwtPlotCurve* curves[2];
    void SetLegendItem();
};


#endif // PLOTCALIBR_H
