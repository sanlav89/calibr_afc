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
}
