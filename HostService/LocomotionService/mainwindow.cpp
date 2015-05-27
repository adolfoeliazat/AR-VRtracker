#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qwt.h>
#include <qwt_plot.h>
#include <armadillo>
#include <armadillo_bits/config.hpp>
#include <QtWidgets>
#include <iostream>
#include <algorithm>
#include <vector>

#define BUF_SIZE 256
TCHAR szName[]=TEXT("MyFileMappingObject");

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    is_connected = false;
    is_calibration_started = false;
    is_paused = false;
    is_params_applyed = false;

    shmem = new QSharedMemory("Shared_memory");//for single instance check

    const QString s = "Locomotion device visualizer";
    setWindowTitle(s);

    createActions();
    createTrayIcon();

    timer = new QTimer;

    app_icon = new QIcon("app_icon.ico");
    restore_icon = new QIcon("restore.ico");
    exit_icon = new QIcon("exit.ico");

    trayIcon->setIcon(*app_icon);
    restoreAction->setIcon(*restore_icon);
    quitAction->setIcon(*exit_icon);

    trayIcon->show();

    //flags
    flags = Qt::Window;
    flags |= Qt::MSWindowsFixedSizeDialogHint;
    //flags |= Qt::WindowMinimizeButtonHint;
    flags |= Qt::WindowCloseButtonHint;
    this->menuBar()->setEnabled(false);
    this->statusBar()->hide();
    this->statusBar()->setSizeGripEnabled(false);
    this->setWindowFlags(flags);
    this->setWindowIcon(*app_icon);

    //widgets
    QWidget * wdg = new QWidget();
    ogl_wdg = new myOGLWidget();

    //tabs
    tabs = new QTabWidget(this);
    main_lay = new QGridLayout(wdg);


    QGridLayout *gb_lay = new QGridLayout();
    connect_button = new QPushButton("Connect");
    quit_button = new QPushButton("Quit");
    pause_button = new QPushButton("Pause");
    pause_button->setEnabled(false);

    gb_lay->addWidget(connect_button, 0, 0);
    gb_lay->addWidget(pause_button, 0, 1);
    gb_lay->addWidget(quit_button, 1, 0, 1, 0);

    QGroupBox *acc_plot_box = new QGroupBox(wdg);
    acc_plot_box->setTitle("Accelerometer");
    QVBoxLayout *acc_plot_lay = new QVBoxLayout();
    acc_pl = new SensorPlot(QwtText(""), wdg);
    acc_plot_lay->addWidget(acc_pl);
    acc_plot_box->setLayout(acc_plot_lay);

    QGroupBox *mag_plot_box = new QGroupBox(wdg);
    mag_plot_box->setTitle("Magnetometer");
    QVBoxLayout *mag_plot_lay = new QVBoxLayout();
    mag_pl = new SensorPlot(QwtText(""), wdg);
    mag_plot_lay->addWidget(mag_pl);
    mag_plot_box->setLayout(mag_plot_lay);

    QGroupBox *gyr_plot_box = new QGroupBox(wdg);
    gyr_plot_box->setTitle("Gyroscope");
    QVBoxLayout *gyr_plot_lay = new QVBoxLayout();
    gyr_pl = new SensorPlot(QwtText(""), wdg);
    gyr_plot_lay->addWidget(gyr_pl);
    gyr_plot_box->setLayout(gyr_plot_lay);


    //main_lay->addLayout(sensors_raw, 0, 0);
    main_lay->addWidget(acc_plot_box, 0, 0, 1, 0);
    main_lay->addWidget(mag_plot_box, 1, 0, 1, 0);
    main_lay->addWidget(gyr_plot_box, 2, 0, 1, 0);
    main_lay->addLayout(gb_lay, 3, 0);

    wdg->setLayout(main_lay);
    QWidget *calib_wdg = new QWidget();
    calib_start_button = new QPushButton("Start");
    calib_apply_button = new QPushButton("Apply");
    calib_upload_button = new QPushButton("Send to device");

    calib_start_button->setEnabled(false);
    calib_apply_button->setEnabled(false);
    calib_upload_button->setEnabled(false);

    QGridLayout *calib_lay = new QGridLayout();
    calib_wdg->setLayout(calib_lay);

    mag_plot = new Calibwidget();

    calib_lay->addWidget(mag_plot, 1, 0, 1, 0);
    calib_lay->addWidget(calib_start_button, 2, 0);
    calib_lay->addWidget(calib_apply_button, 2, 1);
    calib_lay->addWidget(calib_upload_button, 3, 0, 1, 0);

    tabs->addTab(wdg, "Raw data");
    tabs->addTab(calib_wdg, "Calibration");
    tabs->addTab(ogl_wdg, "Visualization");

    setCentralWidget(tabs);

    //singnals and slots
    connect(connect_button, SIGNAL(clicked()), SLOT(connectSlot()));
    connect(pause_button, SIGNAL(clicked()), SLOT(pauseSlot()));
    connect(quit_button, SIGNAL(clicked()), qApp, SLOT(quit()));
    connect(timer, SIGNAL(timeout()), SLOT(readDeviceSlot()));
    connect(calib_start_button, SIGNAL(clicked()), SLOT(calibrationSlot()));
    connect(calib_apply_button, SIGNAL(clicked()), SLOT(applySlot()));
    connect(calib_upload_button, SIGNAL(clicked()), SLOT(sendParamsSlot()));
}

