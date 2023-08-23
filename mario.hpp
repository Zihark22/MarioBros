#ifndef MARIO_HPP
#define MARIO_HPP

#include <iostream> // bases
#include <string>   // chaines de caracteres
#include <ctime>    // temps
#include <cstdlib>  // conversion et gestion mémoire
#include <fstream>  // Input/output file stream class (fichiers)
#include <cmath> // fonctions maths
#include <vector> // tableaux dynamiques
#include <unistd.h>	// sleep

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_video.h>

using namespace std; // déclaration qui permet de rendre tous les symboles du namespace std disponibles dans le scope actuel sans avoir besoin de les qualifier explicitement.

///////////////////////////////////////////////
///////////////// PARAMETRES /////////////////
//////////////////////////////////////////////
#define WIDTH 	1280
#define HEIGHT 	720
#define WALLPAPER "images/Super_Mario_Bros_Movie.jpg"
#define LOGO "images/New_Super_Mario_Bros._Logo.png"
#define NB_MAPS 6
#define MAX_BLOCS 40
#define MAX_MECHANTS 10
#define MAX_OBJETS 10
#define FRAME_RATE 30
#define TIME_STEP 1.0/FRAME_RATE
#define MAX_SPEED 8.0
#define FREIN 1
#define ACCELERATION 1
#define IMGS_PERSOS 5
#define IMGS_MECHANTS 3
#define PI 3.14159
#define LOADING_ELEMENTS 52
#define HAUTEUR_TEXTE 60
#define FREQ_ECHANTILLONAGE 44100
#define TABLE_SIZE 100
#define NBR_FONTS 5
#define VIT_OBJ 3
#define NBR_BOUT 4
#define VITY_BONUS 2
#define LARGEUR_MAP 5000
#define TEXT_SPEED 5
#define VIT_MAP -3
#define FACTEUR_ZOOM_PERTE_VIE 2

///////////////////////////////////////////////
//mapping QWERTY par défault mais ici AZERTY//
//////////////////////////////////////////////

#define KEYBOARD_Q ALLEGRO_KEY_A
#define KEYBOARD_A ALLEGRO_KEY_Q
#define KEYBOARD_Z ALLEGRO_KEY_W
#define KEYBOARD_W ALLEGRO_KEY_Z
#define KEYBOARD_M ALLEGRO_KEY_SEMICOLON
#define KEYBOARD_E ALLEGRO_KEY_E
#define KEYBOARD_R ALLEGRO_KEY_R
#define KEYBOARD_T ALLEGRO_KEY_T
#define KEYBOARD_Y ALLEGRO_KEY_Y
#define KEYBOARD_U ALLEGRO_KEY_U
#define KEYBOARD_I ALLEGRO_KEY_I
#define KEYBOARD_O ALLEGRO_KEY_O
#define KEYBOARD_P ALLEGRO_KEY_P
#define KEYBOARD_S ALLEGRO_KEY_S
#define KEYBOARD_D ALLEGRO_KEY_D
#define KEYBOARD_F ALLEGRO_KEY_F
#define KEYBOARD_G ALLEGRO_KEY_G
#define KEYBOARD_H ALLEGRO_KEY_H
#define KEYBOARD_J ALLEGRO_KEY_J
#define KEYBOARD_K ALLEGRO_KEY_K
#define KEYBOARD_L ALLEGRO_KEY_L
#define KEYBOARD_X ALLEGRO_KEY_X
#define KEYBOARD_C ALLEGRO_KEY_C
#define KEYBOARD_V ALLEGRO_KEY_V
#define KEYBOARD_B ALLEGRO_KEY_B
#define KEYBOARD_N ALLEGRO_KEY_N

///////////////////////////////////////////////
///////////////// COULEURS ///////////////////
//////////////////////////////////////////////

// Classique
#define ROUGE 	al_map_rgb(255,0,0) 	 // rouge
#define BLEU 	al_map_rgb(0,0,255) 	 // bleu
#define VERT 	al_map_rgb(0,255,0) 	 // vert
#define NOIR 	al_map_rgb(0,0,0) 		 // noir
#define BLANC 	al_map_rgb(255,255,255)  // blanc
#define JAUNE 	al_map_rgb(255,255,0) 	 // jaune
#define ROSE 	al_map_rgb(255,0,255) 	 // rose
#define CYAN 	al_map_rgb(0,255,255) 	 // cyan
#define ORANGE 	al_map_rgb(255,155,55) 	 // orange
#define VIOLET 	al_map_rgb(138,43,226) 	 // violet	
#define GRIS    al_map_rgb(55, 55, 55)   // gris

