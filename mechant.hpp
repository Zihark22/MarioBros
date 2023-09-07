#ifndef MECHANT_HPP
#define MECHANT_HPP

#include "parametres.hpp"
#include "personnage.hpp"

class Mechant : public Personnage
{
    public:
    	Mechant(string nom);  // Constructeur
    	~Mechant(); // Destructeur

    // Méthodes (prototypes)
    	// Getters
		ALLEGRO_BITMAP* getImg(int indice);
    	bool getEcrase();

    	// Setters
		void setImg(const char* chemin, int indice);
		void setImg(ALLEGRO_BITMAP* img, int indice);
    	void setEcrase(bool newEcrase);
		void setVersDroite(bool droite);
    	void changeDir();

    	// Autres
    	void actualiseSize(int indice);
		void draw();

    private:
	    // Attributs
	    ALLEGRO_BITMAP *imgs[IMGS_MECHANTS];
		bool versDroite;
		int indice;
		bool ecrase;
};


#endif