#include "mechant.hpp"

Mechant::Mechant(string nom) : Personnage(nom)  // constructeur
{
    // chargement des persos
    string chemin;
    for (int i = 0; i < IMGS_MECHANTS; ++i) {
        chemin="persos/"+this->nom+"/"+this->nom+"_"+to_string(i)+".png";
        this->imgs[i] = al_load_bitmap(chemin.c_str());
        if(!this->imgs[i]) {
            // string message="chargement du perso : " + this->nom;
            // erreur(message.c_str());
            cerr << "Erreur : chargement du perso : " << this->nom << endl;
        }
    }
    this->h=al_get_bitmap_height(imgs[0]);
    this->w=al_get_bitmap_width(imgs[0]);
    this->versDroite=false;
    this->indice=0;
    this->ecrase=false;
}
Mechant::~Mechant() // Destructeur
{ 
}
ALLEGRO_BITMAP* Mechant::getImg(int indice){
    return imgs[indice];
}
void Mechant::setImg(const char* chemin, int indice){
    al_destroy_bitmap(this->imgs[indice]);
    this->imgs[indice]=al_load_bitmap(chemin);
    if(!imgs[indice]) {
        // erreur("chargement du perso");
        cerr << "Erreur : chargement du perso" << endl;
    }
}
void Mechant::setImg(ALLEGRO_BITMAP* img, int indice) {
    this->imgs[indice]=img;
    if(!imgs[indice]) {
        // erreur("chargement du perso");
        cerr << "Erreur : chargement du perso" << endl;
    }
}
void Mechant::actualiseSize(int indice) {
    this->w = al_get_bitmap_width(imgs[indice])  * this->tailleFactor;
    this->h = al_get_bitmap_height(imgs[indice]) * this->tailleFactor;
}
void Mechant::changeDir() {
    this->versDroite = !this->versDroite;
    this->vitesses.x *= -1;
}
void Mechant::setVersDroite(bool droite) {
    this->versDroite = droite;
}
bool Mechant::getEcrase() {
    return this->ecrase;
}
void Mechant::setEcrase(bool newEcrase) {
    this->ecrase=newEcrase;
    if(this->ecrase==true) {
        this->actualiseSize(2);
        this->setSpeedX(0);
    }
}
void Mechant::draw() {
    if(this->afficher) {
        if(this->ecrase)
        {
            if(this->versDroite)
                al_draw_scaled_bitmap(this->imgs[2], 0, 0, al_get_bitmap_width(this->imgs[2]), al_get_bitmap_height(this->imgs[2]), this->coord.x, this->coord.y, this->w, this->h, 0);
            else
                al_draw_scaled_bitmap(this->imgs[2], 0, 0, al_get_bitmap_width(this->imgs[2]), al_get_bitmap_height(this->imgs[2]), this->coord.x, this->coord.y, this->w, this->h, ALLEGRO_FLIP_HORIZONTAL);
        }
        else {
            this->indice++; 
            if(this->indice>5) {
                if(this->versDroite)
                    al_draw_scaled_bitmap(this->imgs[0], 0, 0, al_get_bitmap_width(this->imgs[0]), al_get_bitmap_height(this->imgs[0]), this->coord.x, this->coord.y, this->w, this->h, 0);
                else
                    al_draw_scaled_bitmap(this->imgs[0], 0, 0, al_get_bitmap_width(this->imgs[0]), al_get_bitmap_height(this->imgs[0]), this->coord.x, this->coord.y, this->w, this->h, ALLEGRO_FLIP_HORIZONTAL);
            }
            else {
                if(this->versDroite)
                    al_draw_scaled_bitmap(this->imgs[1], 0, 0, al_get_bitmap_width(this->imgs[1]), al_get_bitmap_height(this->imgs[1]), this->coord.x, this->coord.y, this->w, this->h, 0);
                else
                    al_draw_scaled_bitmap(this->imgs[1], 0, 0, al_get_bitmap_width(this->imgs[1]), al_get_bitmap_height(this->imgs[1]), this->coord.x, this->coord.y, this->w, this->h, ALLEGRO_FLIP_HORIZONTAL);
            }
            if(this->indice>10)
                this->indice=0;
        }
    }
}
