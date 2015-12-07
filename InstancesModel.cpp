// -*- explicit-buffer-name: "CellWidget.cpp<M1-MOBJ/8-10>" -*-
#include "InstancesModel.h"
#include "Cell.h"

namespace Netlist {

	using namespace std;

	InstancesModel::InstancesModel(QObject* parent) :
	QAbstractTableModel(parent),
	cell_(NULL){
	}

	void InstancesModel::setCell(Cell* duracell){
		emit layoutAboutToBeChanged();
		cell_ = duracell;
		emit layoutChanged();
	}

	Cell* InstancesModel::getModel(int row){
		if(not cell_)
			return NULL;

		if(row >= (int) cell_->getInstances().size() or row < 0)
			return NULL;

		return cell_->getInstances()[row]->getMasterCell();
	}

	int InstancesModel::rowCount(const QModelIndex& parent) const{
		return (cell_) ? cell_->getInstances().size() : 0;
	}

	int InstancesModel::columnCount(const QModelIndex& parent) const{
		return 2;
	}

	QVariant InstancesModel::data(const QModelIndex& index, int role) const{
		if(not cell_)
			return QVariant();

		if(not index.isValid())
			return QVariant();

		if(role == Qt::DisplayRole){
			int row = index.row();
			switch(index.column()){
				case 0 :
					return cell_->getInstances()[row]->getName().c_str();
				case 1 :
					return cell_->getInstances()[row]->getMasterCell()->getName().c_str();
			}
		}

		return QVariant();
	}

	QVariant InstancesModel::headerData(int section, Qt::Orientation orientation, int role) const{
		if(orientation == Qt::Vertical)
			return QVariant();

		if(role != Qt::DisplayRole)
			return QVariant();

		switch(section){
			case 0 :
				return "instances";
			case 1 :
				return "masterCell";
		}

		return QVariant();
	}

}  // Netlist namespace.
