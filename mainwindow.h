#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLabel>
#include <QCameraInfo>
#include <QMessageBox>
#include <QGridLayout>
#include <QPushButton>
#include <QMenuBar>
#include <QRadioButton>
#include <QPushButton>
#include <QSpacerItem>
#include <QComboBox>
#include <QCameraInfo>
#include <QList>
#include <QApplication>
#include <QThread>
#include <QMutex>
#include <QSlider>

#include "opencv2/opencv.hpp"
#include "camera.h"
#include "dibhControls.h"
#include "graphicsview.h"

class MainWindow: public QMainWindow
{
	Q_OBJECT
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	QMenu *fileMenu;
	QAction *cameraInfoAction;
	QAction *openCameraAction;
	QAction * exitAction;

	QGraphicsScene *imageScene;
	//QGraphicsView *imageView;
    GraphicsView *imageView;

	QStatusBar *mainStatusBar;
	QLabel *mainStatusLabel;

	//members of options layout
	QPushButton *cameraButton;
	QPushButton *detectButton;
	QComboBox *modeComboBox;

    dibhControls *dibhWindow;

	//Camera Object and QThread for multitasking
	Camera *camera;
	QThread *cameraThread;
    QMutex *data_lock;

	/**
	 *All layout designs
	 */
	void initUI();

	/**
	 *
	 */
	void createAction();


private slots:
	void showCameraInfo();
	void openCamera();
    void showdibhWindow();
    void mouseReleased();

public slots:
	void display_Video(cv::Mat *frame);
    void hsvChanged(int lowH, int lowS, int lowV,
            int highH, int highS, int highV);
    void receiveAreaPoints(QPointF p1, QPointF p2);
    void receiveContourSlot();
	void colorScheme(bool);
};
#endif	//MAINWINDOW
