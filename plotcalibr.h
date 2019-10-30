#ifndef PLOTCALIBR_H
#define PLOTCALIBR_H

#include <qwt_plot.h>
#include <qwt_interval.h>
#include <qwt_system_clock.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_directpainter.h>
#include <qwt_plot_textlabel.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_zoomer.h>
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
    // Обновление данных на графиках
    void UpdateCurves(
            double dataX[1024],
            double dataY[2][1024],
            double peakX_min,
            double peakY_min,
            double peakX_max,
            double peakY_max
            );
    // Установка масштаба
    void SetScale(double Xmin, double Xmax, double Ymin, double Ymax);

private:
    QwtPlotCurve* curves[2];
    QwtPlotMarker* peakMarker[2];
    QwtText peakText;
    QwtPlotTextLabel* diffPeaksLabel;
    QwtText diffPeaksText;
    QwtPlotGrid* grid;
    QwtPlotZoomer* zoomer;
    void initCanvasDesign(
            QString title,
            QString xAxisTitle,
            QString yAxisTitle,
            QColor beginColor,
            QColor endColor
            );
    void initCurves();
    void initLegendItem();
    void initMarkers();

};


#endif // PLOTCALIBR_H
