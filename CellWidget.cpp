// -*- explicit-buffer-name: "CellWidget.cpp<M1-MOBJ/8-10>" -*-

#include  <QResizeEvent>
#include  <QPainter>
#include  <QPen>
#include  <QBrush>
#include  <QFont>
#include  <QApplication>
#include  "CellWidget.h"
#include  "Term.h"
#include  "Instance.h"
#include  "Symbol.h"
#include  "Shape.h"
#include  "Cell.h"
#include  "Line.h"
#include  "Node.h"
#include  "Net.h"

#define TERM_SIZE 5
#define TERM_NAME_WIDTH 100
#define TERM_NAME_HEIGHT 20


namespace Netlist {

  using namespace std;


  ostream& operator<< ( ostream& o, const QRect& rect )
  {
    o << "<QRect x:" << rect.x()
      <<       " y:" << rect.y()
      <<       " w:" << rect.width()
      <<       " h:" << rect.height() << ">";
    return o;
  }


  ostream& operator<< ( ostream& o, const QPoint& p )
  { o << "<QRect x:" << p.x() << " y:" << p.y() << ">"; return o; }



  CellWidget::CellWidget ( QWidget* parent )
    : QWidget(parent)
    , cell_  (NULL)
		, viewport_ (Box(0, 0, 500, 500))
  {
    setAttribute    ( Qt::WA_OpaquePaintEvent );
    setAttribute    ( Qt::WA_NoSystemBackground );
    setAttribute    ( Qt::WA_StaticContents );
    setSizePolicy   ( QSizePolicy::Expanding, QSizePolicy::Expanding );
    setFocusPolicy  ( Qt::StrongFocus );
    setMouseTracking( true );
  }


  CellWidget::~CellWidget ()
  { }


  void  CellWidget::setCell ( Cell* cell )
  {
    cell_ = cell;
    repaint();
  }


  QSize  CellWidget::minimumSizeHint () const
  { return QSize(500,500); }


  void CellWidget::resizeEvent(QResizeEvent* event){
		cerr << "*********resizeEvent" << endl;
		const QSize& size = event->size();
		viewport_.setX2(viewport_.getX1() + size.width());
		viewport_.setY1(viewport_.getY2() - size.height());
		repaint();
	}

	void CellWidget::goRight(){
		viewport_.translate(Point(20, 0));
		repaint();
	}

	void CellWidget::goLeft(){
		viewport_.translate(Point(-20, 0));
		repaint();
	}

	void CellWidget::goUp(){
		viewport_.translate(Point(0, 20));
		repaint();
	}

	void CellWidget::goDown(){
		viewport_.translate(Point(0, -20));
		repaint();
	}

