// -*- explicit-buffer-name: "CellWidget.cpp<M1-MOBJ/8-10>" -*-
#include "CellsModel.h"
#include "Cell.h"

namespace Netlist {

	using namespace std;

	CellsModel::CellsModel(QObject* parent) :
	QAbstractTableModel(parent),
	cells_(Cell::getAllCells()){ /*on initialise le vector avec le vector des cells*/
	}

	void CellsModel::setCells(std::vector<Cell*> cells){
		emit layoutAboutToBeChanged();
		cells_ = cells;
		emit layoutChanged();
	}

	Cell* CellsModel::getModel(int row){
		if(not cells_.size())
			return NULL;

		if(row >= (int) Cell::getAllCells().size() or row < 0)
			return NULL;

		return cells_[row];
	}

	int CellsModel::rowCount(const QModelIndex& parent) const{
		return cells_.size();
	}

	int CellsModel::columnCount(const QModelIndex& parent) const{
		return 1;
	}

	QVariant CellsModel::data(const QModelIndex& index, int role) const{
		if(not cells_.size())
			return QVariant();

		if(not index.isValid())
			return QVariant();

		if(role == Qt::DisplayRole){
			int row = index.row();
			if(index.column() == 0)
				return cells_[row]->getName().c_str();
		}

		return QVariant();
	}

	QVariant CellsModel::headerData(int section, Qt::Orientation orientation, int role) const{
		if(orientation == Qt::Vertical)
			return QVariant();

		if(role != Qt::DisplayRole)
			return QVariant();

		if(not section)
			return "Cell";

		return QVariant();
	}

	void CellsModel::updateDatas(){ /*quand on reçoit le signal, on "rafraichit" */
		setCells(Cell::getAllCells());
	}
}  // Netlist namespace.
