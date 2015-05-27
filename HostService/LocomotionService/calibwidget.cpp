#include "calibwidget.h"
#include <qwt3d_enrichment_std.h>
#include <qdebug.h>

Calibwidget::Calibwidget()
{
    QFunction calib_func(this);

    calib_func.setMesh(50, 50);
    calib_func.setDomain(-1,1,-1,1);
    calib_func.setMinZ(-1);

    calib_func.create();

    setRotation(30,0,15);

    float aspect = 1;

    setScale(1, 1, aspect);
    setShift(0, 0, 0);
    setZoom(1.0);

    for (unsigned i=0; i!=coordinates()->axes.size(); ++i)
    {
        coordinates()->axes[i].setMajors(3);
        coordinates()->axes[i].setMinors(3);
    }


    coordinates()->axes[X1].setLabelString("x");
    coordinates()->axes[Y1].setLabelString("y");
    coordinates()->axes[Z1].setLabelString("z");


    setCoordinateStyle(BOX);

    //double rad, double linewidth, bool smooth, bool boxed
    CrossHair cross (0.01, 1, true, false);

    setPlotStyle(cross);

    updateData();
    updateGL();
}

void Calibwidget::addData(Qwt3D::Triple var) {
    var /= scale;
    vars.push_back(var);
}

void Calibwidget::updatePlot() {
    Qwt3D::CellField poly;
    loadFromData(vars, poly);
    updateData();
    updateGL();
}

void Calibwidget::clearPlot() {
    vars.clear();
}

void Calibwidget::setInputScale(int val) {
    scale = val;
}

void Calibwidget::updatePlotSize() {
    aspect = (float)width() / (float)height();
    setScale(1, 1, aspect);
}

void Calibwidget::resizeEvent(QResizeEvent *e) {
    updatePlotSize();
}

