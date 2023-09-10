#ifndef Game_HPP
#define Game_HPP

#include "parametres.hpp"
#include "mesClasses.hpp"

class Game
{
    public:
    	Game(ALLEGRO_DISPLAY *display);  // constructeur
		~Game(); // Destructeur

    // Méthodes (prototypes)
    	// Getters
        bool isGameOver();
        string getNomUser();


    	// Setters
        void setPerso(string nom);
        void setNomUser(string nom);
        void setWindowPOS(int windowX,int windowY, ALLEGRO_DISPLAY *display);
        void setWindowX(int windowX, ALLEGRO_DISPLAY *display);
        void setWindowY(int windowY, ALLEGRO_DISPLAY *display);
        void setGameOver(bool isOver);


    	// Autres
        void begin(ALLEGRO_EVENT_QUEUE *event_queue); // début du jeu (saisie du nom puis lancement du jeu sur map0)
        void erreur(const char* txt);
        string saisirUserName(void);
        void load_maps(void);
        void update();
        void tracerAccueil();
        void changeMap();

    private :
        void charge_polices();
        void detruit_polices();
        int createMap0();


    protected:
	// Attributs
        // Joueur
        string nomUser; // pour l'affichage en haut du texte "User : ? "
        int score;
        Duree temps;
        int vies;


        // Affichage
        ALLEGRO_DISPLAY *display;
        ALLEGRO_FONT *polices[NBR_FONTS];
        int window_height;
        int window_width;
        int window_x;
        int window_y;
        float RATIO_FRAME;
     
        // Jeu
        User *perso;
        int level;
        bool fullscreen;
        bool playsound;
        int gravity;
        int jump_force;
        bool gameOver;
        vector<Map> maps;
        vector<Bloc> blocs;
        vector<Mechant> mechants;
        vector<ObjectLance> objets;

        // Repères
        int base_sol;    // position de base du sol au niveau de la creation de la map
        int sol;         // position actuelle du sol, la limite de descente verticale du perso (le dessus d'un bloc mystère par exemple)
        int nbrBlocsSol; // nbr de blocs constituant le sol (de 0 a nbrBlocsSol dans blocs)
        int num_map;     // numéro de la map actuelle
        int entree;      // indice du bloc d'entree
        int sortie;      // indice du bloc de sortie

        // Accueil
        string msg_accueil;
        int posHidePart;  // Position de la partie cachée du texte
        int posPrintPart;    // Position de la partie affichée du texte

        // Menu Echap
        vector<bouton> listeBut;

        // Musiques
        ALLEGRO_VOICE *voice;
        ALLEGRO_MIXER *mixer;
        map <string,Sound*> mesSons;


        // Images
        ALLEGRO_BITMAP *wallpaper;
        ALLEGRO_BITMAP *logo;


        // Moments du jeu
        bool enter; // si on a validé le début du jeu


        // Anim
        bool anim_fin;
        bool anim_tuyau;
        bool anim_entree;
        bool anim_sortie;
        bool anim_perteVie;
};

#endif