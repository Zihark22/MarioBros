#ifndef Game_HPP
#define Game_HPP

#include "parametres.hpp"
#include "mesClasses.hpp"

class Game
{
    public:
    	Game();  // constructeur
		~Game(); // Destructeur

    // Méthodes (prototypes)
    	// Getters
        bool isGameOver();
        string getNomUser();
        bool getStarted();
        bool getMenuSelected();
        POS getMousePos();
        ALLEGRO_DISPLAY* getDisplay();

    	// Setters
        void setPerso(string nom);
        void setNomUser(string nom);
        void setWindowX(int windowX);
        void setWindowY(int windowY);
        void setGameOver(bool isOver);
        void setMenuSelected(bool menu);
        void setMousePos(int x, int y);

    	// Bases
        void erreur(string txt);
        string saisirUserName(void);
        void load_maps(void);
        void begin(ALLEGRO_EVENT_QUEUE *event_queue); // début du jeu (saisie du nom puis lancement du jeu sur map0)

        // Dessins
        void dessine();
        void dessineMenu();
        void afficherParametres();
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
        int createMap1();
        void afficheCommandes();
        void masqueRGB(ALLEGRO_DISPLAY *display, ALLEGRO_BITMAP *image, bool R, bool G, bool B);

        // Mouvements
        void checkAnimations();

    //---------- COLLISIONS ----------//
        float calculateAngle(VECT2D const& vectorA, VECT2D const& vectorB);
        float conv_to_Rad(float const& degrees);
        float conv_to_Deg(float const& rad);
        // perso
        int collisionPersoBloc(User const& perso, Bloc const& bloc);
        // int collisionPersoMechant(User const& perso, Mechant const& mechant);
        // handle
        void handleCollisions();

    protected:
	// Attributs
        // Joueur
        string nomUser; // pour l'affichage en haut du texte "User : ? "
        int score;
        Duree temps;
        int vies;

        // Affichage
        shared_ptr<ALLEGRO_DISPLAY>  display;
        unordered_map<string, ALLEGRO_FONT*> polices;
        int window_height;
        int window_width;
        int window_x;
        int window_y;
        float RATIO_FRAME;
        float agrandi_fact;
     
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
        int cmptFrames;     // compte le défilement des frames pour le calcul du temps
        int cmptAnim;       // Pour les animations
        POS souris;         // position de la souris
        bool isOrientedLeft;// savoir l'orientation du perso pour quand il se relève

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