	void CellWidget::query(QPainter& painter){
		cerr << "Query James" << endl;
		if(not cell_){
			cerr << "Cell NULL" << endl;
			return;
		}

		const vector<Instance*> instances = cell_->getInstances();

		for(size_t i = 0; i < instances.size(); i++){
			cerr << "Peinturage" << endl;
			Point intPos = instances[i]->getPosition();

			const Symbol* symbol = instances[i]->getMasterCell()->getSymbol();

			if(not symbol)
				return;

			const vector<Shape*> shapes = symbol->getShapes();

			for(size_t j = 0; j < shapes.size(); j++){
				BoxShape* boxShape = dynamic_cast<BoxShape*>(shapes[j]);

				if(boxShape){
					Box box = boxShape->getBox();

					QRect rect = boxToScreenRect(box.translate(intPos));

					painter.drawRect(rect);
				} else {
					LineShape* lineShape = dynamic_cast<LineShape*>(shapes[j]);

					if(lineShape){
						painter.drawLine(pointToScreenPoint(Point(lineShape->getX1(), lineShape->getY1())), pointToScreenPoint(Point(lineShape->getX2(), lineShape->getY2())));
					} else {
						ArcShape* arcShape = dynamic_cast<ArcShape*>(shapes[j]);

						if(arcShape){
							Box box = arcShape->getBox();

							QRect rect = boxToScreenRect(box.translate(intPos));

							painter.drawArc(rect, arcShape->getStart(), arcShape->getSpan());
						} else {
							EllipseShape* ellipseShape = dynamic_cast<EllipseShape*>(shapes[j]);

							if(ellipseShape){
								Box box = ellipseShape->getBox();

								QRect rect = boxToScreenRect(box.translate(intPos));

								painter.drawEllipse(rect);
							} else {
								TermShape* termShape= dynamic_cast<TermShape*>(shapes[j]);

								if(termShape){
									if(termShape->getTerm()->isInternal()){ //un simple rectangle
										QRect rect = QRect(termShape->getX1(), termShape->getY1(), TERM_SIZE, TERM_SIZE);

										painter.drawRect(rect);
									}

									if(termShape->getTerm()->isExternal()){ //un polygone
										QVector<QPoint> points(5);

										points[0] = pointToScreenPoint(Point(termShape->getX1(), termShape->getY1()));

										points[1] = pointToScreenPoint(Point(termShape->getX1() + TERM_SIZE, termShape->getY1()));

										switch(termShape->getTerm()->getDirection()){
											case Term::In : //une maison penchée vers la droite
												points[2] = pointToScreenPoint(Point(termShape->getX1() + 1.5 * TERM_SIZE, termShape->getY1() - 0.5 * TERM_SIZE));

												points[3] = pointToScreenPoint(Point(termShape->getX1() + TERM_SIZE, termShape->getY1() - TERM_SIZE));

												points[4] = pointToScreenPoint(Point(termShape->getX1(), termShape->getY1() - TERM_SIZE));

												break;
											case Term::Out : //une maison penchée vers la gauche
												points[2] = pointToScreenPoint(Point(termShape->getX1() + TERM_SIZE, termShape->getY1() - TERM_SIZE));

												points[3] = pointToScreenPoint(Point(termShape->getX1(), termShape->getY1() - TERM_SIZE));

												points[4] = pointToScreenPoint(Point(termShape->getX1() - 0.5 * TERM_SIZE, termShape->getY1() - 0.5 * TERM_SIZE));

												break;
											default : //un rectangle simple
												points[2] = pointToScreenPoint(Point(termShape->getX1() + TERM_SIZE, termShape->getY1() - TERM_SIZE));

												points[3] = pointToScreenPoint(Point(termShape->getX1(), termShape->getY1() - TERM_SIZE));

												points[4] = points[0];
										}
										QPolygon polygon = QPolygon(points);

										painter.drawPolygon(polygon);

										QRect nameRect;

										switch(termShape->getAlign()){
											case TermShape::TopLeft :
												nameRect = QRect(pointToScreenPoint(Point(termShape->getX1() - TERM_NAME_WIDTH, termShape->getY1() + TERM_NAME_HEIGHT)), QSize(TERM_NAME_WIDTH, TERM_NAME_HEIGHT));

												painter.drawText(nameRect, Qt::AlignRight, termShape->getTerm()->getName().c_str());

												break;
											case TermShape::TopRight :
												nameRect = QRect(pointToScreenPoint(Point(termShape->getX1() + TERM_SIZE, termShape->getY1() + TERM_NAME_HEIGHT)), QSize(TERM_NAME_WIDTH, TERM_NAME_HEIGHT));

												painter.drawText(nameRect, Qt::AlignLeft, termShape->getTerm()->getName().c_str());

												break;
											case TermShape::BottomLeft :
												nameRect = QRect(pointToScreenPoint(Point(termShape->getX1() - TERM_NAME_WIDTH, termShape->getY1() - 2 * TERM_NAME_HEIGHT)), QSize(TERM_NAME_WIDTH, TERM_NAME_HEIGHT));

												painter.drawText(nameRect, Qt::AlignRight, termShape->getTerm()->getName().c_str());

												break;
											case TermShape::BottomRight :
												nameRect = QRect(pointToScreenPoint(Point(termShape->getX1() + TERM_SIZE, termShape->getY1() - 2 * TERM_NAME_HEIGHT)), QSize(TERM_NAME_WIDTH, TERM_NAME_HEIGHT));

												painter.drawText(nameRect, Qt::AlignLeft, termShape->getTerm()->getName().c_str());

												break;
										}

										painter.drawRect(nameRect);
									}
								}
							}
						}
					}
				}
			}
		}
	}

  void CellWidget::paintEvent(QPaintEvent* event){
    QPainter painter(this);
    painter.setBackground(QBrush(Qt::black));

    painter.eraseRect(QRect(QPoint(0,0), size()));

		painter.setPen(QPen(Qt::darkGreen, 10));

		QRect rect = boxToScreenRect(viewport_);

		cerr << "Peinturage du viewport_" << endl;
		cerr << rect << endl;
    painter.drawRect(rect);
  }

  void CellWidget::keyPressEvent(QKeyEvent* event){
		event->ignore();

		if(event->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier))
			return;
		switch(event->key()){
			case Qt::Key_Up :
				goUp();
				break;
			case Qt::Key_Down :
				goDown();
				break;
			case Qt::Key_Left :
				goLeft();
				break;
			case Qt::Key_Right :
				goRight();
				break;
		}
		event->accept();
	}


}  // Netlist namespace.
