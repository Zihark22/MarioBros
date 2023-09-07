#include "bloc.hpp"

POS Bloc::getCoord() const {
    return this->coord;
}
int Bloc::getAngle() const {
    return this->angle;
}
void Bloc::setCoord(POS newCoord) {
    this->coord = newCoord;
}
void Bloc::setCoord(int x, int y) {
    POS newCoord = {x,y};
    this->coord = newCoord;
}
void Bloc::setAngle(int angle) {
    this->angle = angle;
}
void Bloc::setHiding(bool hide) {
    this->hiding=hide;
}
void Bloc::draw() {
    float tourne=0;
    switch(this->angle)
    {
        case ZERO:
            tourne=0;
            break;
        case DROITE:
            tourne=PI/2;
            break;
        case GAUCHE:
            tourne=3*PI/2;
            break;
        case INVERSION:
            tourne=PI;
            break;
        default :
            tourne=0;
            break;
    }
    if(afficher)
        al_draw_scaled_rotated_bitmap(this->img, al_get_bitmap_width(this->img)/2, al_get_bitmap_height(this->img)/2, this->coord.x+this->w/2, this->coord.y+this->h/2, this->scale, this->scale, tourne, 0); // -3.14/2 = 90° sens trigo
}
bool Bloc::isHiding() {
    return this->hiding;
}
bool Bloc::isObject() {
    return this->object;
}
int Bloc::getType() const {
    return this->type;
}
bool Bloc::getSortieObjet() const{
    return this->sortie_objet;
}
void Bloc::setSortieObjet(bool sortie){
    this->sortie_objet=sortie;
}
