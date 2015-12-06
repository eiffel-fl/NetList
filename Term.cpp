#include  <cstdlib>
#include  "Cell.h"
#include  "Term.h"
#include  "Net.h"
#include  "Instance.h"

namespace Netlist {
	using namespace std;

	//constructor and destructor
	Term::Term(Cell* cell, const string& name, Direction direction) :
		owner_(cell),
		name_(name),
		direction_(direction),
		type_(External),
		net_(),
		node_(this){
			cell->add(this); //On ajoute le terminal courant à la liste des terminaux de la cell
	}

	Term::Term(Instance* instance, const string& name) :
		owner_(instance),
		name_(name),
		direction_(Unknown),
		type_(Internal),
		net_(),
		node_(this){
			instance->add(this); //même traitement qu'au dessus mais avec l'instance
	}

	Term::~Term(){
		setNet(NULL);
		node_.~Node();
	}

	//static functions
	string Term::toString(Type type){
		switch(type){
			case Internal :
				return "Internal";
			case External :
				return "External";
			default :
				cerr << "Type inconnu" << endl;
				exit(EXIT_FAILURE);
		}
	}

	string Term::toString(Direction direction){
		switch(direction){
			case In :
				return "In";
			case Out :
				return "Out";
			case InOut :
				return "InOut";
			case Tristate :
				return "Tristate";
			case Transcv :
				return "Transcv";
			default :
				return "Unknown";
		}
	}

	Term::Type Term::toType(string str){
		if(str == "Internal")
			return Internal;
		if(str == "External")
			return External;
		cerr << "Type string inconnu" << endl;
		exit(EXIT_FAILURE);
	}

	Term::Direction Term::toDirection(string str){
		if(str == "In")
			return In;
		if(str == "Out")
			return Out;
		if(str == "InOut")
			return InOut;
		if(str == "Tristate")
			return Tristate;
		if(str == "Transcv")
			return Transcv;
		return Unknown;
	}

	void Term::toXml(std::ostream& stream){
		stream << indent << "<term name=\"" << name_ << "\" direction=\"" << toString(direction_);
		stream << "\" x=\"" << getPosition().getX() << "\" y=\"" << getPosition().getY() << "\"/>\n";
	}

	//setters
	void Term::setNet(Net* net){
		net_ = net;
		net_->add(&node_); //on ajoute au net le node qui correspond au term
	}

	void Term::setNet(string& str){
		vector<Net*>::const_iterator iN = (static_cast<Cell*>(owner_))->getNets().begin();
		for(; iN != (static_cast<Cell*>(owner_))->getNets().end(); ++iN){
			if((*(*iN)).getName() == str){ //l'iterator est un pointeur qui pointe sur un Net* donc il faut déréférencer 2 fois pour obtenir un Net et appliquer getName()
				net_ = *iN;
				net_->add(&node_);
			}
		}
	}

	void Term::setPosition(const Point& point){
		node_.setPosition(point);
	}

	void Term::setPosition(int x, int y){
		node_.setPosition(x, y);
	}

	//static
	Term* Term::fromXml(Cell* cell, xmlTextReaderPtr xmlPtr){
		string name;
		Direction direction;

		int x;
		int y;

		int& rx = x;
		int& ry = y;

		xmlChar* nameXml;
		xmlChar* directionXml;

		const xmlChar* nameTag = xmlTextReaderConstString(xmlPtr, (const xmlChar*)"name");
		const xmlChar* directionTag = xmlTextReaderConstString(xmlPtr, (const xmlChar*)"direction");

		if((nameXml = xmlTextReaderGetAttribute(xmlPtr, nameTag)) == NULL)
			return NULL;

		if((directionXml = xmlTextReaderGetAttribute(xmlPtr, directionTag)) == NULL)
			return NULL;

		if(not xmlGetIntAttribute(xmlPtr, "x", rx))
			return NULL;

		if(not xmlGetIntAttribute(xmlPtr, "y", ry))
			return NULL;


		name = xmlCharToString(nameXml);
		direction = toDirection(xmlCharToString(directionXml));
		Term* t = new Term(cell, name, direction);

		t->setPosition(x, y);

		return t;
	}
}