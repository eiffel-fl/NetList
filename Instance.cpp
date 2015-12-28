#include  <cstdlib>
#include  "Cell.h"
#include  "Term.h"
#include  "Net.h"
#include  "Instance.h"

namespace Netlist {
	using namespace std;

	Instance::Instance(Cell* owner, Cell* model, const string& name) :
		owner_(owner),
		masterCell_(model),
		name_(name){
			owner->add(this); //on ajoute l'instance à la liste des instances de la cell

			Term* c;

			//model est NULL
			if(model != NULL){
				vector<Term*> modelTerms = model->getTerms();

				//on copie les terms du modèle
				for(vector<Term*>::iterator iMT = modelTerms.begin(); iMT != modelTerms.end(); ++iMT){
					c = new Term(this, (*iMT)->getName());

					terms_.push_back(c);
				}
			}
	}

	Instance::~Instance(){
		//on détruit les terminaux
		while(not terms_.empty())
			delete *terms_.begin();
	}

	//getters
	const string& Instance::getName() const{
		return name_;
	}

	Cell* Instance::getMasterCell() const{
		return masterCell_;
	}

	Cell* Instance::getCell() const{
		return owner_;
	}

	const vector<Term*>& Instance::getTerms() const{
		return terms_;
	}

	Term* Instance::getTerm(const string& str) const{
		vector<Term*>::const_iterator iT = terms_.begin();
		for(; iT != terms_.end(); ++iT){
			if((*(*iT)).getName() == str) //l'iterator est un pointeur qui pointe sur un Term* donc il faut déréférencer 2 fois pour obtenir un Term et appliquer getName()
				return *iT;
		}
		return NULL;
	}

	Point Instance::getPosition() const{
		return position_;
	}

	void Instance::toXml(std::ostream& stream){
		stream << indent << "<instance name=\"" << name_ << "\" mastercell=\"" << masterCell_->getName() << "\" x=\"" << position_.getX() << "\" y=\"" << position_.getY() << "\"/>\n";
	}


	//setters
	bool Instance::connect(const string& name, Net* net){ //on reprend le code de Cell::connect
		Term* term = getTerm( name );

		if (term == NULL)
			return false;

		term->setNet( net );
		return true;
	}

	void Instance::add(Term* term){ //pareil qu'au dessus
		if (getTerm(term->getName())) {
			cerr << "[ERROR] Attemp to add duplicated terminal <" << term->getName() << ">." << endl;
			return;
		}
		terms_.push_back(term);
	}

	void Instance::remove(Term* term){
		vector<Term*>::iterator iT = terms_.begin();
		for(; iT != terms_.end(); ++iT){
			if(*iT == term) //si term et l'iterator pointe sur le même Term
				terms_.erase(iT);
		}
	}

	void Instance::setPosition(const Point& point){
		position_ = point;
		vector<Term*>::iterator iT = terms_.begin();

		for(; iT != terms_.end(); ++iT){ //translation
			(*iT)->setPosition(getMasterCell()->getSymbol()->getTermPosition(*iT).translate(point));
		}
	}

	void Instance::setPosition(int x, int y){
		position_ = Point(x, y);
		vector<Term*>::iterator iT = terms_.begin();

		for(; iT != terms_.end(); ++iT){ //translation
			(*iT)->setPosition(getMasterCell()->getSymbol()->getTermPosition(*iT).translate(x, y));
		}
	}

	Instance* Instance::fromXml(Cell* cell, xmlTextReaderPtr xmlPtr){
		xmlChar* nameXml;
		xmlChar* masterCellXml;

		string name;
		string masterCell;

		int x;
		int y;

		int& rx = x;
		int& ry = y;

		const xmlChar* nameTag = xmlTextReaderConstString(xmlPtr, (const xmlChar*) "name");
		const xmlChar* masterCellTag = xmlTextReaderConstString(xmlPtr, (const xmlChar*) "mastercell");

		if((masterCellXml = xmlTextReaderGetAttribute(xmlPtr, masterCellTag)) == NULL)
			return NULL;

		if((nameXml = xmlTextReaderGetAttribute(xmlPtr, nameTag)) == NULL)
			return NULL;

		if(not xmlGetIntAttribute(xmlPtr, "x", rx))
			return NULL;

		if(not xmlGetIntAttribute(xmlPtr, "y", ry))
			return NULL;

		name = xmlCharToString(nameXml);
		masterCell = xmlCharToString(masterCellXml);

		Instance* i = new Instance(cell, cell->find(masterCell), name);
		i->setPosition(x, y);

		return i;
	}
}