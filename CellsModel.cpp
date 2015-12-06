// -*- explicit-buffer-name: "CellWidget.cpp<M1-MOBJ/8-10>" -*-
#include "CellsModel.h"
#include "Cell.h"

namespace Netlist {

	using namespace std;

	CellsModel::CellsModel(QObject* parent) :
	QAbstractTableModel(parent),
	cell_(NULL){
	}

	void CellsModel::setCells(Cell* cell){
		emit layoutAboutToBeChanged();
		cell_ = cell;
		cerr << "anchois" << endl;
		emit layoutChanged();
	}

	Cell* CellsModel::getModel(int row){
		if(row >= (int) Cell::getAllCells().size() or row < 0)
			return NULL;

		return Cell::getAllCells()[row];
	}

	int CellsModel::rowCount(const QModelIndex& parent) const{
		return Cell::getAllCells().size();
	}

	int CellsModel::columnCount(const QModelIndex& parent) const{
		return 1;
	}

	QVariant CellsModel::data(const QModelIndex& index, int role) const{
		if(not index.isValid())
			return QVariant();

		if(role == Qt::DisplayRole){
			int row = index.row();
			if(index.column() == 0)
				return Cell::getAllCells()[row]->getName().c_str();
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

}  // Netlist namespace.
