#include "../include/objectlance.hpp"

ObjectLance::ObjectLance(string nom) : Personnage(nom) { // Constructeur
    string chemin;
    chemin="images/"+this->nom+".png";
    this->img = al_load_bitmap(chemin.c_str());
    if(!this->img)
        cerr << "Erreur : chargement du perso : " << this->nom << endl;
    this->h=al_get_bitmap_height(img);
    this->w=al_get_bitmap_width(img);
    this->angle=0;
}
void ObjectLance::draw() {
    if(this->afficher) {
        this->angle+=PI/6;
        if(this->angle>=2*PI)
            this->angle/=2*PI;
        al_draw_scaled_rotated_bitmap(this->img, al_get_bitmap_width(this->img)/2, al_get_bitmap_height(this->img)/2, this->coord.x+this->w/2, this->coord.y+this->h/2, this->tailleFactor, this->tailleFactor, this->angle, 0); // -3.14/2 = 90° sens trigo
    }
}
void ObjectLance::actualiseSize() {
    this->w = al_get_bitmap_width(this->img)  * this->tailleFactor;
    this->h = al_get_bitmap_height(this->img) * this->tailleFactor;
}
