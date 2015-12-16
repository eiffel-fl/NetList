// -*- explicit-buffer-name: "Box.cpp<M1-MOBJ/7>" -*-
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QPushButton>
#include <QTableView>

#include "CellsLib.h"
#include "CellViewer.h"
#include "CellsModel.h"

namespace Netlist {

	using namespace std;

	CellsLib::CellsLib(QWidget* parent) :
	QWidget(parent),
	cellViewer_(NULL),
	baseModel_(new CellsModel()),
	view_(new QTableView()),
	load_(new QPushButton()){
		setAttribute(Qt::WA_QuitOnClose, false);
		setAttribute(Qt::WA_DeleteOnClose, false);

		view_->setShowGrid(false);
		view_->setAlternatingRowColors(true);
		view_->setSelectionBehavior(QAbstractItemView::SelectRows);
		view_->setModel(baseModel_);

		QHeaderView* hH = view_->horizontalHeader();
		hH->setDefaultAlignment(Qt::AlignCenter);
		hH->setMinimumSectionSize(300);
		hH->setStretchLastSection(true);

		view_->verticalHeader()->setVisible(false);

		load_->setText("Load");
		connect(load_, SIGNAL(clicked()), this, SLOT(load()));

		QHBoxLayout* hLayout = new QHBoxLayout();
		hLayout->addStretch();
		hLayout->addWidget(load_);
		hLayout->addStretch();

		QVBoxLayout* vLayout = new QVBoxLayout();
		vLayout->addWidget(view_);
		vLayout->addItem(hLayout);
		vLayout->setSizeConstraint(QLayout::SetFixedSize);

		setWindowTitle("Cells");

		setLayout(vLayout);
	}

	int CellsLib::getSelectedRow() const{
		QModelIndexList selected = view_->selectionModel()->selection().indexes();

		if(selected.empty())
			return -1;

		return selected.first().row();
	}

	void CellsLib::load(){
		int row = getSelectedRow();

		if(row < 0)
			return;

		cellViewer_->setCell(baseModel_->getModel(row));
	}

	void CellsLib::setCellViewer(CellViewer* cellViewer){
		cellViewer_ = cellViewer;
	}

}  // Netlist namespace.