//check if application is already running
bool MainWindow::checkSingle() {
    QSystemSemaphore sem("Global_semaphore", 1); //global semaphore with counter of size 1
    bool isRunning;
    sem.acquire(); //second instance cant't do anything til released

    //connect and release to destroy leftover links
    shmem->attach();
    shmem->detach();


    //try to connect to shared memory - if succes then instance already present
    if (shmem->attach())
    {
        isRunning = true;
        //        QMessageBox::critical(this, tr("MSG"), tr("Running"));
    }
    else
    {
        shmem->create(1);
        isRunning = false;
        //        QMessageBox::critical(this, tr("MSG"), tr("Not running"));
    }

    sem.release(); //release semaphore and enable second instance to proceed

    return isRunning;
}

int MainWindow::createMemFile(){
    //Common Named Memory
    hMapFile = CreateFileMapping(
                INVALID_HANDLE_VALUE,    // use paging file
                NULL,                    // default security
                PAGE_READWRITE,          // read/write access
                0,                       // maximum object size (high-order DWORD)
                BUF_SIZE,                // maximum object size (low-order DWORD)
                szName);                 // name of mapping object

    if (hMapFile == NULL)
    {
        qDebug("Could not create file mapping object");
        return 1;
    }
    pBuf = (LPTSTR) MapViewOfFile(hMapFile,   // handle to map object
                                  FILE_MAP_ALL_ACCESS, // read/write permission
                                  0,
                                  0,
                                  BUF_SIZE);

    if (pBuf == NULL)
    {
        qDebug("Could not map view of file");
        CloseHandle(hMapFile);
        return 1;
    }
}

