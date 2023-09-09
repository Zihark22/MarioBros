#ifndef PERSONNAGE_HPP
#define PERSONNAGE_HPP

#include "parametres.hpp"

class Personnage
{
    public:
    	Personnage();  			 // Constructeur par défault
		Personnage(string nom);  // Constructeur
    	virtual ~Personnage(); 	 // Destructeur

    // Méthodes (prototypes)
    	// Getters
    	POS getPos() const;
    	int getH() const;
    	int getW() const;
    	string getNom() const;
    	float getTaille() const;
    	VITESSE getSpeed() const;

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
		int h;
		int w;
		float tailleFactor;
		bool afficher;
		VITESSE vitesses;
};


#endif