#ifndef MARIO_HPP
#define MARIO_HPP

#include "parametres.hpp"   // sera aussi inclus dans les classes mais ifndef commandes préprocesseurs permettent 1 seule inclusion
#include "mesClasses.hpp"   // inclusion des classes
#include "game.hpp"

///////////////////////////////////////////////
///////////////// POLICES ////////////////////
//////////////////////////////////////////////
// #define ARIAL_BOLD_36 al_load_font("polices/Arial Bold.ttf", 36, 0)

///////////////////////////////////////////////
///////////// VARIABLE GLOBALES //////////////
//////////////////////////////////////////////
// extern User perso;
// extern vector<Map> maps;
// extern vector<Bloc> blocs;
// extern vector<Mechant> mechants;
// extern vector<ObjectLance> objets;
// extern bool objetsCol[MAX_OBJETS];
// extern POS coordCol;

// extern Sound *music;
// extern Sound *son_finish;
// extern Sound *son_jump;
// extern Sound *son_tuyau;
// extern Sound *son_sol;
// extern Sound *son_mario;
// extern Sound *son_luigi;
// extern Sound *son_coin;
// extern Sound *son_over;
// extern Sound *son_powerUp;
// extern Sound *son_powerDown;
// extern Sound *son_fireBall;
// extern Sound *son_fireBallHit;
// extern Sound *son_ecrase;
// extern Sound *son_koopa_shell;

// extern int entree;
// extern int sortie;
// extern int nbrBlocsSol;
// extern int nbrObjets;
// extern int nbrMechants;
// extern int indice_sol;
// extern int sol;
// extern int base_sol;
// extern int window_height;
// extern int window_width;
// extern int JUMP_FORCE;
// extern int GRAVITY;

// extern int level;
// extern int vies;
// extern int score;
// extern int temps;
// extern string nomUser;
// extern float AGRANDI_FACT;
// extern float RATIO_FRAME;
// extern int num_map;
// extern int perso_num_img;
// extern int anim_tuyau;

// extern bool key[KEY_MAX];
// extern bool stop;
// extern bool playSound;
// extern bool enter;
// extern bool lanceObjet;
// extern bool fin, dessine, fullscreen, jump, img_courir, baisse, anim_fin, anim_sortie, anim_entree, anim_perteVie, remonte, retreci, grabObject, sounds_on, sortie_objet;

// extern ALLEGRO_EVENT_QUEUE *event_queue;

// extern string const boutons[NBR_BOUT];

// extern string const txt;
// extern int longTxt;  // Position de la partie cachée du texte
// extern int textX; // Position de la partie affichée du texte

///////////////////////////////////////////////
//////////////// PROTOTYPES ///////////////////
//////////////////////////////////////////////

void printAllegroVersion();
int perdu(int vies);
bool confirmQuit(void);
int initialisation(void);
ALLEGRO_BITMAP* tracerAccueil(ALLEGRO_FONT* font);
bool MouseOnButton(int mouseX, int mouseY, bouton monBut);
void charge_polices();
void detruit_polices();
string choixPerso(void);
void masqueRGB(ALLEGRO_DISPLAY *display, ALLEGRO_BITMAP *image, bool R, bool G, bool B);
void load_maps();
void afficheOrientation(int num);
string saisirUserName(void);
void afficheCommandes(void);
void afficherTexte(ALLEGRO_FONT *font);
// void afficherBarreProgression(int pourcentage);
void afficheTypeBloc(int numBloc);
void restart();
void finish();

//---------- COLLISIONS ----------//
float calculateAngle(VECT2D const& vectorA, VECT2D const& vectorB);
float conv_to_Rad(float const& degrees);
float conv_to_Deg(float const& rad);
// perso
int collisionPersoBloc(User const& perso, Bloc const& bloc);
int collisionPersoMechant(User const& perso, Mechant const& mechant);
// mechants
int collision2Mechants(Mechant const& mechant1, Mechant const& mechant2);
int collisionBlocMechant(Bloc const& bloc, Mechant const& mechant);
// objets
int collisionObjetBloc(ObjectLance const& objet, Bloc const& bloc);
int collisionObjetMechant(ObjectLance const& objet, Mechant const& mechant);
// handle
void handleCollisions(); // objets et mechants

//------------ MAPS -------------//
int changeMap();
int createMap0();
int createMap1();
// int createMap2();
// int createMap3();
// int createMap4();
// int createMap5();

//------------ HACHAGE -------------//
// int hachage(char *chaine);
// int hashe(char* key);	// Fonction de hachage pour générer l'index à partir de la clé
// void insert(HashTable* table, char* key, int value);	// Fonction pour ajouter une paire clé-valeur à la table de hachage
// int get(HashTable* table, char* key);	// Fonction pour récupérer la valeur associée à une clé

#endif