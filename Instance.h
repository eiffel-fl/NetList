#ifndef NETLIST_INSTANCE_H
#define NETLIST_INSTANCE_H

#include <string>
#include <vector>
#include <libxml/xmlreader.h>
#include "Point.h"
#include "Indentation.h"
#include "XmlUtil.h"

namespace Netlist {
	class Cell;
	class Term;

	class Instance{
		public:
			//constructor and destructor
			Instance(Cell* owner, Cell* model ,const std::string&);
			~Instance();

			//getters
			const std::string& getName() const;
			Cell* getCell() const;
			Cell* getMasterCell() const;
			const std::vector<Term*>& getTerms() const;
			Term* getTerm(const std::string&) const;
			Point getPosition() const;
			void toXml(std::ostream&);

			//setters
			bool connect(const std::string& name, Net*);
			void add(Term*);
			void remove(Term*);
			void setPosition(const Point&);
			void setPosition(int x, int y);
			static Instance* fromXml(Cell*, xmlTextReaderPtr);
		private:
			Cell* owner_;
			Cell* masterCell_;
			std::string name_;
			std::vector<Term*> terms_;
			Point position_;
	};
}

#endif //NETLIST_INSTANCE_H