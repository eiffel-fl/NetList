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
	pthread_t tid[4];

	string files[] = {"vdd", "gnd", "TransistorN", "TransistorP", "and2", "xor2", "or2", "halfadder", "fulladder"};

	int i;
	int j;
	int k;

	for(i = 0; i < 4; i++){ //niveau 0 : vdd, gnd, N et P
		if(pthread_create(&tid[i], NULL, Cell::threadLoad, (void*) &files[i]) == -1){
			perror("pthread_create ");
			exit(EXIT_FAILURE);
		}
	}

	for(i = 0; i < 4; i++){ //attente du niveau 0
		if(pthread_join(tid[i], NULL) == -1){
			perror("pthread_join ");
			exit(EXIT_FAILURE);
		}
	}

	for(k = 0; k < 2; k++){ //niveau 1 : XOR et AND
		for(j = 0; j < 2; j++){ //niveau 2 : OR et halfadder
			if(pthread_create(&tid[j], NULL, Cell::threadLoad, (void*) &files[i]) == -1){
				perror("pthread_create ");
				exit(EXIT_FAILURE);
			}

			i++;
		}

		for(j = 0; j < 2; j++){ //attente du niveau 1 puis du niveau 2
			if(pthread_join(tid[j], NULL) == -1){
				perror("pthread_join ");
				exit(EXIT_FAILURE);
			}
		}

	}

	if(pthread_create(&tid[0], NULL, Cell::threadLoad, (void*) &files[i]) == -1){ //niveau 3 : fulladder
		perror("pthread_create ");
		exit(EXIT_FAILURE);
	}

	Cell* fulladder;

	if(pthread_join(tid[0], (void**) &fulladder) == -1){
		perror("pthread_join ");
		exit(EXIT_FAILURE);
	}

	QApplication* qa = new QApplication(argc, argv);

	CellViewer* viewer = new CellViewer();
	viewer->setCell(fulladder);
	viewer->show();

	int rvalue = qa->exec();
	delete qa;

	pthread_mutex_destroy(&Cell::mutex);

	return rvalue;
}