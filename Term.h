#ifndef NETLIST_TERM_H
#define NETLIST_TERM_H

#include <string>
#include <vector>
#include <libxml/xmlreader.h>
#include "XmlUtil.h"
#include "Point.h"
#include "Node.h"
#include "Instance.h"
#include "Indentation.h"

namespace Netlist {
	class Cell;
	class Net;

	class Term{
	public:
		//enum
		enum Type{Internal = 1, External = 2};
		enum Direction{In = 1, Out = 2, InOut = 3, Tristate = 4, Transcv = 5, Unknown = 6};

		//static functions
		static std::string toString(Type);
		static std::string toString(Direction);
		static Direction toDirection(std::string);
		static Type toType(std::string);
		static Term* fromXml(Cell*, xmlTextReaderPtr);

		//constructor and destructor
		Term(Cell* cell, const std::string& name, Direction direction);
		Term(Instance* instance, const std::string& name);
		~Term();

		//predicates
		inline bool isInternal() const{
			return (type_ == Internal) ? true : false;
		}

		inline bool isExternal() const{
			return (type_ == External) ? true : false;
		}

		//getters
		inline const std::string& getName() const{
			return name_;
		}

		inline Net* getNet() const{
			return net_;
		}

		inline Cell* getCell() const{
			if(type_ == External)
				return static_cast<Cell*>(owner_);
			else
				return NULL;
		}

		inline Cell* getOwnerCell() const{
			if(type_ == Internal)
				return getInstance()->getCell();
			return static_cast<Cell*>(owner_);
		}

		inline Instance* getInstance() const{
			if(type_ == Internal)
				return static_cast<Instance*>(owner_);
			else
				return NULL;
		}

		inline Direction getDirection() const{
			return direction_;
		}

		inline Point getPosition() const{
			return node_.getPosition();
		}

		inline Type getType() const{
			return type_;
		}

		inline Node* getNode(){
			return &node_;
		}

		void toXml(std::ostream&);

		//setters
		void setNet(Net*);
		void setNet(std::string&);

		inline void setDirection(Direction direction){
			direction_ = direction;
		}

		void setPosition(const Point&);
		void setPosition(int x, int y);

	private:
		void* owner_;
		std::string name_;
		Direction direction_;
		Type type_;
		Net* net_;
		NodeTerm node_;
	};
}  // Netlist namespace.

#endif  // NETLIST_TERM_H