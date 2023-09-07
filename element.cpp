#include "element.hpp"

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
