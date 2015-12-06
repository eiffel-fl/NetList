// -*- explicit-buffer-name: "CellWidget.h<M1-MOBJ/8-10>" -*-

#ifndef NETLIST_CELLS_LIB_H
#define NETLIST_CELLS_LIB_H

#include <QWidget>
#include <QPushButton>
#include <QTableView>


namespace Netlist {

	class CellViewer;
	class CellsModel;

	class CellsLib : public QWidget {
		Q_OBJECT;
	public:
		CellsLib       ( QWidget* parent=NULL );
		void        setCellViewer  ( CellViewer* );
		int         getSelectedRow () const;
		inline CellsModel* getBaseModel   ();
	public slots:
		void        load           ();
	private:
		CellViewer*  cellViewer_;
		CellsModel*  baseModel_;
		QTableView*  view_;
		QPushButton* load_;
	};
}  // Netlist namespace.

#endif  // NETLIST_CELLS_LIB_H