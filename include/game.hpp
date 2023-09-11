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
        bool getStarted();
        bool getMenuSelected();
        POS getMousePos();

    	// Setters
        void setPerso(string nom);
        void setNomUser(string nom);
        void setWindowPOS(int windowX,int windowY, ALLEGRO_DISPLAY *display);
        void setWindowX(int windowX, ALLEGRO_DISPLAY *display);
        void setWindowY(int windowY, ALLEGRO_DISPLAY *display);
        void setGameOver(bool isOver);
        void setMenuSelected(bool menu);
        void setMousePos(int x, int y);

    	// Bases
        void erreur(const char* txt);
        string saisirUserName(void);
        void load_maps(void);
        void begin(ALLEGRO_EVENT_QUEUE *event_queue); // début du jeu (saisie du nom puis lancement du jeu sur map0)

        // Dessins
        void dessine();
        void dessineMenu();
        void afficherTexte();
        void tracerAccueil();

        // Changements
        void update();
        void changeMap();
        void handleKeyPressed(int keycode, ALLEGRO_EVENT_QUEUE *event_queue);
        void handleKeyUnPressed(int keycode, ALLEGRO_EVENT_QUEUE *event_queue);
        void checkMouseOnButton();
        void actionOnMenu();

    private :
        void charge_polices();
        void detruit_polices();
        int createMap0();
        void afficheCommandes();

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
        map<string,bool> move;

        // Repères
        int base_sol;       // position de base du sol au niveau de la creation de la map
        int sol;            // position actuelle du sol, la limite de descente verticale du perso (le dessus d'un bloc mystère par exemple)
        int nbrBlocsSol;    // nbr de blocs constituant le sol (de 0 a nbrBlocsSol dans blocs)
        int num_map;        // numéro de la map actuelle
        int entree;         // indice du bloc d'entree
        int sortie;         // indice du bloc de sortie
        int perso_num_img;  // indice de l'image du perso
        int cmptFrames;     // compte le défilement des frames pour le calcul du temps
        POS souris;          // position de la souris

        // Accueil
        string msg_accueil;
        int posHidePart;  // Position de la partie cachée du texte
        int posPrintPart;    // Position de la partie affichée du texte

        // Menu Echap
        vector<bouton> listeBut;
        int boutonSelected;      // obtenir le bouton selectionné pour l'action a accomplir

        // Musiques
        ALLEGRO_VOICE *voice;
        ALLEGRO_MIXER *mixer;
        map <string,Sound*> mesSons;
        bool sounds_on;

        // Images
        ALLEGRO_BITMAP *wallpaper;
        ALLEGRO_BITMAP *logo;

        // Moments du jeu
        bool started;      // si on a validé le début du jeu
        bool menuSelected; // si on a appuyé sur ECHAP pour afficher le Menu

        // Anim
        bool anim_fin;
        bool anim_tuyau;
        bool anim_entree;
        bool anim_sortie;
        bool anim_perteVie;
};

#endif