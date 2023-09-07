#ifndef USER_HPP
#define USER_HPP

#include "parametres.hpp"
#include "personnage.hpp"

class User : public Personnage
{
    public:
    	User(string nom);  // Constructeur
    	~User(); // Destructeur

    // Méthodes (prototypes)
    	// Getters
		ALLEGRO_BITMAP* getImg(int indice);
    	string getMessage();

    	// Setters
		void setImg(const char* chemin, int indice);
		void setImg(ALLEGRO_BITMAP* img, int indice);
    	void setMessage(string message);

    	// Autres
    	void actualiseSize(int indice);
		void draw(int indice);
    	void afficherMessage(ALLEGRO_FONT *font);

    private:
	    // Attributs
	    ALLEGRO_BITMAP *imgs[IMGS_PERSOS];
		string msg;
};


#endif