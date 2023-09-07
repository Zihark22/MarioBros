#include "user.hpp"

User::User(string nom) : Personnage(nom) // constructeur 
{
    // chargement des persos
    string chemin;
    for (int i = 0; i < IMGS_PERSOS; ++i) {
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
    this->msg="";
}
User::~User() // Destructeur
{
}
void User::actualiseSize(int indice) {
    switch(indice) {
        case SUD:
            indice=0;
            break;
        case EST:
        case OUEST:
            indice=1;
            break;
        case NORD:
            indice=2;
            break;
        case RUNR:
        case RUNL:
            indice=3;
            break;
        case COUCHER:
        case COUCHEL:
            indice=4;
            break;
        default:
            indice=0;
            break;
    }
    this->w = al_get_bitmap_width(imgs[indice])  * this->tailleFactor;
    this->h = al_get_bitmap_height(imgs[indice]) * this->tailleFactor;
}
ALLEGRO_BITMAP* User::getImg(int indice){
    switch(indice) {
        case SUD:
            indice=0;
            break;
        case EST:
        case OUEST:
            indice=1;
            break;
        case NORD:
            indice=2;
            break;
        case RUNR:
        case RUNL:
            indice=3;
            break;
        case COUCHER:
        case COUCHEL:
            indice=4;
            break;
        default:
            indice=0;
            break;
    }
    return imgs[indice];
}
void User::setImg(const char* chemin, int indice){
    switch(indice) {
        case SUD:
            indice=0;
            break;
        case EST:
        case OUEST:
            indice=1;
            break;
        case NORD:
            indice=2;
            break;
        case RUNR:
        case RUNL:
            indice=3;
            break;
        case COUCHER:
        case COUCHEL:
            indice=4;
            break;
        default:
            indice=0;
            break;
    }
    al_destroy_bitmap(this->imgs[indice]);
    this->imgs[indice]=al_load_bitmap(chemin);
    if(!imgs[indice]) {
        // erreur("chargement du perso");
        cerr << "Erreur : chargement du perso" << endl;
    }
}
void User::setImg(ALLEGRO_BITMAP* img, int indice) {
    switch(indice) {
        case SUD:
            indice=0;
            break;
        case EST:
        case OUEST:
            indice=1;
            break;
        case NORD:
            indice=2;
            break;
        case RUNR:
        case RUNL:
            indice=3;
            break;
        case COUCHER:
        case COUCHEL:
            indice=4;
            break;
        default:
            indice=0;
            break;
    }
    this->imgs[indice]=img;
    if(!imgs[indice]) {
        // erreur("chargement du perso");
        cerr << "Erreur : chargement du perso" << endl;
    }
}
string User::getMessage() {
    return this->msg;
}
void User::setMessage(string message) {
    this->msg=message;
}
void User::afficherMessage(ALLEGRO_FONT *font) {
    al_draw_text(font, ROUGE, this->coord.x+this->w, this->coord.y-20, ALLEGRO_ALIGN_CENTER, this->msg.c_str());
}
void User::draw(int indice) {
    if(afficher) {
        bool gauche=false;
        if(indice==COUCHEL || indice==OUEST || indice==RUNL)
            gauche=true;
        switch(indice) {
            case SUD:
                indice=0;
                break;
            case EST:
            case OUEST:
                indice=1;
                break;
            case NORD:
                indice=2;
                break;
            case RUNR:
            case RUNL:
                indice=3;
                break;
            case COUCHER:
            case COUCHEL:
                indice=4;
                break;
            default:
                indice=0;
                break;
        }
        if(gauche)
            al_draw_scaled_bitmap(this->imgs[indice], 0, 0, al_get_bitmap_width(imgs[indice]), al_get_bitmap_height(imgs[indice]), this->coord.x, this->coord.y, this->w, this->h, ALLEGRO_FLIP_HORIZONTAL);
        else
            al_draw_scaled_bitmap(this->imgs[indice], 0, 0, al_get_bitmap_width(imgs[indice]), al_get_bitmap_height(imgs[indice]), this->coord.x, this->coord.y, this->w, this->h, 0);
    }
/*
void al_draw_scaled_bitmap(ALLEGRO_BITMAP *bitmap,
   float sx, float sy, float sw, float sh,
   float dx, float dy, float dw, float dh, 
   int flags)

    sx - source x
    sy - source y
    sw - source width
    sh - source height
    dx - destination x
    dy - destination y
    dw - destination width
    dh - destination height
    flags - same as for al_draw_bitmap
        ALLEGRO_FLIP_HORIZONTAL - flip the bitmap about the y-axis
        ALLEGRO_FLIP_VERTICAL - flip the bitmap about the x-axis
*/
}
