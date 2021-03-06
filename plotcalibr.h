//==============================================================================
// (C) Copyright 2019 MIET
// Moscow, Zelenograd, Russia
//
// Device:      DISS
// Module:      MPR
// Component:   AFC calibration utility
// File:        plotcalibr.h
// Function:    Object for displaying calibration information on a graph
// Notes:
// Author:      A.Lavrinenko
//==============================================================================
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

/*
 * Объект для отображения информации о калибровке на графике
 */
class PlotCalibr : public QwtPlot
{
public:
    PlotCalibr(QString title,       // Название графика
               QString xAxisTitle,  // Название оси X
               QString yAxisTitle,  // Название оси Y
               QColor beginColor,   // Начальный цвет фона (градиент)
               QColor endColor,     // Конечный цвет фона (градиент)
               QWidget* parent = NULL);
    // Обновление данных на графиках
    void UpdateCurves(
            double dataX[1024],     // Ось X
            double dataY[2][1024],  // Ось Y
            double peakX_min,       // Координата X первого маркера (минимума)
            double peakY_min,       // Координата Y первого маркера (минимума)
            double peakX_max,       // Координата X первого маркера (максимума)
            double peakY_max        // Координата Y первого маркера (максимума)
            );
    // Установка масштаба
    void SetScale(double Xmin, double Xmax, double Ymin, double Ymax);

private:
    // Объекты на графике
    QwtPlotCurve* curves[2];            // Кривые
    QwtPlotMarker* peakMarker[2];       // Маркеры (максимум и минимум 2 кривой)
    QwtText peakText;                   // Текст для маркеров
    QwtPlotTextLabel* diffPeaksLabel;   // Лэйбл на графике (разность пиков)
    QwtText diffPeaksText;              // Текст для лэйбла на графике
    QwtPlotGrid* grid;                  // Сетка на графике
    QwtPlotZoomer* zoomer;              // Зуммер
    // Инициализация объектов графика
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
//==============================================================================

#endif // PLOTCALIBR_H
