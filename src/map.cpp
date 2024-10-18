#include "../include/map.hpp"

Map::Map(string chemin_img) : Element(chemin_img, 1) // constructeur classique
{
    this->backgroundScale=1;
    this->backgroundX=0;
    this->Map0=true;
}
// Map::~Map() // Destructeur
// {
//     al_destroy_bitmap(this->img);
// }
void Map::draw(int width, int height) const {
    if(this->Map0)
        al_draw_scaled_bitmap(this->img, 0, 0, this->w, this->h, 0, HAUTEUR_TEXTE, width, height, 0);
    else
        al_draw_scaled_bitmap(this->img, 0, 0, this->w, this->h, this->backgroundX, HAUTEUR_TEXTE, this->w * this->backgroundScale, this->h * this->backgroundScale, 0);
}
void Map::setBackgroundScale(float scale) {
    this->backgroundScale=scale;
}
void Map::setBackgroundX(int pos) {
    this->backgroundX=pos;
}
void Map::setMap0(bool Map0) {
    this->Map0=Map0;
}
float Map::getBackgroundScale() const {
    return this->backgroundScale;
}
int Map::getBackgroundX() const {
    return this->backgroundX;
}
bool Map::isMap0() const {
    return this->Map0;
}
