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
        void erreur(const char* txt);
        string saisirUserName(void);
        void load_maps(void);
        void update();
        void tracerAccueil();

    private :
        void charge_polices();
        void detruit_polices();


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
     
        // Jeu
        User perso;
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

        // Accueil
        string msg_accueil;
        int posHidePart;  // Position de la partie cachée du texte
        int posPrintPart;    // Position de la partie affichée du texte

        // Menu Echap
        vector<bouton> listeBut;

        // Musiques
        ALLEGRO_VOICE *voice;
        ALLEGRO_MIXER *mixer;
        Sound music;
        Sound son_finish;
        Sound son_jump;
        Sound son_tuyau;
        Sound son_sol;
        Sound son_mario;
        Sound son_luigi;
        Sound son_coin;
        Sound son_over;
        Sound son_powerUp;
        Sound son_powerDown;
        Sound son_fireBall;
        Sound son_fireBallHit;
        Sound son_ecrase;
        Sound son_koopa_shell;


        // Images
        ALLEGRO_BITMAP * wallpaper;
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