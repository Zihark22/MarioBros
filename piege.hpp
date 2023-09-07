#ifndef PIEGE_HPP
#define PIEGE_HPP

#include "parametres.hpp"
#include "bloc.hpp"

class Piege : public Bloc // ex : pics, traits de feu, ...
{
    public:
    	Piege(const char* chemin_img, int posx, int posy, int angle, float scale, bool object, bool hiding, int type): Bloc(chemin_img, posx, posy, angle, scale, object, hiding, type) { } ;  // Constructeur
    	~Piege(); // Destructeur

    // Méthodes (prototypes)
    	// Getters

    	// Setters

    	// Autres

    private :
	    // Attributs
};

#endif