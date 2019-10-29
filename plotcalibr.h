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
};


#endif // PLOTCALIBR_H
