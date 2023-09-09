#ifndef ELEMENT_HPP
#define ELEMENT_HPP

#include "parametres.hpp"

class Element
{
    public:
    	Element(string chemin_img, float scale);  // constructeur
		Element(const Element& other) : scale(other.scale), afficher(other.afficher), h(other.h), w(other.w), img(al_clone_bitmap(other.img)) {} // Constructeur de copie
		virtual ~Element(); // Destructeur

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