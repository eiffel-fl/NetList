// -*- explicit-buffer-name: "CellWidget.cpp<M1-MOBJ/8-10>" -*-
#include <QWidget>
#include <QMenu>
#include <QMenuBar>
#include <QApplication>

#include "Cell.h"
#include "CellViewer.h"
#include "CellWidget.h"
#include "SaveCellDialog.h"
#include "OpenCellDialog.h"
#include "InstancesWidget.h"
#include "CellsLib.h"

namespace Netlist {

	using namespace std;

	CellViewer::CellViewer(QWidget* parent) :
	QMainWindow(parent),
	cellWidget_(NULL),
	cellsLib_(NULL),
	instancesWidget_(NULL),
	saveCellDialog_(NULL){
		cellWidget_ = new CellWidget();
		saveCellDialog_ = new SaveCellDialog();
		instancesWidget_ = new InstancesWidget();
		cellsLib_= new CellsLib();

		setCentralWidget(cellWidget_);

		QMenu* fileMenu = menuBar()->addMenu("&File");

		QAction* action = new QAction("&Save as", this);
		action->setStatusTip("Save to disk (rename the old file)");
		action->setShortcut(QKeySequence("Ctrl+S"));
		action->setVisible(true);

		fileMenu->addAction(action);
		connect(action, SIGNAL(triggered()), this, SLOT(saveCell()));

		action = new QAction("&Open", this);
		action->setStatusTip("Open the file");
		action->setShortcut(QKeySequence("Ctrl+O"));
		action->setVisible(true);

		fileMenu->addAction(action);
		connect(action, SIGNAL(triggered()), this, SLOT(openCell()));

		action = new QAction("&Instances", this);
		action->setStatusTip("Show the Instances");
		action->setShortcut(QKeySequence("Ctrl+I"));
		action->setVisible(true);

		fileMenu->addAction(action);
		connect(action, SIGNAL(triggered()), this, SLOT(showInstancesWidget()));

		action = new QAction("&Cells", this);
		action->setStatusTip("Show the Cells");
		action->setShortcut(QKeySequence("Ctrl+C"));
		action->setVisible(true);

		fileMenu->addAction(action);
		connect(action, SIGNAL(triggered()), this, SLOT(showCellsLib()));

		action = new QAction("&Quit", this);
		action->setStatusTip("Close the application");
		action->setShortcut(QKeySequence("Ctrl+Q"));
		action->setVisible(true);

		fileMenu->addAction(action);
		connect(action, SIGNAL(triggered()), this, SLOT(close()));
	}

	CellViewer::~CellViewer(){}

	Cell* CellViewer::getCell() const{
		return cellWidget_->getCell();
	}

	void CellViewer::saveCell(){
		pthread_t tid;

		Cell* cell = getCell();

		if(cell == NULL)
			return;

		QString cellName = cell->getName().c_str();

		cerr << "Save Cell Dialog name : " << cell->getName().c_str() << " *" << endl;

		if(saveCellDialog_->run(cellName)){
			cell->setName(cellName.toStdString());
			cerr << "Save Cell Dialog name : " << cell->getName().c_str() << " 2" << endl;

			if(pthread_create(&tid, NULL, Cell::threadSave, (void*) cell) == -1){
				perror("pthread_create ");
				exit(EXIT_FAILURE);
			}

			//on n'attend pas la thread, elle se finira quand elle se finira, on gagne du temps
		}
	}

	void CellViewer::setCell(Cell* cell){
		cellWidget_->setCell(cell);
	}

	void CellViewer::openCell(){
		Cell* cell;
		pthread_t tid;
		string cellName = OpenCellDialog::run();

		if((cell = cellWidget_->getCell()->find(cellName))){ //si on a déjà chargé la cell
			setCell(cell); //on change juste
			return;
		}

		//il faut charger la cell
		if(pthread_create(&tid, NULL, Cell::threadLoad, (void*) &cellName) == -1){
			perror("pthread_create ");
			exit(EXIT_FAILURE);
		}

		if(pthread_join(tid, (void**) &cell) == -1){
			perror("pthread_join ");
			exit(EXIT_FAILURE);
		}

		setCell(cell);
	}

	void CellViewer::showInstancesWidget(){
		instancesWidget_->setCellViewer(this);
		instancesWidget_->setCell(getCell());
		instancesWidget_->setVisible(true);
	}

	void CellViewer::showCellsLib(){
		cellsLib_->setCellViewer(this);
		cellsLib_->setVisible(true);
	}
}  // Netlist namespace.