// Transparent
#define ROUGE_TR 	al_map_rgba(255,0,0,200) 
#define BLEU_TR 	al_map_rgba(0,0,255,200)
#define VERT_TR 	al_map_rgba(0,255,0,200)
#define NOIR_TR 	al_map_rgba(0,0,0,200) 
#define BLANC_TR 	al_map_rgba(255,255,255,200)
#define JAUNE_TR 	al_map_rgba(255,255,0,200) 
#define ROSE_TR 	al_map_rgba(255,0,255,200)
#define CYAN_TR 	al_map_rgba(0,255,255,200)
#define ORANGE_TR 	al_map_rgba(255,155,55,200) 	
#define VIOLET_TR 	al_map_rgba(138,43,226,200) 	
#define GRIS_TR		al_map_rgba(55, 55, 55, 200)

// Codes d'échappement ANSI pour les couleurs
#define RESET_COLOR "\033[0m"
#define SET_COLOR_NOIR "\033[30m"
#define SET_COLOR_RED "\033[31m"
#define SET_COLOR_GREEN "\033[32m"
#define SET_COLOR_YELLOW "\033[33m"
#define SET_COLOR_VIOLET "\033[34m"
#define SET_COLOR_ROSE "\033[35m"
#define SET_COLOR_CYAN "\033[36m"
#define SET_COLOR_BLANC "\033[37m"
#define SET_COLOR_BLEU "\033[38m"

///////////////////////////////////////////////
///////////////// POLICES ////////////////////
//////////////////////////////////////////////
#define ARIAL_BOLD_36 al_load_font("polices/Arial Bold.ttf", 36, 0)

///////////////////////////////////////////////
/////////////// ENUMERATIONS /////////////////
//////////////////////////////////////////////

enum { KEY_DOWN, KEY_RIGHT, KEY_LEFT, KEY_UP, KEY_Z, KEY_Q, KEY_D, KEY_S, KEY_SPACE, KEY_E, KEY_ENTER, KEY_C, KEY_MAX };
enum { SUD, EST, OUEST, NORD, RUNR, RUNL, COUCHER, COUCHEL, FIN };
enum { ONCE, LOOP, BIDIR};
enum { TERRE, CHATEAU, ESCALIER, MYSTERE, COIN, PERSO, CHAMPI, ETOILE, CHAMPI_GEANT, CHAMPI_MINI, FLEUR, CHAMPI_ROYAL, TUYAU, BLOC, BLOC_VIDE, DOOR_OPEN, DOOR_CLOSED}; // type de bloc
enum { ZERO, DROITE, GAUCHE, INVERSION };

///////////////////////////////////////////////
//////////////// STRUCTURES //////////////////
//////////////////////////////////////////////

typedef struct {
	int x;
	int y;
} POS;
typedef struct {
	float x;
	float y;
	float norme;
} VECT2D;
typedef struct {
	int x;
	int y;
} VITESSE;
typedef struct
{
    string nom;
    float posX;
    float posY;
    float w;
    float h;
    ALLEGRO_FONT* font;
    ALLEGRO_COLOR couleurTxt;
    ALLEGRO_COLOR couleurRect;
} bouton;

// ----------  HACHAGE ---------------------

// Structure pour stocker une paire clé-valeur
typedef struct {
    char key[50];
    int value;
} KeyValuePair;

// Structure de la table de hachage
typedef struct {
    KeyValuePair data[TABLE_SIZE];
} HashTable;

///////////////////////////////////////////////
///////////////// CLASSES ////////////////////
//////////////////////////////////////////////
class Sound
{
    public:
    	Sound();  // Constructeur
    	Sound(const char* newChemin, ALLEGRO_MIXER *mixer);
    	Sound(const char* newChemin, int playmode, float gain, float vitesse, ALLEGRO_MIXER *mixer);
    	~Sound(); // Destructeur

    // Méthodes (prototypes)
    	// Getters
    	int getGain();
    	int getSpeed();
    	string getChemin();
    	int getPlaymode();
    	bool is_played();

    	// Setters
    	void setChemin(const char* newChemin);
    	void setGain(float newGain);
    	void setSpeed(float newSpeed);
    	void setPlaymode(int playmode);

    	// Autres
    	void stop(void);
    	void play(void);

    protected:
	    // Attributs
	    string chemin;
	    bool played;
		float gain;
		float speed;
		int playmode;
		ALLEGRO_SAMPLE *son;
		ALLEGRO_SAMPLE_INSTANCE *spl;
};
class Personnage
{
    public:
    	Personnage(string nom);  // Constructeur
    	~Personnage(); // Destructeur

