#include "../include/element.hpp"

Element::Element(string chemin_img, float scale) : scale(scale) {  // constructeur
    this->img=al_load_bitmap(chemin_img.c_str());
    if(!this->img) {
        // erreur("chargement du bloc");
        cerr << "Erreur : chargement image : " << chemin_img << endl;
        this->h=0;
        this->w=0;
    }
    else {
        this->h=al_get_bitmap_height(img)*scale;
        this->w=al_get_bitmap_width(img)*scale;
    }
    this->afficher=true;
}; 

Element::~Element() // Destructeur
{
    // if(img)
    //     al_destroy_bitmap(img);
}

ALLEGRO_BITMAP* Element::getImg() const {
    return this->img;
}
int Element::getH() const {
    return this->h;
}
int Element::getW() const {
    return this->w;
}
float Element::getScale() const {
    return this->scale;
}
void Element::setScale(float newScale) {
    this->scale = newScale;
}
void Element::setH(int newH) {
    this->h = newH;
}
void Element::setW(int newW) {
    this->w = newW;
}
void Element::setImg(string chemin) {
    al_destroy_bitmap(this->img);
    this->img=al_load_bitmap(chemin.c_str());
    if(!img) {
        // erreur("chargement du bloc");
        cerr << "Erreur : chargement du bloc" << endl;
    }
}
void Element::disable(void) {
    this->afficher=false;
}
void Element::enable(void) {
    this->afficher=true;
}
bool Element::is_enable() const {
    return this->afficher;
}
