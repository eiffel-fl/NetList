// -*- explicit-buffer-name: "Node.cpp<M1-MOBJ/7>" -*-

#include <limits>
#include "XmlUtil.h"
#include "Node.h"
#include "Line.h"
#include "Term.h"
#include "Net.h"
#include "Instance.h"
#include "Cell.h"

namespace Netlist {

  using namespace std;

	Shape::Shape(Symbol* owner) :
		owner_(owner){
			owner_->add(this);
		}

	Shape::~Shape(){
		owner_->remove(this);
	}

	Shape* Shape::fromXml (Symbol* owner, xmlTextReaderPtr reader){// Factory-like method.
		const xmlChar* boxTag = xmlTextReaderConstString( reader, (const xmlChar*)"box" );
		const xmlChar* ellipseTag = xmlTextReaderConstString( reader, (const xmlChar*)"ellipse" );
		const xmlChar* arcTag = xmlTextReaderConstString( reader, (const xmlChar*)"arc" );
		const xmlChar* lineTag = xmlTextReaderConstString( reader, (const xmlChar*)"line" );
		const xmlChar* termTag = xmlTextReaderConstString( reader, (const xmlChar*)"term" );
		const xmlChar* nodeName = xmlTextReaderConstLocalName( reader );

		Shape* shape = NULL;

		if (boxTag == nodeName)
			shape = BoxShape::fromXml( owner, reader );
		if (ellipseTag == nodeName)
			shape = EllipseShape::fromXml( owner, reader );
		if (arcTag == nodeName)
			shape = ArcShape::fromXml( owner, reader );
		if (lineTag == nodeName)
			shape = LineShape::fromXml( owner, reader );
		if (termTag == nodeName)
			shape = TermShape::fromXml( owner, reader );

		if (shape == NULL)
			cerr << "[ERROR] Unknown or misplaced tag <" << nodeName << "> (line:" << xmlTextReaderGetParserLineNumber(reader) << ")." << endl;
		return shape;
	}

  LineShape::LineShape(Symbol* owner, int x1, int x2, int y1, int y2) :
		Shape(owner),
		x1_(x1),
		x2_(x2),
		y1_(y1),
		y2_(y2){
	}

	LineShape::~LineShape(){}

	void LineShape::toXml(ostream& stream) const{
		stream << indent << "<line x1=\"" << x1_ << "\" y1=\"" << y1_ << "\" x2=\"" << x2_ << "\" y2=\"" << y2_ << "\"/>\n";
	}

	Shape* LineShape::fromXml(Symbol* owner, xmlTextReaderPtr reader){
		int x1;
		int y1;
		int x2;
		int y2;

		if(not xmlGetIntAttribute(reader, "x1", x1))
			return NULL;

		if(not xmlGetIntAttribute(reader, "y1", y1))
			return NULL;

		if(not xmlGetIntAttribute(reader, "x2", x2))
			return NULL;

		if(not xmlGetIntAttribute(reader, "y2", y2))
			return NULL;

		cerr << "Line : " << x1 << " " << x2 << " " << y1 << " " << y2 << endl;

		return (Shape*) new LineShape(owner, x1, x2, y1, y2);
	}

	BoxShape::BoxShape(Symbol* owner, Box box) :
		Shape(owner),
		box_(box){
	}

	BoxShape::BoxShape(Symbol* owner, int x1, int y1, int x2, int y2) :
		Shape(owner),
		box_(x1, y1, x2, y2){
	}

	BoxShape::~BoxShape(){}

	void BoxShape::toXml(ostream& stream) const{
		stream << indent << "<box x1=\"" << box_.getX1() << "\" y1=\"" << box_.getY1() << "\" x2=\"" << box_.getX2() << "\" y2=\"" << box_.getY2() << "\"/>\n";
	}

	Shape* BoxShape::fromXml(Symbol* owner, xmlTextReaderPtr reader){
		int x1;
		int y1;
		int x2;
		int y2;

		if(not xmlGetIntAttribute(reader, "x1", x1))
			return NULL;

		if(not xmlGetIntAttribute(reader, "y1", y1))
			return NULL;

		if(not xmlGetIntAttribute(reader, "x2", x2))
			return NULL;

		if(not xmlGetIntAttribute(reader, "y2", y2))
			return NULL;

		cerr << "Box : " << x1 << " " << x2 << " " << y1 << " " << y2 << endl;

		return (Shape*) new BoxShape(owner, x1, y1, x2, y2);
	}

	TermShape::TermShape(Symbol* owner, Term* term, int x1, int y1, NameAlign align) :
		Shape(owner),
		term_(term),
		x1_(x1),
		y1_(y1),
		align_(align){
	}

	TermShape::~TermShape(){}

