#ifndef SENSORPLOT_H
#define SENSORPLOT_H
#include <qwt.h>
#include <qwt_plot.h>
#include <qwt_legend.h>
#include <qwt_plot_curve.h>
#include <qwt_series_data.h>
#include <qwt_plot_grid.h>
#include <qwt_legend.h>
#include <qwt_symbol.h>
#include <qwt_plot_marker.h>

class SensorPlot : public QwtPlot
{
public:
    SensorPlot(const QwtText &title, QWidget *parent);

    void addDataPoint(int16_t x_val, int16_t y_val, int16_t z_val);
    void createData(QVector<QPointF> &data);
    void dataUpdate(QVector<QPointF> &data);

private:
    QwtPlotCurve *x_axis, *y_axis, *z_axis;
    QVector<QPointF> x_data, y_data, z_data;
    QwtPlotGrid *grid;
    QwtLegend *leg;
    QwtPlotMarker *mrk_x, *mrk_y, *mrk_z;

    int data_size;

};

#endif // SENSORPLOT_H
