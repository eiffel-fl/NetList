// -*- explicit-buffer-name: "CellWidget.h<M1-MOBJ/8-10>" -*-

#ifndef NETLIST_INSTANCES_WIDGET_H
#define NETLIST_INSTANCES_WIDGET_H

#include <QWidget>
#include <QTableView>
#include <QPushButton>
#include "InstancesModel.h"

namespace Netlist {

	class Cell;
	class CellViewer;

	class InstancesWidget : public QWidget {
		Q_OBJECT;
	public:
		InstancesWidget ( QWidget* parent=NULL );
		void  setCellViewer   ( CellViewer* );
		void  goTo            ( int );
		int   getSelectedRow  () const;
		inline void setCell(Cell* cell){ baseModel_->setCell(cell); }
	public slots:
		void  load            ();
	private:
		CellViewer*     cellViewer_;
		InstancesModel* baseModel_;
		QTableView*     view_;
		QPushButton*    load_;
	};
}  // Netlist namespace.

#endif  // NETLIST_INSTANCES_WIDGET_H