    // Méthodes (prototypes)
    	// Getters
    	POS getPos();
    	int getH();
    	int getW();
    	string getNom();
    	float getTaille();
    	VITESSE getSpeed();

    	// Setters
    	void setPos(POS newCoord);
    	void setPos(int x, int y);
    	void setPosX(int x);
    	void setPosY(int y);
    	void setH(int newH);
    	void setW(int newW);
    	void setTaille(float newTailleFACTOR);
    	void setAfficher(bool affiche);
    	void setSpeed(int newSpeedX, int newSpeedY);
    	void setSpeedX(int newSpeedX);
    	void setSpeedY(int newSpeedY);
    	void actualisePos();

    protected:
	    // Attributs
	    string nom;
	    POS coord;
		float h;
		float w;
		float tailleFactor;
		bool afficher;
		VITESSE vitesses;
};
class User : public Personnage
{
    public:
    	User(string nom);  // Constructeur
    	~User(); // Destructeur

    // Méthodes (prototypes)
    	// Getters
		ALLEGRO_BITMAP* getImg(int indice);
    	string getMessage();

    	// Setters
		void setImg(const char* chemin, int indice);
		void setImg(ALLEGRO_BITMAP* img, int indice);
    	void setMessage(string message);

    	// Autres
    	void actualiseSize(int indice);
		void draw(int indice);
    	void afficherMessage(ALLEGRO_FONT *font);

    private:
	    // Attributs
	    ALLEGRO_BITMAP *imgs[IMGS_PERSOS];
		string msg;
};
class Mechant : public Personnage
{
    public:
    	Mechant(string nom);  // Constructeur
    	~Mechant(); // Destructeur

    // Méthodes (prototypes)
    	// Getters
		ALLEGRO_BITMAP* getImg(int indice);
    	bool getEcrase();

    	// Setters
		void setImg(const char* chemin, int indice);
		void setImg(ALLEGRO_BITMAP* img, int indice);
    	void setEcrase(bool newEcrase);
		void setVersDroite(bool droite);
    	void changeDir();

    	// Autres
    	void actualiseSize(int indice);
		void draw();

    private:
	    // Attributs
	    ALLEGRO_BITMAP *imgs[IMGS_MECHANTS];
		bool versDroite;
		int indice;
		bool ecrase;
};
class ObjectLance : public Personnage
{
    public:
    	ObjectLance(string nom);  // Constructeur
    	~ObjectLance(); // Destructeur

    // Méthodes (prototypes)
    	// Getters

    	// Setters

		void draw();
		void actualiseSize();

    private:
	    // Attributs
	    ALLEGRO_BITMAP *img;
	    float angle;
};
class Element
{
    public:
    	Element(const char* chemin_img); // constructeur
    	~Element(); // Destructeur

    // Méthodes (prototypes)
    	// Getters
    	int getH();
    	int getW();
    	float getScale();
    	ALLEGRO_BITMAP* getImg();
    	bool is_enable();

    	// Setters
    	void setH(int newH);
    	void setW(int newW);
    	void setScale(float newScale);
    	void setImg(const char* chemin);

    	// Autres
    	void disable(void);
    	void enable(void);

    protected:
	    // Attributs
	    ALLEGRO_BITMAP *img;
	    float scale;
		int h;
		int w;
		bool afficher;
};
class Bloc : public Element // ex : bloc de terre , tuyau, nuage, sol, ...
{
    public:
    	Bloc(const char* chemin_img, int posx, int posy, int angle, float scale, bool object, bool hiding, int type);  // Constructeur
    	~Bloc(); // Destructeur

    // Méthodes (prototypes)
    	// Getters
    	POS getCoord();
    	int getAngle();
    	int getType();
    	bool getSortieObjet();

    	// Setters
    	void setCoord(POS newCoord);
    	void setCoord(int x, int y);
    	void setAngle(int angle);
    	void setHiding(bool hide);
    	void setSortieObjet(bool sortie);

    	// Autres
    	void draw();
    	bool isHiding();
    	bool isObject();

    private :
	    // Attributs
		POS coord;
		int angle; // radians
		bool adjust;
		bool hiding; // bloc mystere cachant un objet
		bool object;
		bool sortie_objet;
		int type;
};
class Piege : public Bloc // ex : pics, traits de feu, ...
{
    public:
    	Piege(const char* chemin_img, int posx, int posy, int angle, float scale, bool object, bool hiding, int type): Bloc(chemin_img, posx, posy, angle, scale, object, hiding, type) { } ;  // Constructeur
    	~Piege(); // Destructeur

