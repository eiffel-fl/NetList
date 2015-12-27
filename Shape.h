// -*- explicit-buffer-name: "Node.h<M1-MOBJ/7>" -*-

#ifndef NETLIST_SHAPE_H
#define NETLIST_SHAPE_H

#include <iostream>
#include <vector>
#include <libxml/xmlreader.h>
#include "Symbol.h"
#include "Box.h"
#include "Term.h"
#include "Indentation.h"


namespace Netlist {
	class Term;
	class Box;
	class Symbol;


	class Shape {
	public:
		Shape(Symbol*);
		~Shape();
		static Shape* fromXml(Symbol*, xmlTextReaderPtr);

		inline Symbol* getSymbol() const { return owner_;}

		virtual void toXml(std::ostream&) const =0;
	protected :
		Symbol* owner_;
	};


	class LineShape : public Shape {
	public:
		LineShape(Symbol*, int, int, int, int);
		~LineShape();

		inline int getX1(){ return x1_; };
		inline int getX2(){ return x2_; };
		inline int getY1(){ return y1_; };
		inline int getY2(){ return y2_; };

		virtual void toXml(std::ostream&) const;
		static Shape* fromXml(Symbol*, xmlTextReaderPtr);
	private:
		int x1_;
		int x2_;
		int y1_;
		int y2_;
	};

	class BoxShape : public Shape {
	public:
		BoxShape(Symbol*, Box);
		BoxShape(Symbol*, int, int, int, int);
		~BoxShape();

		inline Box getBox(){ return box_; };

		virtual void toXml(std::ostream&) const;
		static Shape* fromXml(Symbol*, xmlTextReaderPtr);
	protected:
		Box box_;
	};

	class TermShape : public Shape {
	public:
		enum NameAlign {TopLeft = 1, TopRight = 2, BottomLeft = 3, BottomRight = 4};

		TermShape(Symbol*, Term*, int, int, NameAlign);
		~TermShape();

		inline Term* getTerm(){ return term_; };
		inline int getX1(){ return x1_; };
		inline int getY1(){ return y1_; };
		inline Point getPosition(){ return Point(x1_, y1_); }
		inline NameAlign getAlign(){ return align_; };

		virtual void toXml(std::ostream&) const;
		static Shape* fromXml(Symbol*, xmlTextReaderPtr);

		static NameAlign toNameAlign(std::string);
		static std::string toString(NameAlign);

		inline void setX1(int x){ x1_ = x; };
		inline void setY1(int y){ y1_ = y; };
	private:
		Term* term_;
		int x1_;
		int y1_;
		NameAlign align_;
	};

	class ArcShape : public BoxShape {//ArcShape possède une Box donc elle héritera de BoxShape
	public:
		ArcShape(Symbol*, int, int, int, int, int, int);
		~ArcShape();

		inline int getStart(){ return start_;}
		inline int getSpan(){ return span_;}

		inline void setStart(int start){ start_ = start;}
		inline void setSpan(int span){ span_ = span;}

		virtual void toXml(std::ostream&) const;
		static Shape* fromXml(Symbol*, xmlTextReaderPtr);
	private :
		int start_;
		int span_;
	};

	class EllipseShape : public BoxShape {//pareil que pour ArcShape
	public:
		EllipseShape(Symbol*, int, int, int, int);
		~EllipseShape();

		virtual void toXml(std::ostream&) const;
		static Shape* fromXml(Symbol*, xmlTextReaderPtr);
	};
}  // Netlist namespace.

#endif  // NETLIST_SHAPE_H