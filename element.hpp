#ifndef ELEMENT_HPP
#define ELEMENT_HPP

#include "parametres.hpp"

class Element
{
    public:
    	Element(string chemin_img, float scale) : scale(scale) {  // constructeur
			this->img=al_load_bitmap(chemin_img.c_str());
			if(!this->img) {
				// erreur("chargement du bloc");
				this->h=0;
				this->w=0;
			}
			else {
				this->h=al_get_bitmap_height(img)*scale;
				this->w=al_get_bitmap_width(img)*scale;
			}
			this->afficher=true;
		}; 
		Element(const Element& other) : scale(other.scale), afficher(other.afficher), h(other.h), w(other.w), img(al_clone_bitmap(other.img)) {} // Constructeur de copie
		// ~Element() // Destructeur
		// {
		// 	if(al_get_bitmap_height(this->img)>0)
		// 		al_destroy_bitmap(this->img);
		// }

    // Méthodes (prototypes)
    	// Getters
    	int getH() const;
    	int getW() const;
    	float getScale() const;
    	ALLEGRO_BITMAP* getImg() const;
    	bool is_enable() const;

    	// Setters
    	void setH(int newH);
    	void setW(int newW);
    	void setScale(float newScale);
    	void setImg(string chemin);

    	// Autres
    	void disable(void);
    	void enable(void);

    protected:
	    // Attributs
	    ALLEGRO_BITMAP *img;
	    float scale;
		int h;
		int w;
		bool afficher;
};

#endif