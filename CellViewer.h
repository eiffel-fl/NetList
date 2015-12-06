// -*- explicit-buffer-name: "CellWidget.h<M1-MOBJ/8-10>" -*-

#ifndef NETLIST_CELL_VIEWER_H
#define NETLIST_CELL_VIEWER_H

#include <QWidget>
#include <QMainWindow>


namespace Netlist {

	class Cell;
	class CellWidget;
	class CellsLib;
	class InstancesWidget;
	class SaveCellDialog;

	class CellViewer : public QMainWindow {
	Q_OBJECT;
	public:
		CellViewer          ( QWidget* parent=NULL );
		virtual          ~CellViewer          ();
		Cell*     getCell             () const;
		inline  CellsLib* getCellsLib         ();  // TME9+.
	public slots:
		void      setCell             ( Cell* );
		void      saveCell            ();
		void      openCell            ();
		void      showCellsLib        ();  // TME9+.
		void      showInstancesWidget ();  // TME9+.
	private:
		CellWidget*      cellWidget_;
		CellsLib*        cellsLib_;         // TME9+.
		InstancesWidget* instancesWidget_;  // TME9+.
		SaveCellDialog*  saveCellDialog_;
	};
}  // Netlist namespace.

#endif  // NETLIST_CELL_VIEWER_H