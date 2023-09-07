#ifndef PERSONNAGE_HPP
#define PERSONNAGE_HPP

#include "parametres.hpp"

class Personnage
{
    public:
    	Personnage(string nom);  // Constructeur
    	~Personnage(); // Destructeur

    // Méthodes (prototypes)
    	// Getters
    	POS getPos();
    	int getH();
    	int getW();
    	string getNom();
    	float getTaille();
    	VITESSE getSpeed();

    	// Setters
    	void setPos(POS newCoord);
    	void setPos(int x, int y);
    	void setPosX(int x);
    	void setPosY(int y);
    	void setH(int newH);
    	void setW(int newW);
    	void setTaille(float newTailleFACTOR);
    	void setAfficher(bool affiche);
    	void setSpeed(int newSpeedX, int newSpeedY);
    	void setSpeedX(int newSpeedX);
    	void setSpeedY(int newSpeedY);
    	void actualisePos();

    protected:
	    // Attributs
	    string nom;
	    POS coord;
		float h;
		float w;
		float tailleFactor;
		bool afficher;
		VITESSE vitesses;
};


#endif