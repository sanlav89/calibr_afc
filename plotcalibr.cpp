#include "plotcalibr.h"
#include <qwt_plot_grid.h>
#include <qwt_plot_layout.h>

PlotCalibr::PlotCalibr(
        QString title,
        QString xAxisTitle,
        QString yAxisTitle,
        QColor beginColor,
        QColor endColor, QWidget * parent):
    QwtPlot( parent )
{
    QString str;

    setCanvas( new Canvas(beginColor, endColor) );
    plotLayout()->setAlignCanvasToScales( true );

    // Инициализация элементов графика
    // Сетка
    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->setPen( Qt::gray, 0.0, Qt::DotLine );
    grid->enableX( true );
    grid->enableXMin( true );
    grid->enableY( true );
    grid->enableYMin( false );
    grid->attach( this );
    // Шрифт осей
    QFont axisFont( "Helvetica", 8 );
    axisFont.setBold(true);
    setAxisFont(QwtPlot::xBottom, axisFont);
    setAxisFont(QwtPlot::yLeft, axisFont);

    QwtText t;

    if (!title.isEmpty()) {
        t.setText(title);
        t.setFont(QFont("Helvetica", 10));
        setTitle(t);
    }
    if (!xAxisTitle.isEmpty()) {
        t.setText(xAxisTitle);
        t.setFont(QFont("Courier", 10));
        setAxisTitle(QwtPlot::xBottom, t);
    }
    if (!yAxisTitle.isEmpty()) {
        t.setText(yAxisTitle);
        t.setFont(QFont("Courier", 10));
        setAxisTitle(QwtPlot::yLeft, t);
    }

    // Инициализация кривых
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

    curves[0]->setTitle("Средний спектр из SRAM");
    curves[1]->setTitle("Средний спектр с применением поправок");

    // Инициализация легенд
    SetLegendItem();
}

// Установить масштаб
void PlotCalibr::UpdateCurves(double dataX[1024], double dataY[2][1024])
{
    // Обновление кривых
    for (int i = 0; i < 2; i++) {
        curves[i]->setSamples(dataX, &dataY[i][0], 1024);
        replot();
    }
}

// Установить масштаб осей координат
void PlotCalibr::SetScale(double Xmin, double Xmax, double Ymin, double Ymax)
{
    setAxisScale(QwtPlot::xBottom, Xmin, Xmax);
    setAxisScale(QwtPlot::yLeft, Ymin, Ymax);
    replot();
}

// Установить "Легенду"
void PlotCalibr::SetLegendItem()
{
    LegendItem * legend = new LegendItem;
    legend->attach(this);
    legend->setMaxColumns(1);
    legend->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    legend->setBackgroundMode(QwtPlotLegendItem::BackgroundMode(0));
    legend->setBorderRadius(4);
    legend->setMargin(0);
    legend->setSpacing(4);
    legend->setItemMargin(2);
    QFont legItemFont("Helvetica", 8);
    legItemFont.setBold(false);
    legend->setFont(legItemFont);
}
