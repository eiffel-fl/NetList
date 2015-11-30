// -*- explicit-buffer-name: "Main.cpp<M1-MOBJ/7>" -*-

#include <string>
#include <sstream>
#include <iostream>
#include <memory>
using namespace std;

#include <QtGui>
#include <QApplication>

#include "Net.h"
#include "Instance.h"
#include "Cell.h"
#include "Term.h"
#include "CellViewer.h"
using namespace Netlist;


int main (int argc, char* argv[]){
	Cell* gnd = Cell::load("gnd"); //chargement de gnd car c'est une cellule basique

	QApplication* qa = new QApplication(argc, argv);

	CellViewer* viewer = new CellViewer();
	viewer->setCell(gnd);
	viewer->show();

	int rvalue = qa->exec();
	delete qa;

	return rvalue;
}
