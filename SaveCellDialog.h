#ifndef NETLIST_SAVE_DIALOG_H
#define NETLIST_SAVE_DIALOG_H

#include <QDialog>
#include <QLineEdit>
class QPainter;

namespace Netlist {

	class Cell;
	class NodeTerm;


	class SaveCellDialog : public QDialog {
		Q_OBJECT;
	public :
		SaveCellDialog(QWidget* parent=NULL);
		bool run(QString& name);
		inline QString getCellName() const{ return lineEdit_->text();}
		inline void setCellName(QString& name){ lineEdit_->setText(name);}
	protected :
		QLineEdit* lineEdit_;
	};
}  // Netlist namespace.

#endif  // NETLIST_SAVE_WIDGET_H