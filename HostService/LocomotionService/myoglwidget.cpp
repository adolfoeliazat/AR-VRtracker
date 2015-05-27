#include "myoglwidget.h"
#include <QQuaternion>
#include <QtWidgets>


myOGLWidget::myOGLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    xRot = 25 * 8;
    yRot = 15 * 8;
    zRot = 0;

    angle = 0.0;
    axis_x = 0.0;
    axis_y = 0.0;
    axis_z = 0.0;
}

myOGLWidget::~myOGLWidget()
{

}

void myOGLWidget::initializeGL() {

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Set background color to white and opaque
    glClearDepth(1.0f);                   // Set background depth to farthest
    glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
    glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
    glShadeModel(GL_SMOOTH);   // Enable smooth shading
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections

    // First Column
    matrix[0] = 1.0;
    matrix[1] = 0.0;
    matrix[2] = 0.0;
    matrix[3] = 0.0;

    // Second Column
    matrix[4] = 0.0;
    matrix[5] = 1.0;
    matrix[6] = 0.0;
    matrix[7] = 0.0;

    // Third Column
    matrix[8] = 0.0;
    matrix[9] = 0.0;
    matrix[10] = 1.0;
    matrix[11] = 0.0;

    // Fourth Column
    matrix[12] = 0.0;
    matrix[13] = 0.0;
    matrix[14] = 0.0;
    matrix[15] = 1.0;

    static GLfloat lightPosition[4] = {0, 0, 10, 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

}

void myOGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -10.0);
    glRotatef(xRot / 8.0, 1.0, 0.0, 0.0);
    glRotatef(yRot / 8.0, 0.0, 1.0, 0.0);
    glRotatef(zRot / 8.0, 0.0, 0.0, 1.0);
    glMultMatrixd(matrix);

    draw();
}

void quaternionToMatrix(QQuaternion &q, GLdouble *matrix) {
    q.normalize();

    matrix[0] = (GLdouble)1 - 2 * (q.y() * q.y() + q.z() * q.z());
    matrix[1] = (GLdouble)2 * (q.x() * q.y() + q.z() * q.scalar());
    matrix[2] = (GLdouble)2 * (q.x() * q.z() - q.y() * q.scalar());
    matrix[3] = (GLdouble)0.0;

    matrix[4] = (GLdouble)2 * (q.x() * q.y() - q.z() * q.scalar());
    matrix[5] = (GLdouble)1 - 2 * (q.x() * q.x() + q.z() * q.z());
    matrix[6] = (GLdouble)2 * (q.z() * q.y() + q.x() * q.scalar());
    matrix[7] = (GLdouble)0.0;

    matrix[8] = (GLdouble)2 * (q.x() * q.z() + q.y() * q.scalar());
    matrix[9] = (GLdouble)2 * (q.y() * q.z() - q.x() * q.scalar());
    matrix[10] = (GLdouble)1 - 2 * (q.x() * q.x() + q.y() * q.y());
    matrix[11] = (GLdouble)0.0;

    matrix[12] = (GLdouble)0.0;
    matrix[13] = (GLdouble)0.0;
    matrix[14] = (GLdouble)0.0;
    matrix[15] = (GLdouble)1.0;
}

void myOGLWidget::rotateByQuaternion(float w, float x, float y, float z) {
    QQuaternion q(w, x, y, z);
    quaternionToMatrix(q, matrix);
    updateGL();
}

void myOGLWidget::resizeGL(int width, int height) {
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
#ifdef QT_OPENGL_ES_1
    glOrthof(-2, +2, -2, +2, 1.0, 15.0);
#else
    glOrtho(-2, +2, -2, +2, 1.0, 15.0);
#endif
    glMatrixMode(GL_MODELVIEW);
}

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360)
        angle -= 360 * 16;
}

void myOGLWidget::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != xRot) {
        xRot = angle;
        emit xRotationChanged(angle);
        updateGL();
    }
}

void myOGLWidget::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != yRot) {
        yRot = angle;
        emit yRotationChanged(angle);
        updateGL();
    }
}

void myOGLWidget::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != zRot) {
        zRot = angle;
        emit zRotationChanged(angle);
        updateGL();
    }
}

void myOGLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void myOGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(xRot + 8 * dy);
        setYRotation(yRot + 8 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(xRot + 8 * dy);
        setZRotation(zRot + 8 * dx);
    }

    lastPos = event->pos();
}

void myOGLWidget::draw() {

    glBegin(GL_QUADS);

    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f( 1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f,  1.0f);
    glVertex3f( 1.0f, 1.0f,  1.0f);

    glColor3f(1.0f, 0.5f, 0.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);

    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f( 1.0f,  1.0f, 1.0f);
    glVertex3f(-1.0f,  1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f( 1.0f, -1.0f, 1.0f);

    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);

    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);

    glColor3f(1.0f, 0.0f, 1.0f);
    glVertex3f(1.0f,  1.0f, -1.0f);
    glVertex3f(1.0f,  1.0f,  1.0f);
    glVertex3f(1.0f, -1.0f,  1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glEnd();
}