    // Méthodes (prototypes)
    	// Getters

    	// Setters

    	// Autres

    private :
	    // Attributs
};
class Map : public Element
{
    public:
    	Map(const char* chemin_img); // Constructeur
    	~Map(); // Destructeur

		// Setters
		void setBackgroundScale(float scale);
		void setBackgroundX(int pos);
		void setMap0(bool Map0);

		// Getters
		float getBackgroundScale();
		int getBackgroundX();
		bool isMap0();

    	void draw(int width, int height);

	private :
		float backgroundScale;
		int backgroundX;
		bool Map0;
};

///////////////////////////////////////////////
///////////// VARIABLE GLOBALES //////////////
//////////////////////////////////////////////

extern ALLEGRO_FONT *polices[NBR_FONTS];
extern User *perso;
extern Map *maps[NB_MAPS];
extern Bloc *blocs[MAX_BLOCS];
extern Bloc *blocsCopy[MAX_BLOCS];
extern Mechant *mechants[MAX_MECHANTS];
extern ObjectLance *objets[MAX_OBJETS];
extern bool objetsCol[MAX_OBJETS];
extern POS coordCol;

extern Sound *music;
extern Sound *son_finish;
extern Sound *son_jump;
extern Sound *son_tuyau;
extern Sound *son_sol;
extern Sound *son_mario;
extern Sound *son_luigi;
extern Sound *son_coin;
extern Sound *son_over;
extern Sound *son_powerUp;
extern Sound *son_powerDown;
extern Sound *son_fireBall;
extern Sound *son_fireBallHit;
extern Sound *son_ecrase;
extern Sound *son_koopa_shell;

extern int entree;
extern int sortie;
extern int nbrBlocsSol;
extern int nbrBlocs;
extern int nbrObjets;
extern int nbrMechants;
extern int indice_sol;
extern int sol;
extern int base_sol;
extern int window_height;
extern int window_width;
extern int JUMP_FORCE;
extern int GRAVITY;

extern int level;
extern int vies;
extern int score;
extern int temps;
extern string nomUser;
extern float AGRANDI_FACT;
extern float RATIO_FRAME;
extern int num_map;
extern int perso_num_img;
extern int anim_tuyau;

extern bool key[KEY_MAX];
extern bool stop;
extern bool playSound;
extern bool enter;
extern bool lanceObjet;
extern bool fin, dessine, fullscreen, jump, img_courir, baisse, anim_fin, anim_sortie, anim_entree, anim_perteVie, remonte, retreci, grabObject, sounds_on, sortie_objet;

extern ALLEGRO_EVENT_QUEUE *event_queue;

extern string boutons[NBR_BOUT];

extern string txt;
extern int longTxt;  // Position de la partie cachée du texte
extern int textX; // Position de la partie affichée du texte

///////////////////////////////////////////////
//////////////// PROTOTYPES ///////////////////
//////////////////////////////////////////////

void erreur(const char* txt);
int perdu(int vies);
bool confirmQuit(void);
int initialisation(void);
void detruitRessources(ALLEGRO_BITMAP *image, ALLEGRO_TIMER* timer, ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_DISPLAY *display);
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
float calculateAngle(VECT2D vectorA, VECT2D vectorB);
float conv_to_Rad(float degrees);
float conv_to_Deg(float rad);
void afficherBarreProgression(int pourcentage);
void afficheTypeBloc(int numBloc);
void restart();
void finish();

//---------- COLLISIONS ----------//
// perso
int collisionPersoBloc(User *perso, Bloc *bloc);
int collisionPersoMechant(User *perso, Mechant *mechant);
// mechants
int collision2Mechants(Mechant *mechant1, Mechant *mechant2);
int collisionBlocMechant(Bloc *bloc, Mechant *mechant);
// objets
int collisionObjetBloc(ObjectLance *objet, Bloc *bloc);
int collisionObjetMechant(ObjectLance *objet, Mechant *mechant);
// handle
void handleCollisions(); // objets et mechants

//------------ MAPS -------------//
int changeMap();
int createMap0();
int createMap1();
int createMap2();
int createMap3();
// int createMap4();
// int createMap5();

//------------ HACHAGE -------------//
int hachage(char *chaine);
int hashe(char* key);	// Fonction de hachage pour générer l'index à partir de la clé
void insert(HashTable* table, char* key, int value);	// Fonction pour ajouter une paire clé-valeur à la table de hachage
int get(HashTable* table, char* key);	// Fonction pour récupérer la valeur associée à une clé

#endif