#ifndef MYOGLWIDGET_H
#define MYOGLWIDGET_H

#include <QMouseEvent>
#include <QQuaternion>
#include <QGLWidget>
#include <QtOpenGL>

class myOGLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit myOGLWidget(QWidget *parent = 0);
    ~myOGLWidget();

signals:

public slots:

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

public slots:
    // slots for xyz-rotation slider
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);
    void rotateByQuaternion(float w, float x, float y, float z);

signals:
    // signaling rotation from mouse movement
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);

private:
    void draw();

    int xRot;
    int yRot;
    int zRot;

    float angle, axis_x, axis_y, axis_z; //for quaternion rotation

    GLdouble matrix[16];
    QPoint lastPos;
};

#endif // MYOGLWIDGET_H
