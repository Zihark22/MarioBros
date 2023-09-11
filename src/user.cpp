#include "../include/user.hpp"

User::User(User const& userACopier) :  msg(userACopier.msg) 
{
    map<string,ALLEGRO_BITMAP*>::iterator it;  //Un itérateur
    for (auto it=userACopier.imgs.begin(); it!=userACopier.imgs.end(); ++it) {
        if(it->second)
            imgs[it->first] = al_clone_bitmap(it->second); // copie des images
        else
            imgs[it->first] = nullptr;
    }
}
string User::indiceToKey(int indice) const {
    string orientation;
    switch(indice) {
        case SUD:
            orientation="FACE";
            break;
        case EST:
        case OUEST:
            orientation="COTE";
            break;
        case NORD:
            orientation="DOS";
            break;
        case RUNR:
        case RUNL:
            orientation="COURIR";
            break;
        case COUCHER:
        case COUCHEL:
            orientation="BAISSE";
            break;
        default:
            orientation="FACE";
            break;
    }
    return orientation;
}
void User::actualiseSize(int indice) {
    string orientation=indiceToKey(indice);
    this->w = al_get_bitmap_width( imgs[orientation]) * this->tailleFactor;
    this->h = al_get_bitmap_height(imgs[orientation]) * this->tailleFactor;
}
ALLEGRO_BITMAP* User::getImg(int indice) const {
    string orientation=indiceToKey(indice);

    // Utilisez la fonction find pour rechercher l'image par clé
    auto it = imgs.find(orientation);

    // Vérifiez si l'image a été trouvée
    if (it != imgs.end()) {
        return it->second;
    } 
    else {
        // Retournez nullptr (ou une valeur par défaut) si l'image n'est pas trouvée
        return nullptr; // Vous pouvez aussi choisir de renvoyer une image par défaut ici si nécessaire
    }
}
void User::setImg(const char* chemin, int indice){
    string orientation=indiceToKey(indice);  
    al_destroy_bitmap(this->imgs[orientation]);
    this->imgs[orientation]=al_load_bitmap(chemin);
    if(!imgs[orientation]) {
        cerr << "Erreur : chargement du perso" << endl;
    }
}
void User::setImg(ALLEGRO_BITMAP* img, int indice) {
    string orientation=indiceToKey(indice);  
    this->imgs[orientation]=img;
    if(!imgs[orientation]) {
        cerr << "Erreur : chargement de l'image " << orientation << " du perso" << nom << endl;
    }
}
string User::getMessage() const {
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
        
        string orientation=indiceToKey(indice);  
        if(imgs[orientation]) {
            if(gauche)
                al_draw_scaled_bitmap(this->imgs[orientation], 0, 0, al_get_bitmap_width(imgs[orientation]), al_get_bitmap_height(imgs[orientation]), this->coord.x, this->coord.y, this->w, this->h, ALLEGRO_FLIP_HORIZONTAL);
            else
                al_draw_scaled_bitmap(this->imgs[orientation], 0, 0, al_get_bitmap_width(imgs[orientation]), al_get_bitmap_height(imgs[orientation]), this->coord.x, this->coord.y, this->w, this->h, 0);
        }
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