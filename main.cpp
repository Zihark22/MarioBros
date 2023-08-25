/* 

Compilation+Execution : 
-----------------------

    ./compileExecute.sh main fonctions


Ameloirations :
---------------
    - ajout joueur 2
    - saut écrase bloc avec fleche du bas
    - optimisation chargement image bloc une seule fois
    - bloc pieces multiples (nbr aléatoire)
    - animation mort mechant
    - drapeau de fin et de milieu de niveau
    - pieges (pics, murs, lame de feu)
    - portes secretes
    - menu sauvegarde progression
    - boule de feu supprimé trop tôt

*/

#include "mario.hpp"

///////////////////////////////////////////////
/////////////// VAR GLOBALES /////////////////
//////////////////////////////////////////////

ALLEGRO_FONT *polices[NBR_FONTS];

User *perso;
Map *maps[NB_MAPS];
Bloc *blocs[MAX_BLOCS];
Bloc *blocsCopy[MAX_BLOCS];
Mechant *mechants[MAX_MECHANTS];
ObjectLance *objets[MAX_OBJETS];
bool objetsCol[MAX_OBJETS];

Sound *music=NULL;
Sound *son_finish=NULL;
Sound *son_jump=NULL;
Sound *son_tuyau=NULL;
Sound *son_sol=NULL;
Sound *son_mario=NULL;
Sound *son_luigi=NULL;
Sound *son_coin=NULL;
Sound *son_over=NULL;
Sound *son_powerUp=NULL;
Sound *son_powerDown=NULL;
Sound *son_fireBall=NULL;
Sound *son_fireBallHit=NULL;
Sound *son_ecrase=NULL;
Sound *son_koopa_shell=NULL;

int entree=0;
int sortie=0;
int nbrBlocs=0;
int nbrObjets=0;
int nbrBlocsSol=0;
int nbrMechants=0;
int indice_sol=0;
int sol=0;
int base_sol=0;
int window_height=HEIGHT;
int window_width=WIDTH;
int anim_tuyau=0;

int JUMP_FORCE = -25;
int GRAVITY = 2;

int level=0;
int vies=3;
int score=0;
int temps=0, perso_num_img=0;
string nomUser;
float AGRANDI_FACT = 1;
int num_map=0;
bool key[KEY_MAX]={0};
bool stop=true;
bool playSound=false;
bool enter=false;
bool lanceObjet=false;
POS coordCol;

float RATIO_FRAME=window_height/HEIGHT;

bool fin=false, menu=false, dessine=false, fullscreen=false, jump=false, img_courir=false, baisse = false, anim_fin=false, anim_perteVie=false, anim_sortie=false;
bool anim_entree=false, remonte=true, retreci=false, grabObject=false, sounds_on=true, sortie_objet=false;

string boutons[NBR_BOUT] = {"RESUME" , "CONTROLS" , "SOUNDS" , "EXIT"};

string txt = "Bienvenu dans Mario Bros. !! Appuyez sur ENTRER pour JOUER";
int longTxt;  // Position de la partie cachée du texte
int textX; // Position de la partie affichée du texte

ALLEGRO_EVENT_QUEUE *event_queue=NULL;

///////////////////////////////////////////////
/////////////////// MAIN /////////////////////
//////////////////////////////////////////////

