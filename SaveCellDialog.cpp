// -*- explicit-buffer-name: "CellWidget.cpp<M1-MOBJ/8-10>" -*-
#include <iostream>
#include <QLayout>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QApplication>

#include "SaveCellDialog.h"

namespace Netlist {

	using namespace std;

	SaveCellDialog::SaveCellDialog(QWidget* parent) :
		QDialog(parent),
		lineEdit_(NULL){
			setWindowTitle("Save Cell");

			QLabel* label = new QLabel();
			label->setText("Enter Cell Name");
			label->setVisible(true);

			lineEdit_ = new QLineEdit();
			lineEdit_->setMinimumWidth(400);

			QPushButton* okButton = new QPushButton();
			okButton->setText("Ok");
			okButton->setDefault(true);

			QPushButton* cancelButton = new QPushButton();
			cancelButton->setText("Cancel");

			QHBoxLayout* hLayout = new QHBoxLayout();
			hLayout->addStretch();
			hLayout->addWidget(okButton);
			hLayout->addStretch();
			hLayout->addWidget(cancelButton);
			hLayout->addStretch();

			QVBoxLayout* vLayout = new QVBoxLayout();
			vLayout->addWidget(label);
			vLayout->addWidget(lineEdit_);
			vLayout->addItem(hLayout);
			vLayout->setSizeConstraint(QLayout::SetFixedSize);

			setLayout(vLayout);

			connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
			connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
		}

		bool SaveCellDialog::run(QString& name){
			setCellName(name);

			setVisible(true);

			int result = exec();

			name = getCellName();

			return (result == Accepted);
		}
}  // Netlist namespace.
