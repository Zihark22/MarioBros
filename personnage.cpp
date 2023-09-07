#include "personnage.hpp"

Personnage::Personnage(string nom) // constructeur 
{
    this->nom=nom;
    this->coord.x=0; this->coord.y=0;
    this->afficher=true;
    this->vitesses.x=0;
    this->vitesses.y=0;
    this->tailleFactor=1;
}
Personnage::~Personnage() // Destructeur
{ 
    /* Rien à mettre ici car on ne fait pas d'allocation dynamique
    dans la classe Personnage. Le destructeur est donc inutile mais
    je le mets pour montrer à quoi cela ressemble.
    En temps normal, un destructeur fait souvent des delete et quelques
    autres vérifications si nécessaire avant la destruction de l'objet. */
}
string Personnage::getNom()
{
    return this->nom;
}
POS Personnage::getPos() // constructeur classique
{
    return this->coord;
}
int Personnage::getH() {
    return this->h;
}
int Personnage::getW() {
    return this->w;
}
float Personnage::getTaille() {
    return this->tailleFactor;
}
void Personnage::setH(int newH) {
    this->h = newH;
}
void Personnage::setW(int newW) {
    this->w = newW;
}
void Personnage::setPos(POS newCoord) // constructeur classique
{
    this->coord=newCoord;
}
void Personnage::setPos(int x, int y) 
{
    this->coord.x=x;
    this->coord.y=y;
}
void Personnage::setPosX(int x) {
    this->coord.x=x;
}
void Personnage::setPosY(int y) {
    this->coord.y=y;
}
VITESSE Personnage::getSpeed() {
    return this->vitesses;
}
void Personnage::setSpeed(int newSpeedX, int newSpeedY) {
    this->vitesses.x=newSpeedX;
    this->vitesses.y=newSpeedY;
}
void Personnage::setSpeedX(int newSpeedX) {
    this->vitesses.x=newSpeedX;
}
void Personnage::setSpeedY(int newSpeedY) {
    this->vitesses.y=newSpeedY;
}
void Personnage::setTaille(float newTailleFACTOR) {
    this->tailleFactor=newTailleFACTOR;
}
void Personnage::actualisePos() {
    this->coord.x+=this->vitesses.x;
    this->coord.y+=this->vitesses.y;
}
void Personnage::setAfficher(bool affiche) {
    this->afficher=affiche;
}
