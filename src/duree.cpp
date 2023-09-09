#include "../include/duree.hpp"

/* ------------------- Méthodes ------------------- */

///////// CONSTRUCTEUR //////////
Duree::Duree(int heures, int minutes, int secondes) : m_heures(heures), m_minutes(minutes), m_secondes(secondes)
{
}

///////// GETTERS //////////
int Duree::getH() const 
{
    return m_heures;
}
int Duree::getM() const {
    return m_minutes;
}
int Duree::getS() const {
    return m_secondes;
}

///////// SETTERS //////////
void Duree::setH(int H) {
    m_heures = H;
}
void Duree::setM(int M) {
    m_minutes = M;
}
void Duree::setS(int S) {
    m_secondes = S;
}

///////// AUTRES //////////
void Duree::afficher() const
{
    cout << m_heures << "h" << m_minutes << "m" << m_secondes << "s" << endl;
}
bool Duree::isValide() const 
{
    if(m_heures>=0 && m_heures<24 && m_minutes>=0 && m_minutes<60 && m_secondes>=0 && m_secondes<60 )
        return true;
    else
        return false;
}


/* ------------------- Opérateurs ------------------- */

///////// COMPARAISONS //////////
bool operator==(Duree const& a, Duree const& b)
{
    return (a.getH() == b.getH() && a.getM() == b.getM() && a.getS() == b.getS());
}
bool operator!=(Duree const& a, Duree const& b)
{
    return (a.getH() || b.getH() && a.getM() || b.getM() && a.getS() || b.getS());
}
bool operator>(Duree const& a, Duree const& b)
{
    if(a.getH() > b.getH())      // heure plus grande que b
        return true;
    else if(a.getH() < b.getH()) // heure plus petite que b
        return false;
    else {                          // même heure que b
        if(a.getM() > b.getM())   // min plus grande que b
            return true;
        else if(a.getM() < b.getM()) // min plus petite que b
            return false;
        else {                          // même min que b
            if(a.getS() > b.getS())   // min plus grande que b
                return true;
            else if(a.getS() < b.getS()) // min plus petite que b
                return false;
            else                            // même sec que b
                return false;
        }
    }
}
bool operator<(Duree const& a, Duree const& b)
{
    if(a.getH() < b.getH())      // heure plus petite que b
        return true;
    else if(a.getH() > b.getH()) // heure plus grande que b
        return false;
    else {                          // même heure que b
        if(a.getM() < b.getM())   // min plus petite que b
            return true;
        else if(a.getM() > b.getM()) // min plus grande que b
            return false;
        else {                          // même min que b
            if(a.getS() < b.getS())   // min plus petite que b
                return true;
            else if(a.getS() > b.getS()) // min plus grande que b
                return false;
            else                            // même sec que b
                return false;
        }
    }
}
bool operator<=(Duree const& a, Duree const& b) {
    if(a.getH() < b.getH())      // heure plus petite que b
        return true;
    else if(a.getH() > b.getH()) // heure plus grande que b
        return false;
    else {                          // même heure que b
        if(a.getM() < b.getM())   // min plus petite que b
            return true;
        else if(a.getM() > b.getM()) // min plus grande que b
            return false;
        else {                          // même min que b
            if(a.getS() < b.getS())   // min plus petite que b
                return true;
            else if(a.getS() > b.getS()) // min plus grande que b
                return false;
            else                            // même sec que b
                return true;
        }
    }
}
bool operator>=(Duree const& a, Duree const& b) {
    if(a.getH() > b.getH())      // heure plus grande que b
        return true;
    else if(a.getH() < b.getH()) // heure plus petite que b
        return false;
    else {                          // même heure que b
        if(a.getM() > b.getM())   // min plus grande que b
            return true;
        else if(a.getM() < b.getM()) // min plus petite que b
            return false;
        else {                          // même min que b
            if(a.getS() > b.getS())   // min plus grande que b
                return true;
            else if(a.getS() < b.getS()) // min plus petite que b
                return false;
            else                            // même sec que b
                return true;
        }
    }
}




///////// ARITHMETIQUES //////////
void Duree::operator+=(const Duree& a) // raccourci
{
    //1 : ajout des secondes
    m_secondes += a.m_secondes;
    //Si le nombre de secondes dépasse 60, on rajoute des minutes
    //Et on met un nombre de secondes inférieur à 60
    m_minutes += m_secondes / 60;
    m_secondes %= 60;

    //2 : ajout des minutes
    m_minutes += a.m_minutes;
    //Si le nombre de minutes dépasse 60, on rajoute des heures
    //Et on met un nombre de minutes inférieur à 60
    m_heures += m_minutes / 60;
    m_minutes %= 60;

    //3 : ajout des heures
    m_heures += a.m_heures;
}
Duree operator+(Duree const& a, Duree const& b)
{
    Duree copie(a);   //On utilise le constructeur de copie de la classe Duree !
    copie += b;       //On appelle la méthode d'addition qui modifie l'objet 'copie'
    return copie;     //Et on renvoie le résultat. Ni a ni b n'ont changé.
}

void Duree::operator-=(const Duree& a) // raccourci
{
    if(*this == a) {
        m_heures   = 0;
        m_minutes  = 0;
        m_secondes = 0;
    }
    else if(*this > a) {
        m_heures   -= a.getH();
        m_minutes  -= a.getM();
        m_secondes -= a.getS();
    }
    else {
        m_heures   = 0;
        m_minutes  = 0;
        m_secondes = 0;
    }
}
Duree operator-(Duree const& a, Duree const& b)
{
    Duree copie(a);   //On utilise le constructeur de copie de la classe Duree !
    copie -= b;       //On appelle la méthode d'addition qui modifie l'objet 'copie'
    return copie;     //Et on renvoie le résultat. Ni a ni b n'ont changé.
}


/////////////// FLUX ////////////
ostream &operator<<(ostream &flux, Duree const& duree)
{
    duree.afficher(flux) ;
    return flux;
}
void Duree::afficher(ostream &flux) const
{
    flux << m_heures << "h" << m_minutes << "m" << m_secondes << "s";
}

istream &operator>>(istream &flux, Duree &duree) {
    flux >> duree.m_heures >> duree.m_minutes >> duree.m_secondes;
    return flux;
}


