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
    actualImg=al_clone_bitmap(userACopier.actualImg);
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

////////// INUTILES /////////////
ALLEGRO_BITMAP* User::getImg() const {
    return actualImg;
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
////////////////////////////////

string User::getMessage() const {
    return this->msg;
}
void User::setMessage(string message) {
    this->msg=message;
}
void User::afficherMessage(ALLEGRO_FONT *font) {
    al_draw_text(font, ROUGE, this->coord.x+this->w, this->coord.y-20, ALLEGRO_ALIGN_CENTER, this->msg.c_str());
}
void User::changeActualImg(string mouv) {
    if(mouv=="courirD") {
        actualImg=imgs["courir"];
        inverse=false;
        w=al_get_bitmap_width(actualImg);
        h=al_get_bitmap_height(actualImg);
    }
    else if(mouv=="courirG") {
        actualImg=imgs["courir"];
        inverse=true;
        w=al_get_bitmap_width(actualImg);
        h=al_get_bitmap_height(actualImg);
    }
    else if(mouv=="droite") {
        coord.y+=h;
        actualImg=imgs["cote"];
        inverse=false;
        w=al_get_bitmap_width(actualImg);
        h=al_get_bitmap_height(actualImg);
        coord.y-=h;
    }
    else if(mouv=="gauche") {
        coord.y+=h;
        actualImg=imgs["cote"];
        inverse=true;
        w=al_get_bitmap_width(actualImg);
        h=al_get_bitmap_height(actualImg);
        coord.y-=h;
    }
    else if(mouv=="sauter") {
        actualImg=imgs["face"];
        inverse=false;
        w=al_get_bitmap_width(actualImg);
        h=al_get_bitmap_height(actualImg);
    }
    else if(mouv=="entrer") {
        actualImg=imgs["dos"];
        inverse=false;
        w=al_get_bitmap_width(actualImg);
        h=al_get_bitmap_height(actualImg);
    }
    else if(mouv=="baisserD") {
        coord.y+=h;
        actualImg=imgs["baisser"];
        inverse=false;
        w=al_get_bitmap_width(actualImg);
        h=al_get_bitmap_height(actualImg);
        coord.y-=h;
    }
    else if(mouv=="baisserG") {
        coord.y+=h;
        actualImg=imgs["baisser"];
        inverse=true;
        w=al_get_bitmap_width(actualImg);
        h=al_get_bitmap_height(actualImg);
        coord.y-=h;
    }
    else
        cerr << "Erreur : mouvement " << mouv << " non existant !" << endl;
}
void User::draw() {
    if(afficher) {
        al_draw_scaled_bitmap(actualImg, 0, 0, w, h, coord.x, coord.y, w*tailleFactor, h*tailleFactor, inverse); // ALLEGRO_FLIP_HORIZONTAL
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