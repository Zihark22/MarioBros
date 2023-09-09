#ifndef USER_HPP
#define USER_HPP

#include "parametres.hpp"
#include "personnage.hpp"

class User : public Personnage
{
    public:
		User() : Personnage() {
			for (int i = 0; i < IMGS_PERSOS; ++i)
				imgs[i] = NULL;
			this->h=0;
			this->w=0;
			this->msg="";
		}
		User(string nom) : Personnage(nom) // constructeur 
		{
			string chemin;
			if(nom=="_") {
				for (int i = 0; i < IMGS_PERSOS; ++i)
					imgs[i] = NULL;
				this->h=0;
				this->w=0;
			}
			else {
				for (int i = 0; i < IMGS_PERSOS; ++i) {
					chemin="datas/persos/"+this->nom+"/"+this->nom+"_"+to_string(i)+".png";
					imgs[i] = al_load_bitmap(chemin.c_str());
					if(!imgs[i]) {
						cerr << "Erreur : chargement du perso = " << this->nom << " et de l'image = " << chemin << endl;
					}
				}
				if(imgs[0]) {
					this->h=al_get_bitmap_height(imgs[0]);
					this->w=al_get_bitmap_width(imgs[0]);
				}
				else {
					this->h=0;
					this->w=0;
				}
			}
			this->msg="";
		}
    	~User() { // Destructeur
			for (int i = 0; i < IMGS_PERSOS; ++i) {
				if(imgs[i])
					al_destroy_bitmap(imgs[i]);
			}
		}
		User(User const& userACopier); // constructeur de copie

    // Méthodes (prototypes)
    	// Getters
		ALLEGRO_BITMAP* getImg(int indice) const;
    	string getMessage() const;

    	// Setters
		void setImg(const char* chemin, int indice);
		void setImg(ALLEGRO_BITMAP* img, int indice);
    	void setMessage(string message);

    	// Autres
    	void actualiseSize(int indice);
		void draw(int indice);
    	void afficherMessage(ALLEGRO_FONT *font);

    protected :
	    // Attributs
	    ALLEGRO_BITMAP *imgs[IMGS_PERSOS];
		string msg;
};


#endif