int main(int argc, char **argv)
{
    // Get the Allegro version
    int version = ALLEGRO_VERSION_INT;

    // Extract the different parts of the version
    int major = (version >> 24) & 0xFF;
    int minor = (version >> 16) & 0xFF;
    int revision = (version >> 8) & 0xFF;
    int release = version & 0xFF;

    // Print the version
    cout << "Allegro version: " << major << "." << minor << "." << revision << "." << release << endl;

    // progress-bar
    int progress=0;
    progress++; afficherBarreProgression(progress);

    // Initialisation des librairies
    initialisation();
    progress+=11; afficherBarreProgression(progress);

    // Variables
    ALLEGRO_KEYBOARD_STATE keyboard_state;
    ALLEGRO_MOUSE_STATE mouse_state;
    progress+=2; afficherBarreProgression(progress);

    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_TIMER* timer=NULL;
    ALLEGRO_EVENT ev;
    ALLEGRO_VOICE *voice=NULL;
    ALLEGRO_MIXER *mixer=NULL;
    ALLEGRO_BITMAP *wallpaper=NULL, *targetBitmap = NULL;
    ALLEGRO_BITMAP *img_fin = al_load_bitmap("images/perteVieTransparent.png");
    progress+=9; afficherBarreProgression(progress);

    int boutonSelected = 0;
    float depl = HEIGHT/NBR_BOUT/2-5;
    int orientation = 0, x=0, y=0, i=0, j=0, k=0, cmptFrames=0, tmpH=0, dirCollision=FIN;
    int cmptSortie=0, cmptVie=0, cmptRetreci=0, window_x=0, window_y=0, nbrColl=0, nbrCollMechants=0, indices_coll[nbrBlocs], indices_collMechants[nbrMechants];
    int cmptColObjet=0;
    float zoom_factor = FACTEUR_ZOOM_PERTE_VIE;  // Facteur de zoom animation perte de vie
    progress+=7; afficherBarreProgression(progress);

    perso = new User("stickman"); //    choixPerso();
    POS newPosition={0,0};
    progress+=2; afficherBarreProgression(progress);

    // Chargement des polices
    charge_polices();
    progress++; afficherBarreProgression(progress);

    // Création de la fenêtre
    al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE);  // ALLEGRO_FULLSCREEN_WINDOW
    display = al_create_display(WIDTH, HEIGHT);
    if (!display) {
        cout << endl;
        detruitRessources(wallpaper, timer, event_queue, display);
        erreur("initialise affichage");
    }
    al_set_window_title(display, "Mario Game");
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
    progress++; afficherBarreProgression(progress);

    // Change position
    al_get_window_position(display, &window_x, &window_y);
    window_x+=1000;
    al_set_window_position(display, window_x, window_y);
    progress++; afficherBarreProgression(progress);

    // Création de la file d'attente d'événements
    event_queue = al_create_event_queue();
    if (!event_queue) {
        cout << endl;
        detruitRessources(wallpaper, timer, event_queue, display);
        erreur("initialise file d'attente d'évènements");
    }
    progress++; afficherBarreProgression(progress);

    // Création d'un timer
    timer = al_create_timer(TIME_STEP);
    if(!timer) {
        cout << endl;
        erreur("initialise timer");
    }
    progress++; afficherBarreProgression(progress);

    // Chargement des maps
    load_maps();
    progress+=3; afficherBarreProgression(progress);

    // Création des blocs 
    base_sol=changeMap();
    perso->setPos(blocs[entree]->getW()/2-perso->getW()/2, blocs[entree]->getH());
    progress++; afficherBarreProgression(progress);

    // Init objets Collisions
    for (i = 0; i < MAX_OBJETS; i++)
    {
        objetsCol[i]=false;
    }
    ALLEGRO_BITMAP *explose = al_load_bitmap("images/explosion.png");
    if (!explose)
        erreur("initialise image d'accueil");

    // Charge l'audio
    voice = al_create_voice(FREQ_ECHANTILLONAGE, ALLEGRO_AUDIO_DEPTH_INT16, ALLEGRO_CHANNEL_CONF_2);
    if (!voice) {
        cout << endl;
        erreur("Initialise voice");
    }
    mixer = al_create_mixer(FREQ_ECHANTILLONAGE, ALLEGRO_AUDIO_DEPTH_FLOAT32,ALLEGRO_CHANNEL_CONF_2);
    al_attach_mixer_to_voice(mixer, voice);
    progress++; afficherBarreProgression(progress);

    // Musiques  -> Sound(const char* newChemin, int playmode, float gain, float vitesse, ALLEGRO_MIXER *mixer)
    music           = new Sound("musiques/new super mario bros (world 1-1).wav", LOOP, 0.5, 1.0, mixer);
    son_finish      = new Sound("musiques/Super Mario Bros Music-Level Complete.wav", ONCE, 1.0, 1.2, mixer);
    son_jump        = new Sound("musiques/ClassicSmallJump.wav", ONCE, 1.0, 1.0, mixer);
    son_tuyau       = new Sound("musiques/MultiplayerTeamFinish.wav", ONCE, 1.0, 1.0, mixer);
    son_mario       = new Sound("musiques/Mario_sounds/Sample_0721-mario number one wouhou.wav", ONCE, 1.0, 1.0, mixer);
    son_luigi       = new Sound("musiques/luigi_okey-dokey.wav", ONCE, 1.0, 1.0, mixer);
    son_coin        = new Sound("musiques/RedCoin.wav", ONCE, 1.0, 1.0, mixer);
    son_sol         = new Sound("musiques/FootStepBlock2.wav", ONCE, 2.0, 1.0, mixer);
    son_over        = new Sound("musiques/game_over.wav", ONCE, 1.0, 1.0, mixer);
    son_powerUp     = new Sound("musiques/power_up.wav", ONCE, 1.0, 1.0, mixer);
    son_powerDown   = new Sound("musiques/Power_Down_sound.wav", ONCE, 1.0, 1.0, mixer);
    son_fireBall    = new Sound("musiques/FireBall.wav", ONCE, 1.0, 1.0, mixer);
    son_fireBallHit = new Sound("musiques/FireBallHit1.wav", ONCE, 1.0, 1.0, mixer);
    son_ecrase      = new Sound("musiques/Goomba_Stomp_Sound.wav", ONCE, 2.0, 1.0, mixer);
    son_koopa_shell = new Sound("musiques/Koopa_Troopa_Kick_Shell.wav", ONCE, 0.5, 1.0, mixer);
    progress+=4; afficherBarreProgression(progress);

    // Liste des boutons
    bouton listeBut[NBR_BOUT];
    int indice  = 0;
    listeBut[indice] = (bouton) { "CONTINUER" , WIDTH/4, (float)((indice+1)*HEIGHT/(NBR_BOUT+2)+(indice*20)) , 2*WIDTH/4 , HEIGHT/(NBR_BOUT+2) , polices[1] , ROUGE , GRIS_TR }; indice++;
    listeBut[indice] = (bouton) { "COMMANDES" , WIDTH/4, (float)((indice+1)*HEIGHT/(NBR_BOUT+2)+(indice*20)) , 2*WIDTH/4 , HEIGHT/(NBR_BOUT+2) , polices[1] , ROUGE , GRIS_TR }; indice++;
    listeBut[indice] = (bouton) { "SONS"      , WIDTH/4, (float)((indice+1)*HEIGHT/(NBR_BOUT+2)+(indice*20)) , 2*WIDTH/4 , HEIGHT/(NBR_BOUT+2) , polices[1] , ROUGE , GRIS_TR }; indice++;
    listeBut[indice] = (bouton) { "QUITTER"   , WIDTH/4, (float)((indice+1)*HEIGHT/(NBR_BOUT+2)+(indice*20)) , 2*WIDTH/4 , HEIGHT/(NBR_BOUT+2) , polices[1] , ROUGE , GRIS_TR }; indice++;

    // Définir l'événement de redimensionnement de fenêtre
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_display_event_source(display));
    progress+=4; afficherBarreProgression(progress);

    // Début du timer
    al_start_timer(timer);
    progress++; afficherBarreProgression(progress);

    // Lancement de la musique
    playSound=false;
    progress++; afficherBarreProgression(progress);

    cout << endl;

    usleep(1000000);

    // Change position
    window_x-=1000;
    al_set_window_position(display, window_x, window_y);

    // Création de la fenêtre d'accueil
    longTxt = al_get_text_width(polices[1], txt.c_str());  // Position de la partie cachée du texte
    textX = window_width/2 - longTxt/2; // Position de la partie affichée du texte
    wallpaper=tracerAccueil(polices[1]); 

    targetBitmap = al_get_backbuffer(display); // Obtient la cible de rendu par défaut

    // Boucle du jeu
    while (!fin) {
        al_wait_for_event(event_queue, &ev);
        al_set_target_bitmap(targetBitmap); // Définit la cible de rendu

        switch (ev.type) 
        {
            case ALLEGRO_EVENT_DISPLAY_RESIZE:
                al_acknowledge_resize(display); // mettre à jour la largeur et la hauteur de la fenêtre
                window_width  = al_get_display_width(al_get_current_display());
                window_height = al_get_display_height(al_get_current_display());
                depl = window_height/NBR_BOUT/2-5;
                RATIO_FRAME=window_height/HEIGHT;
                AGRANDI_FACT*=RATIO_FRAME;
                if(enter) {
                    base_sol=changeMap();
                    temps=0;
                    sol=base_sol+1;
                    anim_entree=true;
                    zoom_factor = FACTEUR_ZOOM_PERTE_VIE+2;
                    if(num_map!=0) {
                        perso->setPosX( blocs[entree-1]->getCoord().x + blocs[entree-1]->getW()/2-perso->getW()/2 );
                        perso->setPosY( blocs[entree-1]->getCoord().y + blocs[entree-1]->getH() );
                    }
                    else {
                        perso->setPos(0,0);
                    }
                }
                else {
                    zoom_factor = FACTEUR_ZOOM_PERTE_VIE;
                    wallpaper=tracerAccueil(polices[1]);
                }
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE: // clique sur la croix rouge
                fin = true; // quitter la boucle
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                switch(ev.keyboard.keycode)
                {      
                    case ALLEGRO_KEY_ESCAPE : // EXIT
                        menu=!menu;
                        break;
                    case ALLEGRO_KEY_ENTER :    // ACCUEIL
                        if(menu) {
                            switch(boutonSelected)
                            {
                                case 0 :
                                    boutonSelected=-1;
                                    menu=false;
                                    break;
                                case 1 :
                                    afficheCommandes();
                                    al_flush_event_queue(event_queue);
                                    // cout << "Controls : \n\tECHAP -> Menu Pause\n\tF -> plein écran\n\tZ/UP/SPACE -> Sauter\n\tQ/Left -> gauche\n\tD/Right -> droite\n\tS/DOWN -> se baisser\n\tC -> saisir objet\n\tB -> boule de feu" << endl;
                                    break;
                                case 2 :
                                    sounds_on=!sounds_on;
                                    playSound=!playSound;
                                    break;
                                case 3 :
                                    fin=true;
                                    while(!al_is_event_queue_empty(event_queue))
                                        al_flush_event_queue(event_queue);
                                    break;
                                default :
                                    break;
                            }
                        }
                        else {
                            if(!enter) {
                                nomUser=saisirUserName();
                                al_set_target_bitmap(wallpaper); // Définit la cible de rendu
                                al_rest(0.2); // Attendez 0,2 secondes
                                if(nomUser.size()>1) {
                                    if(sounds_on) playSound=true;
                                    al_flush_event_queue(event_queue); // enlève évènement lors de la saisie du nom ex: N
                                    enter = true;
                                    dessine=true;
                                    vies=3;
                                    while(!al_is_event_queue_empty(event_queue))
                                        al_flush_event_queue(event_queue);
                                }
                            }
                        }
                        break;
                    case ALLEGRO_KEY_DOWN :    // deplacement bas
                    case KEYBOARD_S :
                        if(menu) {
                            boutonSelected++;
                            if(boutonSelected>=NBR_BOUT)
                                boutonSelected=0;
                            for (i = 0; i < NBR_BOUT; ++i) {
                                if(i==boutonSelected) {
                                    listeBut[i].couleurTxt=JAUNE;
                                    listeBut[i].couleurRect=NOIR;
                                    boutonSelected=i;
                                }
                                else {
                                    listeBut[i].couleurTxt=ROUGE;
                                    listeBut[i].couleurRect=GRIS_TR;
                                }
                            }
                        }
                        else {
                            key[KEY_DOWN]=true;
                            key[KEY_S]=true;
                            stop=true;
                            baisse = true;
                        }
                        break;
                    case ALLEGRO_KEY_UP :    // deplacement haut
                    case KEYBOARD_Z :
                        if(menu) {
                            boutonSelected--;
                            if(boutonSelected<0)
                                boutonSelected=NBR_BOUT-1;
                            for (i = 0; i < NBR_BOUT; ++i) {
                                if(i==boutonSelected) {
                                    listeBut[i].couleurTxt=JAUNE;
                                    listeBut[i].couleurRect=NOIR;
                                    boutonSelected=i;
                                }
                                else {
                                    listeBut[i].couleurTxt=ROUGE;
                                    listeBut[i].couleurRect=GRIS_TR;
                                }
                            }
                        }
                        else {
                            key[KEY_UP]=true;
                            key[KEY_Z]=true;
                            if(perso->getPos().y+perso->getH()>window_height-sol-5) jump=true;
                        }
                        break;
                    case ALLEGRO_KEY_SPACE :
                        if(!menu) {
                            key[KEY_SPACE]=true;
                            if(perso->getPos().y+perso->getH()>window_height-sol-5) jump=true;
                        }
                        break;
                    case ALLEGRO_KEY_RIGHT :    // deplacement droite
                    case KEYBOARD_D :
                        if(!menu) {
                            key[KEY_RIGHT]=true;
                            key[KEY_D]=true;
                            orientation=EST;
                        }
                        break;
                    case ALLEGRO_KEY_LEFT :    // deplacement gauche
                    case KEYBOARD_Q :
                        if(!menu) {
                            key[KEY_LEFT]=true;
                            key[KEY_Q]=true;
                            orientation=OUEST;
                        }
                        break;
                    
                    case KEYBOARD_R :
                        if(enter && !menu) {
                            num_map=0;
                            base_sol=changeMap();
                            temps=0;
                            sol=base_sol+1;
                            perso = new User("stickman");
                            perso->setPos(0,0);
                            perso->setTaille(1);
                            for (int i = 0; i < NB_MAPS; i++)
                            {
                                maps[i]->setBackgroundX(0);
                                maps[i]->setBackgroundScale(1);
                            }
                            
                        }
                        break;

                    case KEYBOARD_F :
                        if(!menu) {
                            fullscreen = !fullscreen;
                            if(fullscreen)
                                al_set_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, true);
                            else
                                al_set_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, false);
                            al_rest(0.5);
                            window_width  = al_get_display_width(al_get_current_display());
                            window_height = al_get_display_height(al_get_current_display());
                            depl = window_height/NBR_BOUT/2-5;
                            RATIO_FRAME=window_height/HEIGHT;
                            if(enter) {
                                base_sol=changeMap();
                                temps=0;
                                sol=base_sol+1;
                                anim_entree=true;
                                if(num_map!=0) {
                                    perso->setPosX( blocs[entree-1]->getCoord().x + blocs[entree-1]->getW()/2-perso->getW()/2 );
                                    perso->setPosY( blocs[entree-1]->getCoord().y + blocs[entree-1]->getH() );
                                }
                                else
                                    perso->setPos(0,0);
                            }
                            else
                                wallpaper=tracerAccueil(polices[1]);
                        }
                        // Dessin des boutons et rectangles
                        for (i = 0; i < NBR_BOUT; ++i) {
                            listeBut[i].posX = window_width/4;
                            listeBut[i].posY = (i+1)*window_height/(NBR_BOUT+2)+(i*20);
                            listeBut[i].w = 2*window_width/4;
                            listeBut[i].h = window_height/(NBR_BOUT+2);
                        }
                        break;

                    case KEYBOARD_N :
                        if(enter && !menu) {
                            num_map = num_map >= NB_MAPS-1 ? 1 : num_map+1;
                            if(num_map>0) {
                                maps[num_map]->setMap0(false);
                                maps[num_map]->setBackgroundScale(2);
                            }
                            base_sol=changeMap();
                            sol=base_sol;
                            perso->setSpeed(0,0);
                        }
                        break;
                    case KEYBOARD_A :
                        if(enter && !menu) playSound = !playSound;
                        break;
                    case KEYBOARD_C :
                        if(enter && !menu)
                            key[KEY_C]=true;
                        break;
                    case KEYBOARD_B :
                        if(perso->getNom().compare("marioFire")==0 && nbrObjets<MAX_OBJETS && enter && !menu) {
                            lanceObjet=true;
                            objets[nbrObjets] = new ObjectLance("boule_feu3");
                            if(sounds_on) son_fireBall->play();
                            objets[nbrObjets]->setAfficher(true);
                            objets[nbrObjets]->setTaille(AGRANDI_FACT/2);
                            objets[nbrObjets]->actualiseSize();
                            objets[nbrObjets]->setPos(perso->getPos().x+2,perso->getPos().y+perso->getH()/2-20);
                            if(perso_num_img==EST || perso_num_img==RUNR || perso_num_img==COUCHER)
                                objets[i]->setSpeedX(10);
                            else if(perso_num_img==OUEST || perso_num_img==RUNL || perso_num_img==COUCHEL)
                                objets[i]->setSpeedX(-10);
                            nbrObjets++;
                        }
                        break;
                    case ALLEGRO_KEY_PAD_0 :
                    case ALLEGRO_KEY_0 :
                        if(enter && !menu) al_convert_mask_to_alpha(maps[num_map]->getImg(), al_get_pixel(maps[num_map]->getImg(),0,0)); // enelve le fond (masque sur le premier pixel)
                        break;
                    case ALLEGRO_KEY_PAD_1 :
                    case ALLEGRO_KEY_1 :
                        if(enter && !menu) masqueRGB(display, maps[num_map]->getImg(), true, false, false); // enleve composante rouge de la map
                        break;
                    case ALLEGRO_KEY_PAD_2 :
                    case ALLEGRO_KEY_2 :
                        if(enter && !menu) masqueRGB(display, maps[num_map]->getImg(), false, true, false); // enleve composante bleue de la map
                        break;
                    case ALLEGRO_KEY_PAD_3 :
                    case ALLEGRO_KEY_3 :
                        if(enter && !menu) masqueRGB(display, maps[num_map]->getImg(), false, false, true); // enleve composante verte de la map
                        break;
                }
                break;

            case ALLEGRO_EVENT_KEY_UP:
                switch(ev.keyboard.keycode)
                {
                    case ALLEGRO_KEY_RIGHT :    // deplacement droite
                    case KEYBOARD_D :
                        key[KEY_RIGHT]=false;
                        key[KEY_D]=false;
                        stop=true;
                        break;
                    case ALLEGRO_KEY_LEFT :    // deplacement gauche
                    case KEYBOARD_Q :
                        key[KEY_LEFT]=false;
                        key[KEY_Q]=false;
                        stop=true;
                        break;
                    case ALLEGRO_KEY_DOWN :    // deplacement bas
                    case KEYBOARD_S :
                        key[KEY_DOWN]=false;
                        key[KEY_S]=false;
                        baisse = false;
                        remonte=true;
                        break;
                    case ALLEGRO_KEY_UP :      // deplacement haut
                    case KEYBOARD_Z :
                        key[KEY_UP]=false;
                        key[KEY_Z]=false;
                        break;
                    case ALLEGRO_KEY_SPACE :
                        key[KEY_SPACE]=false;
                        break;
                    case KEYBOARD_C :
                        key[KEY_C]=false;
                        break;
                }
                break;

            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                if(ev.mouse.button==1) {
                    //fprintf(stdout, "Vous avez cliqué à gauche avec la souris.\n");
                    if(menu) {
                        switch(boutonSelected)
                        {
                            case 0 :
                                boutonSelected=-1;
                                menu=false;
                                break;
                            case 1 :
                                afficheCommandes();
                                al_flush_event_queue(event_queue);
                                // cout << "Controls : \n\tECHAP -> Menu Pause\n\tF -> plein écran\n\tZ/UP/SPACE -> Sauter\n\tQ/Left -> gauche\n\tD/Right -> droite\n\tS/DOWN -> se baisser\n\tC -> saisir objet\n\tB -> boule de feu" << endl;
                                break;
                            case 2 :
                                sounds_on=!sounds_on;
                                playSound=!playSound;
                                break;
                            case 3 :
                                fin=true;
                                break;
                            default :
                                break;
                        }
                    }
                }
                else if(ev.mouse.button==2) {
                    //fprintf(stdout, "Vous avez cliqué à droite avec la souris.\n");
                }
                break;

            case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
                //fprintf(stdout, "Vous avez relâché la souris. Mouse button %d up at (%d, %d)\n", ev.mouse.button, ev.mouse.x, ev.mouse.y);
                break;

            case ALLEGRO_EVENT_MOUSE_AXES:
                al_get_mouse_state(&mouse_state);
                if(menu) {
                    if(abs(x-mouse_state.x)>10 || abs(y-mouse_state.y)>10)
                    {
                        x = mouse_state.x;
                        y = mouse_state.y;
                        for (i = 0; i < NBR_BOUT; ++i) {
                            if(MouseOnButton(x,y,listeBut[i])) {
                                listeBut[i].couleurTxt=JAUNE;
                                listeBut[i].couleurRect=NOIR;
                                boutonSelected=i;
                            }
                            else {
                                listeBut[i].couleurTxt=ROUGE;
                                listeBut[i].couleurRect=GRIS_TR;
                            }
                        }
                    }
                }
                break;

            case ALLEGRO_EVENT_TIMER :

                if(enter && !menu) 
                {
                    if(anim_sortie==false && anim_entree==false && anim_fin==false && anim_perteVie==false) // si pas en anim d'entrée ou de sortie de map
                    {                                                  
                        // Orientation
                        perso_num_img=orientation;
                        if(baisse) {
                            if(orientation==EST)
                                orientation = COUCHER;
                            else if (orientation==OUEST)
                                orientation = COUCHEL;
                        }
                        else {
                            if(orientation==COUCHER)
                                orientation = EST;
                            else if (orientation==COUCHEL)
                                orientation = OUEST;
                        }

                        // Vitesse X
                        if (stop) // continuer le mouvement en x après arret
                        {   
                            if(orientation==EST || orientation==RUNR || orientation==COUCHER || jump==true) {
                                if (perso->getSpeed().x > 0)
                                    perso->setSpeedX(perso->getSpeed().x-FREIN);
                                else {
                                    perso->setSpeedX(0);
                                    stop = false;
                                }
                            }
                            if(orientation==OUEST || orientation==RUNL || orientation==COUCHEL || jump==true) {
                                if (perso->getSpeed().x < 0)
                                    perso->setSpeedX(perso->getSpeed().x+FREIN);
                                else {
                                    perso->setSpeedX(0);
                                    stop = false;
                                }
                            }
                            perso_num_img=orientation;
                            
                        }
                        else // deplacement activé en x
                        {
                            if(key[KEY_RIGHT] or key[KEY_LEFT]) 
                            {
                                if(orientation==EST || orientation==RUNR) 
                                {
                                    perso->setSpeedX(perso->getSpeed().x + ACCELERATION);
                                    if(perso->getSpeed().x > MAX_SPEED)
                                        perso->setSpeedX(MAX_SPEED);
                                    if(img_courir) 
                                        perso_num_img=RUNR;
                                }
                                else if(orientation==OUEST || orientation==RUNL ) 
                                {
                                    perso->setSpeedX(perso->getSpeed().x - ACCELERATION);
                                    if(perso->getSpeed().x < -MAX_SPEED)
                                        perso->setSpeedX(-MAX_SPEED);
                                    if(img_courir) 
                                        perso_num_img=RUNL;
                                }
                                if(cmptFrames%5==0) 
                                    img_courir = !img_courir;
                            }
                        }

                        // animation avec mechant (ecrasement + lance de coquille)
                        for(i=0; i<nbrMechants; i++) 
                        {
                            if(mechants[i]->getEcrase())  // si mechant écrasé
                            {
                                if(mechants[i]->getNom().compare("koopa")!=0 && cmptFrames%9==0) // si c pas koopa
                                { 
                                    mechants[i]->setAfficher(false);
                                    mechants[i]->setPos(-50,-50);
                                    mechants[i]->setEcrase(false);
                                }
                                else if(mechants[i]->getNom().compare("koopa")==0) // si c kooopa
                                { 
                                    mechants[i]->setTaille(0.06);
                                    mechants[i]->actualiseSize(2);
                                    if(grabObject) // si objet saisie (carapace)
                                    {
                                        if(perso_num_img==EST || perso_num_img==RUNR) {
                                            if(key[KEY_C])
                                                mechants[i]->setPos(perso->getPos().x+perso->getW()/2,perso->getPos().y+perso->getH()/2);
                                            else {
                                                mechants[i]->setSpeedX(4);
                                                mechants[i]->setSpeedY(GRAVITY);
                                                mechants[i]->setPos(perso->getPos().x+perso->getW()+1,perso->getPos().y+GRAVITY);
                                                grabObject=false;
                                            }
                                        }
                                        else if(perso_num_img==OUEST || perso_num_img==RUNL) {
                                            if(key[KEY_C])
                                                mechants[i]->setPos(perso->getPos().x-mechants[i]->getW()/2,perso->getPos().y+perso->getH()/2);
                                            else {
                                                mechants[i]->setSpeedX(-4);
                                                mechants[i]->setSpeedY(GRAVITY);
                                                mechants[i]->setPos(perso->getPos().x-mechants[i]->getW()-1,perso->getPos().y+GRAVITY);
                                                grabObject=false;
                                            }
                                        }
                                    }
                                }
                                mechants[i]->actualisePos();
                            }
                        }

                        // Vitesse Y
                        perso->setSpeedY(perso->getSpeed().y + GRAVITY);
                        sol=base_sol+1;
                        
                        handleCollisions(); // Gestion des collisions
                        if(nbrObjets==0) lanceObjet=false;

                        if(jump) {
                            if(key[KEY_SPACE])
                                perso->setSpeedY( perso->getSpeed().y + JUMP_FORCE - 2);
                            else
                                perso->setSpeedY( perso->getSpeed().y + JUMP_FORCE );
                            jump = false;
                            if(sounds_on) son_jump->play();
                        }
                                  
                        // rétrécissement après collision avec un méchant
                        if (retreci) {
                            if(cmptRetreci%2==0 || cmptRetreci%3==0)
                                perso->setAfficher(false);
                            else
                                perso->setAfficher(true);
                            if(cmptRetreci > 100) {
                                retreci=false;
                                cmptRetreci=0;
                            }
                            cmptRetreci++;
                        }

                        // ajuste position si baisse
                        if(baisse)
                            perso->setPosY(perso->getPos().y + abs(perso->getH()-al_get_bitmap_height(perso->getImg(COUCHEL))*AGRANDI_FACT));
                        else if(remonte) {
                            perso->setPosY(perso->getPos().y - abs(perso->getH()-al_get_bitmap_height(perso->getImg(OUEST))*AGRANDI_FACT));
                            remonte=false;
                        }
                    } // fin conditions anim
                    
                    // acualise coord
                    perso->actualisePos();

                    // si deplacement, bouge les blocs et map
                    if((key[KEY_RIGHT] or key[KEY_LEFT]) and num_map>0) 
                    {
                        if(maps[num_map]->getBackgroundX() - perso->getSpeed().x * 2 > 0) // limite bord gauche
                            maps[num_map]->setBackgroundX(0);
                        else if(maps[num_map]->getBackgroundX() - perso->getSpeed().x * 2 - window_width < -maps[num_map]->getW() * maps[num_map]->getBackgroundScale()) // limite bord droit
                            maps[num_map]->setBackgroundX(maps[num_map]->getBackgroundX());
                        else {
                            int bloc_fin = sortie;
                            if(blocs[sortie-1]->getType()==TUYAU or blocs[sortie-1]->getType()==CHATEAU)
                                bloc_fin = sortie - 1;
                            if(blocs[entree]->getCoord().x < 10 && blocs[bloc_fin]->getCoord().x+blocs[bloc_fin]->getW() > window_width) {
                                maps[num_map]->setBackgroundX( maps[num_map]->getBackgroundX() - perso->getSpeed().x * 3 );
                                for (i = 0; i < nbrBlocs; i++) // deplace les blocs
                                {
                                    POS newcoord;
                                    newcoord.x = blocsCopy[i]->getCoord().x - perso->getSpeed().x * 2 / 2;
                                    newcoord.y = blocsCopy[i]->getCoord().y;
                                    blocs[i]->setCoord(newcoord);
                                }
                            }
                        }
                    }

                    // Limites de bords
                    if (perso->getPos().x < 0) {
                        if(entree!=0 || num_map==0)
                            perso->setPosX(0);
                        else {
                            perso->setPosX(window_width-perso->getW()-1);
                            num_map = num_map <= 0 ? NB_MAPS-1 : num_map-1;
                            base_sol=changeMap();
                        }
                    }
                    else if (perso->getPos().x > window_width-perso->getW()) {
                        if(sortie!=0)
                            perso->setPosX(window_width-perso->getW());
                        else {
                            perso->setPosX(0);
                            num_map = num_map >= NB_MAPS-1 ? 1 : num_map+1;
                            base_sol=changeMap();
                        }
                    }
                    if (perso->getPos().y > window_height) { // si tombe dans un trou
                        anim_perteVie=true;
                        if(sounds_on) son_over->play();
                    }

            // ------------------------------- ANIMATIONS -------------------------------------------
                    //  sortie
                    if(anim_fin) { // fin de niveau via porte
                        perso->setSpeedY(0);
                        perso->setPosY(window_height-sol-perso->getH());
                        if(perso->getPos().x+perso->getW()/2>=blocs[sortie]->getCoord().x+blocs[sortie]->getW()/2) { // si perso au nvx de la porte
                            perso->setSpeedX(0);
                            cmptSortie++;
                        }
                        else
                            perso->setSpeedX(2); // avance jusqu a la porte
                        if(blocs[sortie]->getType()==CHATEAU) {
                            music->setGain(0.2);
                            if(sounds_on) son_finish->play();
                        }
                        if(cmptSortie>0) {
                            perso_num_img=NORD;
                        }
                        if(cmptSortie>20) {
                            if(num_map>=NB_MAPS-1) {
                                finish();
                                anim_fin=false;
                                anim_entree=false;
                                cmptSortie=0;
                            }
                            else {
                                num_map++;
                                base_sol=changeMap();
                                perso = new User(perso->getNom());
                                perso->setPos(blocs[entree-1]->getCoord().x+blocs[entree-1]->getW()/2-perso->getW()/2 , blocs[entree-1]->getCoord().y);
                                anim_fin=false;
                                anim_entree=true;
                                cmptSortie=0;
                            }
                        }
                        else if(cmptSortie>15)
                            masqueRGB(display, perso->getImg(perso_num_img), true, true, true);
                        else if(cmptSortie>10)
                            masqueRGB(display, perso->getImg(perso_num_img), true, true, false);
                        else if(cmptSortie>5)
                            masqueRGB(display, perso->getImg(perso_num_img), true, false, false);
                    }
                    else if(anim_sortie) { // fin de map via tuyau
                        anim_tuyau++;
                        music->setGain(0.4);
                        if(anim_tuyau==1 && sounds_on) {
                            son_tuyau->play();
                        }
                        else {
                            if(blocs[sortie]->getAngle()==GAUCHE) {  // sortie sur le cote du tuyau
                                perso->setSpeed(1,0);
                                perso->setPosY(blocs[sortie]->getCoord().y+(blocs[sortie]->getH()-perso->getH())/2);
                                if(blocs[sortie]->getCoord().x<perso->getPos().x) {
                                    anim_tuyau=0;
                                    num_map = num_map >= NB_MAPS-1 ? 1 : num_map+1;
                                    base_sol=changeMap();
                                    perso->setPosX(blocs[entree-1]->getCoord().x+blocs[entree-1]->getW()/2-perso->getW()/2);
                                    perso->setPosY(blocs[entree-1]->getCoord().y-10);
                                    anim_entree=true;
                                    anim_sortie=false;
                                }
                            }
                            else if(blocs[sortie]->getAngle()==ZERO) {  // sortie vers le bas du tuyau
                                perso->setSpeed(0,1);
                                perso_num_img=SUD;
                                perso->setPosX(blocs[sortie]->getCoord().x+(blocs[sortie]->getW()-perso->getW())/2);
                                if(blocs[sortie]->getCoord().y<perso->getPos().y) {
                                    anim_tuyau=0;
                                    num_map = num_map >= NB_MAPS-1 ? 1 : num_map+1;
                                    base_sol=changeMap();
                                    perso->setPosX(blocs[entree-1]->getCoord().x+blocs[entree-1]->getW()/2-perso->getW()/2);
                                    perso->setPosY(blocs[entree-1]->getCoord().y);
                                    anim_entree=true;
                                    anim_sortie=false;
                                }
                            }
                        }
                    }
                    else if (anim_entree) { // entree via tuyau
                        anim_tuyau++;
                        music->setGain(0.4);
                        if(anim_tuyau==1 && sounds_on) {
                            son_tuyau->play();
                            // perso->setPosY(blocs[entree]->getCoord().y+10);
                        }
                        else {
                            if(blocs[entree]->getAngle()==ZERO) {  // entree vers le haut tuyau
                                perso->setSpeed(0,-1);
                                perso->setPosX( blocs[entree]->getCoord().x+(blocs[entree]->getW()-perso->getW())/2 );
                                perso->setPosY( perso->getPos().y + perso->getSpeed().y );
                                if(blocs[entree]->getCoord().y>perso->getPos().y+perso->getH()+1) {
                                    anim_tuyau=0;
                                    anim_entree=false;
                                }
                            }
                            else if(blocs[entree]->getAngle()==INVERSION) {  // entree vers le bas tuyau
                                perso->setSpeed(0,1);
                                perso->setPosX(blocs[entree]->getCoord().x+(blocs[entree]->getW()-perso->getW())/2);
                                perso->setPosY(perso->getPos().y + perso->getSpeed().y);
                                if(blocs[entree]->getCoord().y+blocs[entree]->getH()+1<perso->getPos().y) {
                                    anim_tuyau=0;
                                    anim_entree=false;
                                }
                            }
                        }
                    }
                    else if(anim_perteVie) {
                        cmptVie++;
                        zoom_factor-=0.05;
                        if(zoom_factor <= 1) {
                            cmptVie=0;
                            zoom_factor=FACTEUR_ZOOM_PERTE_VIE;
                            anim_perteVie=false;
                            restart();
                        }
                    }

                    cmptFrames++;
                    if(cmptFrames>=1000) cmptFrames=0;
                    if(cmptFrames%FRAME_RATE==0 && num_map>0) temps++;
                    if(cmptFrames%33==0) perso->setMessage("");

                    dessine=true;
                }
                else if(!enter && !menu) {
                    textX -= TEXT_SPEED;
                    if (textX < -al_get_text_width(polices[1], txt.c_str()))
                        textX = window_width+textX;  // Réinitialisation à droite une fois que le texte a dépassé l'écran
                    dessine=true;
                }
                else if(menu)
                    dessine=true;
                break;

            default:
                break;
        }

        if(dessine==true && al_is_event_queue_empty(event_queue)){

            // clear
            al_clear_to_color(NOIR);

            if(enter) {
                // actual coord
                perso->setTaille(AGRANDI_FACT);
                perso->actualiseSize(perso_num_img);

                // draw wallpaper
                maps[num_map]->draw(window_width, window_height);
                
                // draw perso & blocs
                for (i = 0; i < nbrBlocs; ++i) {
                    if(blocs[i]->getSortieObjet() && blocs[i]->isObject() && !blocs[i]->isHiding() && blocs[i]->is_enable() &&  !blocs[i+1]->isObject() && !blocs[i+1]->isHiding() && blocs[i+1]->getType()==BLOC_VIDE) 
                    {
                        blocs[i]->setCoord(blocs[i]->getCoord().x , blocs[i]->getCoord().y-VITY_BONUS);
                        if(blocs[i]->getCoord().y+blocs[i]->getH()+2<blocs[i+1]->getCoord().y)
                            blocs[i]->setSortieObjet(false);
                    }
                }
                
                if(anim_fin) 
                {
                    for (i = 0; i < nbrBlocs; ++i) blocs[i]->draw(); // draw blocs
                    perso->draw(perso_num_img); // draw perso
                }
                else 
                {
                    if(anim_tuyau>0) { // si sortie par le tuyau
                        perso->draw(perso_num_img); // draw perso
                        for (i = 0; i < nbrBlocs; ++i) blocs[i]->draw(); // draw blocs
                    }
                    else {
                        for (i = 0; i < nbrBlocs; ++i) blocs[i]->draw(); // draw blocs
                        perso->draw(perso_num_img); // draw perso
                    }
                }

                // Draw mechants
                for(i=0;i<nbrMechants;i++)
                    mechants[i]->draw();

                // Afficher message +1 ou Zup
                perso->afficherMessage(polices[3]);

                // Draw objets (boules de feu, boomerang) et explosion
                for ( i = 0; i < MAX_OBJETS; ++i) {
                    if(objetsCol[i]==true) {
                        cmptColObjet++;
                        if(cmptColObjet<10)
                            al_draw_scaled_bitmap(explose, 0, 0, al_get_bitmap_width(explose), al_get_bitmap_height(explose), coordCol.x, coordCol.y, al_get_bitmap_width(explose)/20, al_get_bitmap_height(explose)/20, 0);
                        else {
                            objetsCol[i]=false;
                            cmptColObjet=0;
                            coordCol = (POS) { 0,0 };
                        }
                    }
                }
                for ( i = 0; i < nbrObjets; ++i) {
                    objets[i]->draw();
                }

                // Joue musique
                if(playSound)
                    music->play();
                else
                    music->stop();

                // Dessine l'image perte de vie
                if(anim_perteVie) {
                    // Définir les coordonnées du rectangle source de l'image d'origine
                    int source_x = 0;
                    int source_y = 0;
                    int source_width  = al_get_bitmap_width(img_fin);
                    int source_height = al_get_bitmap_height(img_fin);

                    // Calculez les dimensions du rectangle de destination pour le zoom
                    int dest_width  = window_width  * zoom_factor;
                    int dest_height = window_height * zoom_factor;

                    // Calculez les coordonnées du point central
                    int center_x = dest_width  / 2;
                    int center_y = dest_height / 2;
                    int dest_x = window_width  / 2 - center_x;
                    int dest_y = window_height / 2 - center_y;

                    // Dessinez l'image zoomée
                    al_draw_scaled_bitmap(img_fin, source_x, source_y, source_width, source_height, dest_x, dest_y, dest_width, dest_height, 0);
                    if(vies>1)
                        al_draw_text(polices[1], ROUGE, window_width/2, HAUTEUR_TEXTE+20, ALLEGRO_ALIGN_CENTER, "Vies -1");
                    else
                        al_draw_text(polices[1], ROUGE, window_width/2, HAUTEUR_TEXTE+20, ALLEGRO_ALIGN_CENTER, "PERDU");
                }

                // Draw text, vies, score, temps, nomUser
                afficherTexte(polices[3]);
                if(blocs[sortie]->getType()==CHATEAU) {
                    int xaffich, yaffich;
                    xaffich=blocs[sortie]->getCoord().x+blocs[sortie]->getW()/2;
                    yaffich=blocs[sortie]->getCoord().y+25;
                    al_draw_filled_rectangle(xaffich-25, yaffich, xaffich+25, yaffich+20, BLANC);
                    al_draw_text(polices[3], ROUGE, xaffich, yaffich, ALLEGRO_ALIGN_CENTER, "EXIT");
                }
                
            }
            else {
                wallpaper=tracerAccueil(polices[1]);
            }
            
            // Menu pause
            if(menu)
            {
                // Dessiner le rectangle de flou
                al_draw_filled_rectangle(0, 0, window_width, window_height, al_map_rgba(0, 0, 0, 222));

                // Dessin de MENU
                al_draw_text(polices[1], ROUGE, window_width/2, window_height/5/2, ALLEGRO_ALIGN_CENTER, "MENU");
                
                // Dessin des boutons et rectangles
                for (i = 0; i < NBR_BOUT; ++i) {
                    al_draw_filled_rectangle(listeBut[i].posX, listeBut[i].posY, listeBut[i].posX+listeBut[i].w, listeBut[i].posY+listeBut[i].h, listeBut[i].couleurRect);
                    if(listeBut[i].nom.compare("SONS")==0) {
                        if(sounds_on)
                            al_draw_text(listeBut[i].font, listeBut[i].couleurTxt, listeBut[i].posX+listeBut[i].w/2, listeBut[i].posY+listeBut[i].h/2-18, ALLEGRO_ALIGN_CENTER, (listeBut[i].nom+": ON").c_str());
                        else
                            al_draw_text(listeBut[i].font, listeBut[i].couleurTxt, listeBut[i].posX+listeBut[i].w/2, listeBut[i].posY+listeBut[i].h/2-18, ALLEGRO_ALIGN_CENTER, (listeBut[i].nom+": OFF").c_str());
                    }
                    else
                        al_draw_text(listeBut[i].font, listeBut[i].couleurTxt, listeBut[i].posX+listeBut[i].w/2, listeBut[i].posY+listeBut[i].h/2-18, ALLEGRO_ALIGN_CENTER, listeBut[i].nom.c_str());
                }
            }

            // actualise affichage
            al_flip_display();

            dessine=false;
        }
    }
    
    // Destruction des ressources
    // detruitRessources(wallpaper, timer, event_queue, display);
    al_destroy_bitmap(wallpaper);
    al_destroy_timer(timer);
    detruit_polices();
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);
    al_destroy_voice(voice);
    al_destroy_mixer(mixer);
    al_uninstall_audio();

    return 0;
}