	void TermShape::toXml(ostream& stream) const{
		stream << indent << "<term name=\"" << term_->getName() << "\" x1=\"" << x1_ << "\" y1=\"" << y1_ << "\" align=\"" << toString(align_) << "\"/>\n";
	}

	Shape* TermShape::fromXml(Symbol* owner, xmlTextReaderPtr reader){
		int x1;
		int y1;

		xmlChar* nameXml;
		xmlChar* alignXml;

		string name;
		NameAlign align;

		const xmlChar* nameTag = xmlTextReaderConstString(reader, (const xmlChar*)"name");
		const xmlChar* alignTag = xmlTextReaderConstString(reader, (const xmlChar*)"align");

		if((nameXml = xmlTextReaderGetAttribute(reader, nameTag)) == NULL)
			return NULL;

		if((alignXml = xmlTextReaderGetAttribute(reader, alignTag)) == NULL)
			return NULL;

		if(not xmlGetIntAttribute(reader, "x1", x1))
			return NULL;

		if(not xmlGetIntAttribute(reader, "y1", y1))
			return NULL;

		name = xmlCharToString(nameXml);
		align = toNameAlign(xmlCharToString(alignXml));

		cerr << "Term : " << name << " " << align << " " << x1 << " " << y1 << endl;

		return (Shape*) new TermShape(owner, owner->getCell()->getTerm(name), x1, y1, align);
	}

	string TermShape::toString(NameAlign align){
		switch(align){
			case TopLeft :
				return "top_left";
			case TopRight :
				return "top_right";
			case BottomLeft :
				return "bottom_left";
			case BottomRight :
				return "bottom_right";
			default :
				cerr << "NameAlign inconnu" << endl;
				exit(EXIT_FAILURE);
		}
	}

	TermShape::NameAlign TermShape::toNameAlign(string str){
		if(str == "top_left")
			return TopLeft;
		if(str == "top_right")
			return TopRight;
		if(str == "bottom_left")
			return BottomLeft;
		if(str == "bottom_right")
			return BottomRight;
		cerr << "NameAlign string inconnu" << endl;
		exit(EXIT_FAILURE);
	}

	EllipseShape::EllipseShape(Symbol* owner, int x1, int y1, int x2, int y2) :
		BoxShape(owner, x1, y1, x2, y2){
	}

	void EllipseShape::toXml(ostream& stream) const{
		stream << indent << "<ellipse x1=\"" << box_.getX1() << "\" y1=\"" << box_.getY1() << "\" x2=\"" << box_.getX2() << "\" y2=\"" << box_.getY2() << "\"/>\n";
	}

	Shape* EllipseShape::fromXml(Symbol* owner, xmlTextReaderPtr reader){
		int x1;
		int y1;
		int x2;
		int y2;

		if(not xmlGetIntAttribute(reader, "x1", x1))
			return NULL;

		if(not xmlGetIntAttribute(reader, "y1", y1))
			return NULL;

		if(not xmlGetIntAttribute(reader, "x2", x2))
			return NULL;

		if(not xmlGetIntAttribute(reader, "y2", y2))
			return NULL;

		cerr << "Ellipse : " << x1 << " " << x2 << " " << y1 << " " << y2 << endl;

		return (Shape*) new EllipseShape(owner, x1, y1, x2, y2);
	}

	ArcShape::ArcShape(Symbol* owner, int x1, int y1, int x2, int y2, int start, int span) :
		BoxShape(owner, x1, y1, x2, y2),
		start_(start),
		span_(span){
		}

	void ArcShape::toXml(ostream& stream) const{
		stream << indent << "<arc x1=\"" << box_.getX1() << "\" y1=\"" << box_.getY1() << "\" x2=\"" << box_.getX2() << "\" y2=\"" << box_.getY2() << "\" start=\"" << start_ << "\" span=\"" << span_ << "\"/>\n";
	}

	Shape* ArcShape::fromXml(Symbol* symbol, xmlTextReaderPtr reader){
		int x1;
		int y1;
		int x2;
		int y2;
		int start;
		int span;

		if(not xmlGetIntAttribute(reader, "x1", x1))
			return NULL;

		if(not xmlGetIntAttribute(reader, "y1", y1))
			return NULL;

		if(not xmlGetIntAttribute(reader, "x2", x2))
			return NULL;

		if(not xmlGetIntAttribute(reader, "y2", y2))
			return NULL;

		if(not xmlGetIntAttribute(reader, "start", start))
			return NULL;

		if(not xmlGetIntAttribute(reader, "span", span))
			return NULL;

		cerr << "Arc : " << x1 << " " << x2 << " " << y1 << " " << y2 << " " << start << " " << span << endl;

		return (Shape*) new ArcShape(symbol, x1, y1, x2, y2, start, span);
	}
}  // Netlist namespace.
