#ifndef OBJECTLANCE_HPP
#define OBJECTLANCE_HPP

#include "parametres.hpp"
#include "personnage.hpp"

class ObjectLance : public Personnage
{
    public:
    	ObjectLance(string nom);  // Constructeur
    	~ObjectLance() { // Destructeur
			if(img)
				al_destroy_bitmap(img);
		}

    // Méthodes (prototypes)
    	// Getters
		
    	// Setters

		void draw();
		void actualiseSize();

    protected:
	    // Attributs
	    ALLEGRO_BITMAP *img;
	    float angle;
};

#endif