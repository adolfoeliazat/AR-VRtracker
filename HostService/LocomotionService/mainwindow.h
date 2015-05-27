#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "calibwidget.h"
#include "myoglwidget.h"
#include "sensorplot.h"
#include "hidapi.h"
#include <mgl2/qmathgl.h>
#include <mgl2/config.h>
#include <QMainWindow>
#include <QtWidgets>
#include <cstdint>
#include <tchar.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

namespace Ui {
class MainWindow;
}

#pragma pack(push, 1)
struct dataexchange_t {
    float qr_w, qr_x, qr_y, qr_z; //16 bytes
    int16_t mag_x, mag_y, mag_z; //6 bytes
    int16_t acc_x, acc_y, acc_z;
    int16_t gyr_x, gyr_y, gyr_z;
    float time_period; //4 bytes
    float d1, d2, d3, d4, d5;
    uint8_t d6;
    uint8_t btn; //1 byte
}; //60 bytes
struct params_t {
    uint8_t reportID;
    float indicator;
    float offset_x, offset_y, offset_z;
    float rmat_r1_x, rmat_r1_y, rmat_r1_z;
    float rmat_r2_x, rmat_r2_y, rmat_r2_z;
    float rmat_r3_x, rmat_r3_y, rmat_r3_z;
    float scale_x, scale_y, scale_z;
}; //65 bytes
#pragma pack(pop)
//note: auto alignment disabled


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool checkSingle();

protected:
    void closeEvent(QCloseEvent *event);

private:
    void createTrayIcon();
    void createActions();

    int createMemFile();
    void deleteMemFile();

    Ui::MainWindow *ui;

    Qt::WindowFlags flags;

    QIcon *app_icon, *restore_icon, *exit_icon;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    QTimer* timer;

    std::vector<int16_t> mag_x_arr, mag_y_arr, mag_z_arr;
    SensorPlot *mag_pl, *acc_pl, *gyr_pl;

    QSharedMemory *shmem; //for single instance checking

    QTabWidget *tabs;
    myOGLWidget *ogl_wdg;
    QPushButton *connect_button, *pause_button, *quit_button, \
    *calib_start_button, *calib_apply_button, *calib_upload_button;
    QGridLayout *main_lay;

    hid_device *handle_device;
    dataexchange_t data;
    params_t calib_params;
    Calibwidget *mag_plot;

    HANDLE hMapFile;
    LPCTSTR pBuf;

    QAction *connectAction, *disconnectAction;
    QAction *restoreAction;
    QAction *quitAction;

    bool is_connected, is_calibration_started, is_paused, is_params_applyed;

private slots:
    void connectSlot();
    void readDeviceSlot();
    void pauseSlot();
    void calibrationSlot();
    void applySlot();
    void sendParamsSlot();
};

#endif // MAINWINDOW_H
