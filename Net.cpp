#include  <cstdlib>
#include  "Cell.h"
#include  "Term.h"
#include  "Net.h"
#include  "Line.h"
#include  "Instance.h"

namespace Netlist {
	using namespace std;

	//constructor and destructor
	Net::Net(Cell* cell, const string& str, Term::Type type) :
		owner_(cell),
		name_(str),
		id_(cell->newNetId()),
		type_(type),
		nodes_(){
			cell->add(this); //on ajoute à la liste des nets de la cell le net courant
	}

	Net::~Net(){
		//on efface les nodes (donc les terms)
		nodes_.clear();
	}


	//getters
	Cell* Net::getCell() const{
		return static_cast<Cell*>(owner_);
	}

	const string& Net::getName() const{
		return name_;
	}

	unsigned int Net::getId() const{
		return id_;
	}

	Term::Type Net::getType() const{
		return type_;
	}

	const vector<Node*>& Net::getNodes() const{
		return nodes_;
	}

	Node* Net::getNode(size_t id){
		for(vector<Node*>::const_iterator iN = nodes_.begin(); iN != nodes_.end(); ++iN)
			if(*iN != NULL and (*iN)->getId() == id)
				return *iN;
			return NULL;
	}

	size_t Net::getFreeNodeId() const{
		int firstFree = 0;

		for(vector<Node*>::const_iterator iN = nodes_.begin(); iN != nodes_.end(); ++iN){
			firstFree++;
			if(*iN == NULL) //s'il y a un NULL avant la fin du tableau on renvoie sa position
				return firstFree;
		}

		//sinon on renvoie la taille du tableau
		return nodes_.size();
	}

	void Net::toXml(std::ostream& stream){
		stream << indent++ << "<net name=\"" << name_ << "\" type=\"" << Term::toString(type_) << "\">\n";
		for(vector<Node*>::iterator iN = nodes_.begin(); iN != nodes_.end(); ++iN)
			if(*iN != NULL) //il est possible que le tableau soit troué donc on teste
				(*iN)->toXml(stream);
		for(vector<Line*>::iterator iL = lines_.begin(); iL != lines_.end(); ++iL)
			(*iL)->toXml(stream);
		stream << --indent << "</net>\n";
	}


	//setters
	void  Net::add ( Line* line ){
		if (line) lines_.push_back( line );
	}

	bool  Net::remove(Line* line){
		if(line){
			for(vector<Line*>::iterator il = lines_.begin(); il != lines_.end() ; ++il){
				if(*il == line){
					lines_.erase(il);
					return true;
				}
			}
		}
		return false;
	}

	void Net::add(Node* node){
		cerr << " add node " << node->getId() << " to " << getName() << endl;
		if(node->getId() == Node::noid) //si il n'y a pas d'id on en récupère un
			node->setId(getFreeNodeId());

		while(node->getId() >= nodes_.size()) //on augmente si besoin la taille du tableau
			nodes_.push_back(NULL);

		nodes_[node->getId()] = node; //on ajoute le node à la position de son id
	}

	void Net::remove(Node* node){
		vector<Node*>::iterator iN = nodes_.begin();
		for(; iN != nodes_.end(); ++iN){
			if(*iN == node) //si node et l'iterator pointe sur le même Node
				nodes_.erase(iN);
		}
	}

	Net* Net::fromXml(Cell* cell, xmlTextReaderPtr xmlPtr){
		string name;
		string typeStr;
		Term::Type type;

		xmlChar* nameXml;
		xmlChar* typeXml;

		const xmlChar* nodeTag = xmlTextReaderConstString(xmlPtr, (const xmlChar*)"node");
		const xmlChar* lineTag = xmlTextReaderConstString(xmlPtr, (const xmlChar*)"line");
		const xmlChar* netTag = xmlTextReaderConstString(xmlPtr, (const xmlChar*)"net");

		const xmlChar* nameTag = xmlTextReaderConstString(xmlPtr, (const xmlChar*)"name");
		const xmlChar* typeTag = xmlTextReaderConstString(xmlPtr, (const xmlChar*)"type");

		if((nameXml = xmlTextReaderGetAttribute(xmlPtr, nameTag)) == NULL)
			return NULL;

		if((typeXml = xmlTextReaderGetAttribute(xmlPtr, typeTag)) == NULL)
			return NULL;

		name = xmlCharToString(nameXml);
		typeStr = xmlCharToString(typeXml);

		type = Term::toType(typeStr);

		Net* net = new Net(cell, name, type);

		cerr << "Net : " << name << endl;

		do{
			if(xmlTextReaderRead(xmlPtr) != 1)
				return NULL;

			if(xmlTextReaderConstLocalName(xmlPtr) == nodeTag)
				Node::fromXml(net, xmlPtr);

			if(xmlTextReaderConstLocalName(xmlPtr) == lineTag)
				Line::fromXml(net, xmlPtr);
		} while (xmlTextReaderConstLocalName(xmlPtr) != netTag and xmlTextReaderNodeType(xmlPtr) != XML_READER_TYPE_END_ELEMENT);

		return net;
	}
}