// -*- explicit-buffer-name: "CellWidget.cpp<M1-MOBJ/8-10>" -*-
#include <QLayout>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QApplication>

#include "OpenCellDialog.h"

namespace Netlist {

	using namespace std;

	OpenCellDialog::OpenCellDialog(QWidget* parent) :
	QDialog(parent),
	lineEdit_(NULL){
		setWindowTitle("Open Cell");

		QLabel* label = new QLabel();
		label->setText("Enter Cell Name");

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

	OpenCellDialog::~OpenCellDialog(){
		lineEdit_->~QLineEdit();
	}

	string OpenCellDialog::run(){
		OpenCellDialog* open = new OpenCellDialog();

		int result = open->exec();

		string name = "";

		if(result == Accepted)
			name = open->getCellName().toStdString();

		delete open;

		return name;
	}
}  // Netlist namespace.