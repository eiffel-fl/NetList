// -*- explicit-buffer-name: "Box.cpp<M1-MOBJ/7>" -*-
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QPushButton>
#include <QTableView>

#include "InstancesWidget.h"
#include "CellViewer.h"
#include "InstancesModel.h"

namespace Netlist {

	using namespace std;

	InstancesWidget::InstancesWidget(QWidget* parent) :
	QWidget(parent),
	cellViewer_(NULL),
	baseModel_(new InstancesModel()),
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

		setWindowTitle("Instances");

		setLayout(vLayout);
	}

	int InstancesWidget::getSelectedRow() const{
		QModelIndexList selected = view_->selectionModel()->selection().indexes();

		if(selected.empty())
			return -1;

		return selected.first().row();
	}

	void InstancesWidget::load(){
		int row = getSelectedRow();

		if(row < 0)
			return;

		cellViewer_->setCell(baseModel_->getModel(row));
	}

	void InstancesWidget::setCellViewer(CellViewer* cellViewer){
		cellViewer_ = cellViewer;
	}

}  // Netlist namespace.