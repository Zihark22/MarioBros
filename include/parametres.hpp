#ifndef PARAMETRES_HPP
#define PARAMETRES_HPP

#include <iostream>     // bases
#include <string>       // chaines de caracteres
#include <ctime>        // temps
#include <cstdlib>      // conversion et gestion mémoire
#include <fstream>      // Input/output file stream class (fichiers)
#include <cmath>        // fonctions maths
#include <vector>       // tableaux dynamiques
#include <unistd.h>	    // sleep
#include <cassert>      // assertion
#include <exception>    // exception
#include <iterator>     // iterateurs
#include <map> 			// dictionnaire ou association clé/valeur : map<clé_type, valeur_type> nom_var;  éléments triés selon leur clé. C'est en fait des pairs accessible via <utility>
#include <utility>      // accèder clé/valeur de map
#include <filesystem>   // OS obtenir l'arborescence des fichiers

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

namespace fs = std::__fs::filesystem;

using namespace std;
using namespace fs;

///////////////////////////////////////////////
///////////////// PARAMETRES /////////////////
//////////////////////////////////////////////
#define WIDTH 	1280
#define HEIGHT 	720
#define WALLPAPER "datas/images/Super_Mario_Bros_Movie.jpg"
#define LOGO "datas/images/New_Super_Mario_Bros._Logo.png"
#define NB_MAPS 6
#define MAX_BLOCS 40
#define MAX_MECHANTS 10
#define MAX_OBJETS 10
#define FRAME_RATE 20
#define TIME_STEP 1.0/FRAME_RATE
#define MAX_SPEED 14.0
#define FREIN 1
#define ACCELERATION 2
#define IMGS_PERSOS 5
#define IMGS_MECHANTS 3
#define PI 3.14159
#define LOADING_ELEMENTS 52
#define HAUTEUR_TEXTE 60
#define FREQ_ECHANTILLONAGE 44100
#define NBR_FONTS 5
#define VIT_OBJ 3
#define NBR_BOUT 4
#define VITY_BONUS 2
#define LARGEUR_MAP 5000
#define TEXT_SPEED 5
#define VIT_MAP -3
#define FACTEUR_ZOOM_PERTE_VIE 2
#define TABLE_SIZE 100

///////////////////////////////////////////////
/////////////// ENUMERATIONS /////////////////
//////////////////////////////////////////////

enum { KEY_DOWN, KEY_RIGHT, KEY_LEFT, KEY_UP, KEY_Z, KEY_Q, KEY_D, KEY_S, KEY_SPACE, KEY_E, KEY_ENTER, KEY_C, KEY_MAX };
enum { SUD, EST, OUEST, NORD, RUNR, RUNL, COUCHER, COUCHEL, FIN };
enum { ONCE, LOOP, BIDIR};
enum { TERRE, CHATEAU, ESCALIER, MYSTERE, COIN, PERSO, CHAMPI, ETOILE, CHAMPI_GEANT, CHAMPI_MINI, CHAMPI_DOWN, FLEUR, CHAMPI_ROYAL, TUYAU, BLOC, BLOC_VIDE, DOOR_OPEN, DOOR_CLOSED}; // type de bloc
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
    int posX;
    int posY;
    int w;
    int h;
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

#endif