#ifndef NETLIST_NET_H
#define NETLIST_NET_H

#include <string>
#include <vector>
#include <libxml/xmlreader.h>
#include "XmlUtil.h"
#include "Point.h"
#include "Indentation.h"
#include "Term.h"

namespace Netlist {
	class Cell;
	class Term;
	class Instance;

	class Net{
	public:
		//constructor and destructor
		Net(Cell*, const std::string&, Term::Type);
		~Net();

		//getters
		Cell* getCell() const;
		const std::string& getName() const;
		unsigned int getId() const;
		Term::Type getType() const;
		const std::vector<Node*>& getNodes() const;
		size_t getFreeNodeId() const;
		void toXml(std::ostream&);
		Node* getNode(size_t);
		inline const std::vector<Line*>& getLines() const {
			return lines_;
		}

		//setters
		void add(Line*);
		bool remove(Line*);
		void add(Node*);
		void remove(Node*);
		static Net* fromXml(Cell*, xmlTextReaderPtr);
	private:
		Cell* owner_;
		std::string name_;
		unsigned int id_;
		Term::Type type_;
		std::vector<Line*> lines_;
		std::vector<Node*> nodes_;
	};
}  // Netlist namespace.

#endif  // NETLIST_NET_H