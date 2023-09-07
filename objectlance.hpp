#ifndef OBJECTLANCE_HPP
#define OBJECTLANCE_HPP

#include "parametres.hpp"
#include "personnage.hpp"

class ObjectLance : public Personnage
{
    public:
    	ObjectLance(string nom);  // Constructeur
    	~ObjectLance(); // Destructeur

    // Méthodes (prototypes)
    	// Getters

    	// Setters

		void draw();
		void actualiseSize();

    private:
	    // Attributs
	    ALLEGRO_BITMAP *img;
	    float angle;
};

#endif