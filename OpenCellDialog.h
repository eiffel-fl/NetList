#ifndef NETLIST_OPEN_DIALOG_H
#define NETLIST_OPEN_DIALOG_H

#include <QDialog>
#include <QLineEdit>
class QPainter;

namespace Netlist {

	class Cell;
	class NodeTerm;

	class OpenCellDialog : public QDialog {
		Q_OBJECT;
	public :
		OpenCellDialog(QWidget* parent=NULL);
		~OpenCellDialog();
		static std::string run();
		inline QString getCellName() const{ return lineEdit_->text();}
		inline void setCellName(QString& name){ lineEdit_->setText(name);}
	protected :
		QLineEdit* lineEdit_;
	};
}  // Netlist namespace.

#endif  // NETLIST_SAVE_WIDGET_H