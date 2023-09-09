#ifndef BLOC_HPP
#define BLOC_HPP

#include "parametres.hpp"
#include "element.hpp"

class Bloc : public Element // ex : bloc de terre , tuyau, nuage, sol, ...
{
    public:
    	Bloc(string chemin_img, int posx, int posy, int angle, float scale, bool object, bool hiding, int type) 
		: Element(chemin_img, scale), angle(angle), object(object), hiding(hiding), type(type) // Constructeur
		{
			this->coord.x=posx;
    		this->coord.y=posy;
			this->sortie_objet=false;
    		this->adjust=false;
		};
		Bloc(const Bloc& other) : Element(other), coord(other.coord), angle(other.angle), type(other.type), object(other.object), hiding(other.hiding), adjust(other.adjust), sortie_objet(other.sortie_objet) {};
		
    // Méthodes (prototypes)
    	// Getters
    	POS getCoord() const;
    	int getAngle() const;
    	int getType() const;
    	bool getSortieObjet() const;

    	// Setters
    	void setCoord(POS newCoord);
    	void setCoord(int x, int y);
    	void setAngle(int angle);
    	void setHiding(bool hide);
    	void setSortieObjet(bool sortie);

    	// Autres
    	void draw();
    	bool isHiding();
    	bool isObject();

    private :
	    // Attributs
		POS coord;
		int angle; // radians
		bool adjust;
		bool hiding; // bloc mystere cachant un objet
		bool object;
		bool sortie_objet;
		int type;
};

#endif