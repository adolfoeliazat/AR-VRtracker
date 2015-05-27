#include "sensorplot.h"

SensorPlot::SensorPlot(const QwtText &title, QWidget *parent) : QwtPlot(title, parent) {
    x_axis = new QwtPlotCurve("X");
    y_axis = new QwtPlotCurve("Y");
    z_axis = new QwtPlotCurve("Z");

    x_axis->attach(this);
    y_axis->attach(this);
    z_axis->attach(this);

    x_axis->setPen(Qt::red, 2.5, Qt::SolidLine);
    y_axis->setPen(Qt::green, 2.5, Qt::SolidLine);
    z_axis->setPen(Qt::blue, 2.5, Qt::SolidLine);

    data_size = 50;
    setAutoReplot();

    createData(x_data);
    createData(y_data);
    createData(z_data);

    x_axis->setSamples(x_data);
    y_axis->setSamples(y_data);
    z_axis->setSamples(z_data);

    //legend and grid
    grid = new QwtPlotGrid;
    grid->enableXMin(true);
    grid->setMajorPen(QPen(Qt::black, 0,Qt::DotLine));
    grid->setMinorPen(QPen(Qt::gray, 0,Qt::DotLine));
    grid->attach(this);

    leg = new QwtLegend();
    this->insertLegend(leg, QwtPlot::RightLegend);

    mrk_x = new QwtPlotMarker();
    mrk_x->setLineStyle(QwtPlotMarker::HLine);
    mrk_x->setLabelAlignment(Qt::AlignRight | Qt::AlignCenter);
    mrk_x->setLinePen(QPen(QColor(Qt::red), 0, Qt::DashDotLine));
    mrk_x->setSymbol( new QwtSymbol(QwtSymbol::Ellipse, QColor(Qt::red), QColor(Qt::black), QSize(5, 5)));
    mrk_x->attach(this);

    mrk_y = new QwtPlotMarker();
    mrk_y->setLineStyle(QwtPlotMarker::HLine);
    mrk_y->setLabelAlignment(Qt::AlignRight | Qt::AlignCenter);
    mrk_y->setLinePen(QPen(QColor(Qt::green), 0, Qt::DashDotLine));
    mrk_y->setSymbol( new QwtSymbol(QwtSymbol::Ellipse, QColor(Qt::green), QColor(Qt::black), QSize(5, 5)));
    mrk_y->attach(this);

    mrk_z = new QwtPlotMarker();
    mrk_z->setLineStyle(QwtPlotMarker::HLine);
    mrk_z->setLabelAlignment(Qt::AlignRight | Qt::AlignCenter);
    mrk_z->setLinePen(QPen(QColor(Qt::blue), 0, Qt::DashDotLine));
    mrk_z->setSymbol( new QwtSymbol(QwtSymbol::Ellipse, QColor(Qt::blue), QColor(Qt::black), QSize(5, 5)));
    mrk_z->attach(this);

    mrk_x->setValue(data_size, 0);
    mrk_y->setValue(data_size, 0);
    mrk_z->setValue(data_size, 0);

    enableAxis(2, false);
    canvas()->setCursor(Qt::ArrowCursor);
}

void SensorPlot::addDataPoint(int16_t x_val, int16_t y_val, int16_t z_val) {

    QPointF x_sample(x_data.size(), x_val);
    QPointF y_sample(y_data.size(), y_val);
    QPointF z_sample(z_data.size(), z_val);

    x_data.push_back(x_sample);
    y_data.push_back(y_sample);
    z_data.push_back(z_sample);

    dataUpdate(x_data);
    dataUpdate(y_data);
    dataUpdate(z_data);

    mrk_x->setValue(x_data.end()->x(), x_data.end()->y());
    mrk_y->setValue(y_data.end()->x(), y_data.end()->y());
    mrk_z->setValue(z_data.end()->x(), z_data.end()->y());

    x_axis->setSamples(x_data);
    y_axis->setSamples(y_data);
    z_axis->setSamples(z_data);

}

void SensorPlot::createData(QVector<QPointF> &data) {
    for (int i = 0; i < data_size; i++) {
        data.append(QPointF(data.count(), 0.0));
    }
}

void SensorPlot::dataUpdate(QVector<QPointF> &data) {
    if (data.length() > data_size) {
        data.pop_front();
        QVector<QPointF>::iterator i = data.begin();
        while (i != data.end()) {
            i->setX(i->x() - 1);
            ++i;
        }
    }
}
