#ifndef CALIBWIDGET_H
#define CALIBWIDGET_H

#include <math.h>
#include <qapplication.h>
#include <qwt3d_surfaceplot.h>
#include <qwt3d_function.h>
#include <qwt3d_plot.h>


using namespace Qwt3D;

class QFunction : public Function
{
public:

    QFunction(SurfacePlot* pw)
        :Function(pw)
    {
    }

    double operator()(double x, double y)
    {
        if (x <= 0)
            return sqrt(1 - x*x - y*y);
        else
            return -sqrt(1 - x*x - y*y);
    }
};

class Calibwidget : public SurfacePlot
{
public:
    Calibwidget();

    void addData(Qwt3D::Triple);
    void updatePlot();
    void updatePlotSize();
    void clearPlot();
    void setInputScale(int);

private:
    Qwt3D::TripleField vars;
    int scale;
    float aspect;

protected:
    void resizeEvent(QResizeEvent *e);

};

#endif // CALIBWIDGET_H
