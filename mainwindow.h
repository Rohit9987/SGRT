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


#include "opencv2/opencv.hpp"
#include "camera.h"

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
	QGraphicsView *imageView;

	QStatusBar *mainStatusBar;
	QLabel *mainStatusLabel;
	
	//members of options layout
	QPushButton *cameraButton;
	QPushButton *detectButton;
	QComboBox *modeComboBox;

	//Camera Object and QThread for multitasking
	Camera *camera;
	QThread *cameraThread;

	/*
	 *All layout designs
	 */
	void initUI();
	
	/*
	 *
	 */

	void createAction();

	//TODO
	//radiobuttons to select mode
	//start button to open camera - QPushButton
	//

private slots:
	void showCameraInfo();
	void openCamera();

public slots:
	void display_Video(cv::Mat *frame);

};
#endif	//MAINWINDOW
