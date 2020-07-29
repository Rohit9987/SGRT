#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
{
	initUI();
	createAction();
}

MainWindow::~MainWindow()
{

}

void MainWindow::initUI()
{
	showMaximized();
	
	QWidget *widget = new QWidget(this);

	QGridLayout *main_layout = new QGridLayout(widget);
	
	imageScene = new QGraphicsScene(widget);
	imageView = new QGraphicsView(imageScene);
	main_layout->addWidget(imageView, 0, 0, 12, 3);


	QGridLayout *options_layout = new QGridLayout();
	main_layout->addLayout(options_layout, 0 , 3, 12, 1);
	QLabel *options = new QLabel(this);
	options->setText("_________OPTIONS__________");
	options_layout->addWidget(options, 0, 0, Qt::AlignTop | Qt::AlignCenter);

	QGridLayout *output_layout = new QGridLayout();
	main_layout->addLayout(output_layout, 13, 0, 4, 1);

	QLabel *output = new QLabel(this);
	output->setText("OUTPUT:");
	output_layout->addWidget(output, 0, 0, Qt::AlignTop | Qt::AlignLeft);

	setCentralWidget(widget);
}

void MainWindow::createAction()
{
	fileMenu = menuBar()->addMenu("&File");




}
