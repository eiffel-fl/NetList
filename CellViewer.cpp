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
		setCell(Cell::load(OpenCellDialog::run()));
	}
}  // Netlist namespace.