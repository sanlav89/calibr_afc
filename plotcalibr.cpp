//==============================================================================
// (C) Copyright 2019 MIET
// Moscow, Zelenograd, Russia
//
// Device:      DISS
// Module:      MPR
// Component:   AFC calibration utility
// File:        plotcalibr.c
// Function:    Object for displaying calibration information on a graph
// Notes:
// Author:      A.Lavrinenko
//==============================================================================
#include "plotcalibr.h"
#include <qwt_plot_grid.h>
#include <qwt_plot_layout.h>
#include <qwt_symbol.h>

//==============================================================================
/*
 * Объект для отображения информации о калибровке на графике
 */
PlotCalibr::PlotCalibr(
        QString title,
        QString xAxisTitle,
        QString yAxisTitle,
        QColor beginColor,
        QColor endColor, QWidget * parent):
    QwtPlot( parent )
{
    initCanvasDesign(
            title,
            xAxisTitle,
            yAxisTitle,
            beginColor,
            endColor);  // Инициализация элементов графика
    initCurves();       // Инициализация кривых
    initLegendItem();   // Инициализация легенды
    initMarkers();      // Инициализация маркеров
}

/*
 * Инициализация элементов графика
 */
void PlotCalibr::initCanvasDesign(
        QString title,
        QString xAxisTitle,
        QString yAxisTitle,
        QColor beginColor,
        QColor endColor
        )
{
    // Полотно
    setCanvas( new Canvas(beginColor, endColor) );
    plotLayout()->setAlignCanvasToScales( true );
    // Шрифт осей
    QFont axisFont( "Consolas", 8 );
    axisFont.setBold(false);
    setAxisFont(QwtPlot::xBottom, axisFont);
    setAxisFont(QwtPlot::yLeft, axisFont);
    QwtText t;
    if (!title.isEmpty()) {
        t.setText(title);
        t.setFont(QFont("Consolas", 10));
        setTitle(t);
    }
    if (!xAxisTitle.isEmpty()) {
        t.setText(xAxisTitle);
        t.setFont(QFont("Courier", 10, QFont::Bold));
        setAxisTitle(QwtPlot::xBottom, t);
    }
    if (!yAxisTitle.isEmpty()) {
        t.setText(yAxisTitle);
        t.setFont(QFont("Courier", 10, QFont::Bold));
        setAxisTitle(QwtPlot::yLeft, t);
    }
    // Сетка и зуммер
    QFont zoomerFont( "Consolas", 9 );
    zoomerFont.setBold(false);
    grid = new QwtPlotGrid;
    grid->enableXMin(1);
    grid->enableYMin(1);
    grid->setPen(Qt::gray, (qreal)0.0, Qt::DotLine);
    grid->attach(this);
    zoomer = new QwtPlotZoomer(this->canvas());
    zoomer->setRubberBandPen(QPen(Qt::white));
    zoomer->setTrackerPen(QPen(Qt::white));
    zoomer->setTrackerFont(zoomerFont);
}

/*
 * Инициализация кривых
 */
void PlotCalibr::initCurves()
{
    QPen curvePen[2] = {
        QPen(Qt::cyan),
        QPen(Qt::green)
    };
    for (int i = 0; i < 2; i++) {
        curves[i] = new QwtPlotCurve();
        curves[i]->setStyle(QwtPlotCurve::Lines);
        curves[i]->attach(this);
        curves[i]->setPen(curvePen[i]);
    }
    curves[0]->setTitle(" Средний спектр из SRAM ");
    curves[1]->setTitle(" Средний спектр с применением поправок ");
}

/*
 * Установить "Легенду"
 */
void PlotCalibr::initLegendItem()
{
    LegendItem* legend = new LegendItem;
    legend->attach(this);
    legend->setMaxColumns(1);
    legend->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    legend->setBackgroundMode(QwtPlotLegendItem::BackgroundMode(0));
    legend->setBorderRadius(4);
    legend->setMargin(0);
    legend->setSpacing(4);
    legend->setItemMargin(2);
    QFont legItemFont("Consolas", 10);
    legItemFont.setBold(false);
    legend->setFont(legItemFont);
}

/*
 * Инициализация маркеров
 */
void PlotCalibr::initMarkers()
{
    for (int i = 0; i < 2; i++) {
        peakMarker[i] = new QwtPlotMarker();
        peakMarker[i]->setValue( 0.0, 0.0 );
        peakMarker[i]->setLineStyle( QwtPlotMarker::Cross );
        peakMarker[i]->setLabelAlignment( Qt::AlignRight | Qt::AlignTop );
        peakMarker[i]->setLinePen( QColor( 200, 150, 0 ), 0, Qt::DashDotLine );
        peakMarker[i]->setSymbol( new QwtSymbol( QwtSymbol::Diamond,
            QColor( Qt::yellow ), QColor( Qt::green ), QSize( 8, 8 ) ) );
        peakMarker[i]->attach( this );
    }
    // Инициализация стиля текста маркеров
    peakText.setFont(QFont("Consolas", 9, QFont::Bold));
    peakText.setColor(QColor(200, 150, 0));
    // Инициализация текстового сообщения о разности мин. и макс. значения
    diffPeaksText.setRenderFlags(Qt::AlignCenter | Qt::AlignBottom);
    diffPeaksText.setFont(QFont("Consolas", 9, QFont::Bold));
    diffPeaksText.setColor(QColor(200, 150, 0));
    diffPeaksLabel = new QwtPlotTextLabel();
    diffPeaksLabel->setText( diffPeaksText );
    diffPeaksLabel->attach( this );
}

/*
 * Установить масштаб
 */
void PlotCalibr::UpdateCurves(
        double dataX[1024],
        double dataY[2][1024],
        double peakX_min,
        double peakY_min,
        double peakX_max,
        double peakY_max
        )
{
    // Пики для 2-й кривой
    QString peakLabel;
    peakLabel.sprintf("Min: %.3g дБ", peakY_min);
    peakText.setText(peakLabel);
    peakMarker[0]->setValue(peakX_min, peakY_min);
    peakMarker[0]->setLabel(peakText);
    peakLabel.sprintf("Max: %.3g дБ", peakY_max);
    peakText.setText(peakLabel);
    peakMarker[1]->setValue(peakX_max, peakY_max);
    peakMarker[1]->setLabel(peakText);
    peakLabel.sprintf("Разность: %.3g дБ", peakY_max - peakY_min);
    diffPeaksText.setText(peakLabel);
    diffPeaksLabel->setText(diffPeaksText);
    // Обновление кривых
    for (int i = 0; i < 2; i++) {
        curves[i]->setSamples(dataX, &dataY[i][0], 1024);
        replot();
    }
}

/*
 * Установить масштаб осей координат
 */
void PlotCalibr::SetScale(double Xmin, double Xmax, double Ymin, double Ymax)
{
    setAxisScale(QwtPlot::xBottom, Xmin, Xmax);
    setAxisScale(QwtPlot::yLeft, Ymin, Ymax);
    zoomer->setZoomBase();
    replot();
}
//==============================================================================
