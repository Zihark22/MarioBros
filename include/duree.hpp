#ifndef DEF_DUREE
#define DEF_DUREE

#include <iostream>
using namespace std;

class Duree
{
    public:
        Duree(int heures = 0, int minutes = 0, int secondes = 0);
        void afficher() const;
        bool isValide() const;

        // Getters
        int getH() const;
        int getM() const;
        int getS() const;

        // Setters
        void setH(int H);
        void setM(int M);
        void setS(int S);

        // Méthodes pour opérateurs
        void afficher(std::ostream &flux) const; // pour opérateur de flux : << ou amitié

        // Opérateurs de raccourcis
        void operator+=(Duree const& a);
        void operator-=(Duree const& a);

        // Opérateurs de flux
        friend istream &operator>>(istream &flux, Duree &duree);

    private:
        int m_heures;
        int m_minutes;
        int m_secondes;
};

/* ------------------- Opérateurs ------------------- */

// Opérateurs de comparaison (utilisation d'accesseurs)
bool operator==(Duree const& a, Duree const& b);
bool operator!=(Duree const& a, Duree const& b);
bool operator<=(Duree const& a, Duree const& b);
bool operator>=(Duree const& a, Duree const& b);
bool operator>(Duree const& a, Duree const& b);
bool operator<(Duree const& a, Duree const& b);

// Opérateurs arithmétiques
Duree operator+(Duree const& a, Duree const& b);
Duree operator-(Duree const& a, Duree const& b);

// Opérateurs de flux
ostream &operator<<(ostream &flux, Duree const& duree);

#endif