void MainWindow::createActions(){
    restoreAction = new QAction(tr("&Restore"), this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void MainWindow::createTrayIcon(){
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
}

MainWindow::~MainWindow()
{
    if (is_connected) connectSlot();
    delete ui;
}

void MainWindow::readDeviceSlot()
{
    int report_size = sizeof(dataexchange_t);

    uint8_t buf[report_size];

    if (hid_read(handle_device, buf, report_size)==-1)
    {
        QMessageBox::critical(this, tr("Error"), tr("Error during reading attempt"));
        qApp->quit();

    } else {
        memcpy(&data, &buf, report_size);

        qDebug() << data.d1 << data.d2 << data.d3;

        //put data to memory file
        CopyMemory((PVOID)pBuf, &data, sizeof(dataexchange_t));

        acc_pl->addDataPoint(data.acc_x, data.acc_y, data.acc_z);
        mag_pl->addDataPoint(data.mag_x, data.mag_y, data.mag_z);
        gyr_pl->addDataPoint(data.gyr_x, data.gyr_y, data.gyr_z);

        if (ogl_wdg != nullptr) {
            ogl_wdg->rotateByQuaternion(data.qr_w, data.qr_x, data.qr_y, data.qr_z);
        }

        if (is_calibration_started) {
            mag_x_arr.push_back(data.mag_x);
            mag_y_arr.push_back(data.mag_y);
            mag_z_arr.push_back(data.mag_z);

            Qwt3D::Triple var((double)data.mag_x, (double)data.mag_y, (double)data.mag_z);
            mag_plot->addData(var);
            mag_plot->updatePlot();
        }

    }
}

void MainWindow::calibrationSlot()
{
    if (is_calibration_started) {
        timer->setInterval(10);
        is_calibration_started = false;
        calib_start_button->setText("Start");
        calib_apply_button->setEnabled(true);

    } else {
        timer->setInterval(33);
        is_calibration_started = true;
        mag_x_arr.clear();
        mag_y_arr.clear();
        mag_z_arr.clear();
        mag_plot->clearPlot();
        mag_plot->setInputScale(200);
        calib_start_button->setText("Stop");
        calib_apply_button->setEnabled(false);
        calib_upload_button->setEnabled(false);
    }
}

void MainWindow::connectSlot()
{
    if (!is_connected) {

        QString product_string("LUFA Generic HID Demo");

        struct hid_device_info *devs, *cur_dev;
        devs = hid_enumerate(/*VID*/0x03eb, /*PID*/0x204f);
        cur_dev = devs;
        while (cur_dev)
        {
            qDebug() << "Found: " << QString::fromWCharArray(cur_dev->product_string);
            if (QString::fromWCharArray(cur_dev->product_string) == product_string) {
                qDebug() << "1";
                break;
            }
            cur_dev = cur_dev->next;
        }

        if (cur_dev && (handle_device = hid_open_path(cur_dev->path)))
        {
            is_connected = true;
            is_paused = false;
            createMemFile();
            timer->start(10);
            connect_button->setText("Disconnect");
            pause_button->setEnabled(true);
            calib_start_button->setEnabled(true);
        }
        else
            QMessageBox::critical(this, tr("Error"), tr("Unable to connect to device"));

        hid_free_enumeration(devs);
    } else {
        connect_button->setText("Connect");
        calib_start_button->setEnabled(false);
        pause_button->setEnabled(false);
        pause_button->setText("Pause");

        //stop updates if any
        is_connected = false;
        timer->stop();
        hid_close(handle_device);
        deleteMemFile();
    }
}

void MainWindow::pauseSlot() {
    if (is_paused) {
        is_paused = false;
        calib_start_button->setEnabled(true);
        pause_button->setText("Pause");
        timer->start(10);
    } else {
        is_paused = true;
        calib_start_button->setEnabled(false);
        pause_button->setText("Resume");
        timer->stop();
    }
}

void MainWindow::applySlot() {
    float max_x, max_y, max_z;
    float min_x, min_y, min_z;
    float offset_x, offset_y, offset_z;

    auto result = std::minmax_element(mag_x_arr.begin(), mag_x_arr.end());
    min_x = *result.first;
    max_x = *result.second;

    result = std::minmax_element(mag_y_arr.begin(), mag_y_arr.end());
    min_y = *result.first;
    max_y = *result.second;

    result = std::minmax_element(mag_z_arr.begin(), mag_z_arr.end());
    min_z = *result.first;
    max_z = *result.second;

    offset_x = (max_x + min_x) / 2.0f;
    offset_y = (max_y + min_y) / 2.0f;
    offset_z = (max_z + min_z) / 2.0f;

    qDebug() << offset_x;
    qDebug() << offset_y;
    qDebug() << offset_z;

    //find ellipsoid major and minor axis
    //compute magnitudes and apply offset
    std::vector<float> magnitude;
    for (uint i = 0; i < mag_x_arr.size(); i++) {
        mag_x_arr[i] -= offset_x;
        mag_y_arr[i] -= offset_y;
        mag_z_arr[i] -= offset_z;
        float res = sqrt((mag_x_arr[i] * mag_x_arr[i]) + (mag_y_arr[i] * mag_y_arr[i]) + (mag_z_arr[i] * mag_z_arr[i]));
        magnitude.push_back(res);
    }

    //magnitude coefficients
    auto res = std::minmax_element(magnitude.begin(), magnitude.end());
    int min_ind = std::distance(magnitude.begin(), res.first);
    int min_magnitude = *res.first;

    int max_ind = std::distance(magnitude.begin(), res.second);
    int max_magnitude = *res.second;

    //ellipsoid coordinate system
    arma::frowvec ellipse_x = {mag_x_arr[max_ind], mag_y_arr[max_ind], mag_z_arr[max_ind]};
    ellipse_x = arma::normalise(ellipse_x);

    arma::frowvec ellipse_y = {mag_x_arr[min_ind], mag_y_arr[min_ind], mag_z_arr[min_ind]};
    ellipse_y = arma::normalise(ellipse_y);

    arma::frowvec ellipse_z = arma::cross(ellipse_x, ellipse_y);
    ellipse_z = arma::normalise(ellipse_z);

    ellipse_y = arma::cross(ellipse_x, ellipse_z);

    //ellipsoid matrix
    arma::fmat ellipse_mat;
    ellipse_mat.insert_rows(0, ellipse_x);
    ellipse_mat.insert_rows(1, ellipse_y);
    ellipse_mat.insert_rows(2, ellipse_z);

    //transformation from ellipsoid system to reference
    arma::fmat transform = trans(ellipse_mat);

    qDebug() << transform.at(0, 0);
    qDebug() << transform.at(0, 1);
    qDebug() << transform.at(0, 2);

    qDebug() << transform.at(1, 0);
    qDebug() << transform.at(1, 1);
    qDebug() << transform.at(1, 2);

    qDebug() << transform.at(2, 0);
    qDebug() << transform.at(2, 1);
    qDebug() << transform.at(2, 2);

    //transform
    for (uint i = 0; i < mag_x_arr.size(); i++) {
        arma::frowvec mag_translated = {mag_x_arr[i], mag_y_arr[i], mag_z_arr[i]};
        arma::frowvec mag_rotated = mag_translated * transform;

        //put rotated vectors back to compute scale later
        mag_x_arr[i] = mag_rotated[0];
        mag_y_arr[i] = mag_rotated[1];
        mag_z_arr[i] = mag_rotated[2];
    }

    //find max and min for rotated and translated set of vectors
    result = std::minmax_element(mag_x_arr.begin(), mag_x_arr.end());
    min_x = *result.first;
    max_x = *result.second;

    result = std::minmax_element(mag_y_arr.begin(), mag_y_arr.end());
    min_y = *result.first;
    max_y = *result.second;

    result = std::minmax_element(mag_z_arr.begin(), mag_z_arr.end());
    min_z = *result.first;
    max_z = *result.second;

    float len_x, len_y, len_z;
    len_x = max_x + abs(min_x);
    len_y = max_y + abs(min_y);
    len_z = max_z + abs(min_z);

    float avg_len = (len_x + len_y + len_z) / 3.0f;

    arma::frowvec scale_vec = {avg_len / len_x, avg_len / len_y, avg_len / len_z};

    qDebug() << scale_vec[0];
    qDebug() << scale_vec[1];
    qDebug() << scale_vec[2];

    //apply to plot
    mag_plot->clearPlot();
    for (uint i = 0; i < mag_x_arr.size(); i++) {
        arma::frowvec mag_scaled = {mag_x_arr[i] * scale_vec[0], mag_y_arr[i] * scale_vec[1], mag_z_arr[i] * scale_vec[2]};
        arma::frowvec mag_calibrated = mag_scaled * ellipse_mat;  //rotate back

        Qwt3D::Triple var(mag_calibrated[0], mag_calibrated[1], mag_calibrated[2]);
        mag_plot->addData(var);
    }
    mag_plot->updatePlot();

    //load to data structure
    calib_params.reportID = 0;
    calib_params.indicator = 7;

    //offset
    calib_params.offset_x = offset_x;
    calib_params.offset_y = offset_y;
    calib_params.offset_z = offset_z;

    //rotation matrix
    calib_params.rmat_r1_x = transform.at(0, 0);
    calib_params.rmat_r1_y = transform.at(0, 1);
    calib_params.rmat_r1_z = transform.at(0, 2);

    calib_params.rmat_r2_x = transform.at(1, 0);
    calib_params.rmat_r2_y = transform.at(1, 1);
    calib_params.rmat_r2_z = transform.at(1, 2);

    calib_params.rmat_r3_x = transform.at(2, 0);
    calib_params.rmat_r3_y = transform.at(2, 1);
    calib_params.rmat_r3_z = transform.at(2, 2);

    //scale
    calib_params.scale_x = scale_vec[0];
    calib_params.scale_y = scale_vec[1];
    calib_params.scale_z = scale_vec[2];

    is_params_applyed = true;
    calib_apply_button->setEnabled(false);
    calib_upload_button->setEnabled(true);
}

void MainWindow::deleteMemFile(){
    if (pBuf) {
        UnmapViewOfFile(pBuf);
        CloseHandle(hMapFile);
    }

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (trayIcon->isVisible()) {
        trayIcon->showMessage("Information", tr("The program will keep running in the "
                                                "system tray. To terminate the program, "
                                                "choose Quit in the context menu."));
        hide();
        event->ignore();
    }
}

void MainWindow::sendParamsSlot() {
    //    send data to device
    int report_size = sizeof(params_t);
    uint8_t buf[report_size] = {0};

    memcpy(&buf, &calib_params, report_size);

    if (hid_write(handle_device, buf, report_size) == -1)
    {
        QMessageBox::critical(this, tr("Error"), tr("Error during write attempt"));
    } else {
        QMessageBox::information(this, tr("Ready"), tr("Write attempt succesfull"));
        calib_upload_button->setEnabled(false);
    }

}
