#include "../include/game.hpp"

//////////////////   SPECIAUX   ///////////////////////
Game::Game(ALLEGRO_DISPLAY *display) : display(display) {

    // Joueur
    nomUser = "_";
    score = 0;
    temps = Duree(0,0,0);
    vies = 3;

    // Affichage
    window_height = 720;
    window_width  = 1280;
    window_x = 0;
    window_y = 0;
    RATIO_FRAME = window_height/HEIGHT;

    // Chargement polices
    charge_polices();

    // Jeu
    perso = new User("_");
    gameOver = false;
    level = 0;
    fullscreen = false;
    playsound  = false;
    gravity = 2;
    jump_force = 25;
    maps.clear();
    blocs.clear();
    mechants.clear();
    objets.clear();
    move["baisser"]=false;
    move["droite"]=false;
    move["gauche"]=false;
    move["sauter"]=false;
    move["baisse"]=false;

    // Repères
    base_sol = nbrBlocsSol = entree = sortie = num_map = perso_num_img = cmptFrames = souris.x = souris.y = 0;

    // Accueil
    msg_accueil = "Bienvenu dans Mario Bros. !! Appuyez sur ENTRER pour JOUER";
    posHidePart = al_get_text_width(polices[1], msg_accueil.c_str());
    posPrintPart= window_width/2 - posHidePart/2;

    // Moments du jeu
    started = false;
    menuSelected = false;

    // Animations
    anim_fin      = false;
    anim_tuyau    = false;
    anim_entree   = false;
    anim_sortie   = false;
    anim_perteVie = false;

    // Charge l'audio
    voice = al_create_voice(FREQ_ECHANTILLONAGE, ALLEGRO_AUDIO_DEPTH_INT16, ALLEGRO_CHANNEL_CONF_2);
    if (!voice)
        erreur("Initialise voice");
    mixer = al_create_mixer(FREQ_ECHANTILLONAGE, ALLEGRO_AUDIO_DEPTH_FLOAT32,ALLEGRO_CHANNEL_CONF_2);
    al_attach_mixer_to_voice(mixer, voice);

    // Charge les musiques -> Sound(const char* newChemin, int playmode, float gain, float vitesse, ALLEGRO_MIXER *mixer)
    mesSons["music"]           = new Sound("datas/musiques/new super mario bros (world 1-1).wav", LOOP, 0.5, 1.0, mixer);
    mesSons["son_finish"]      = new Sound("datas/musiques/Super Mario Bros Music-Level Complete.wav", ONCE, 1.0, 1.2, mixer);
    mesSons["son_jump "]       = new Sound("datas/musiques/ClassicSmallJump.wav", ONCE, 1.0, 1.0, mixer);
    mesSons["son_tuyau"]       = new Sound("datas/musiques/MultiplayerTeamFinish.wav", ONCE, 1.0, 1.0, mixer);
    mesSons["son_mario"]       = new Sound("datas/musiques/Mario_sounds/Sample_0721-mario number one wouhou.wav", ONCE, 1.0, 1.0, mixer);
    mesSons["son_luigi"]       = new Sound("datas/musiques/luigi_okey-dokey.wav", ONCE, 1.0, 1.0, mixer);
    mesSons["son_coin "]       = new Sound("datas/musiques/RedCoin.wav", ONCE, 1.0, 1.0, mixer);
    mesSons["son_sol"]         = new Sound("datas/musiques/FootStepBlock2.wav", ONCE, 2.0, 1.0, mixer);
    mesSons["son_over"]        = new Sound("datas/musiques/game_over.wav", ONCE, 1.0, 1.0, mixer);
    mesSons["son_powerUp"]     = new Sound("datas/musiques/power_up.wav", ONCE, 1.0, 1.0, mixer);
    mesSons["son_powerDown"]   = new Sound("datas/musiques/Power_Down_sound.wav", ONCE, 1.0, 1.0, mixer);
    mesSons["son_fireBall"]    = new Sound("datas/musiques/FireBall.wav", ONCE, 1.0, 1.0, mixer);
    mesSons["son_fireBallHit"] = new Sound("datas/musiques/FireBallHit1.wav", ONCE, 1.0, 1.0, mixer);
    mesSons["son_ecrase"]      = new Sound("datas/musiques/Goomba_Stomp_Sound.wav", ONCE, 2.0, 1.0, mixer);
    mesSons["son_koopa_shell"] = new Sound("datas/musiques/Koopa_Troopa_Kick_Shell.wav", ONCE, 0.5, 1.0, mixer);

    sounds_on = false; // pas de sons au début

    load_maps(); // charge les maps

    // Initialisation des boutons
    string nom_but[NBR_BOUT] = { "CONTINUER" , "COMMANDES" , "SONS" , "QUITTER" };
    int posX, posY, w, h;
    posX = window_width/NBR_BOUT;
    w = 2*WIDTH/NBR_BOUT;
    h = HEIGHT/(NBR_BOUT+2);
    for (int i = 0; i < NBR_BOUT; i++)
    {
        posY = 2*window_height/10 + i*window_height/(NBR_BOUT+2) + i*20;
        bouton nvBut = { nom_but[i] , posX , posY , w , h , polices[1] , ROUGE , GRIS_TR };
        listeBut.push_back(nvBut);
    }
    boutonSelected = -1;

    // Création de la fenêtre d'accueil
    posHidePart = al_get_text_width(polices[1], msg_accueil.c_str());  // Position de la partie cachée du texte
    posPrintPart = window_width/2 - posHidePart/2; // Position de la partie affichée du texte
}
Game::~Game() {
    try 
    {
        // Jeu
        maps.clear();
        blocs.clear();
        mechants.clear();
        objets.clear();
        move.clear();

		map<string,Sound*>::iterator it;  //Un itérateur
        // for (const auto& pair : mesSons)
        for (it = mesSons.begin(); it!=mesSons.end(); ++it) 
            delete(it->second); // Libération de chaque instance de Sound
        mesSons.clear();

        // Affichage
        detruit_polices();

        if(wallpaper)
            al_destroy_bitmap(wallpaper);
        if(mixer)
            al_destroy_mixer(mixer);
        if(voice)
            al_destroy_voice(voice);
        
        delete(perso);

    }
    catch(exception const& e)	 //On rattrape les exceptions standard de tous types
    {
        cerr << "ERREUR : " << e.what() << endl; //On affiche la description de l'erreur
    }

}

//////////////////   GETTERS   ///////////////////////
bool Game::isGameOver() {
    return gameOver;
}
string Game::getNomUser() {
    return nomUser;
}
bool Game::getStarted() {
    return started;
}
bool Game::getMenuSelected() {
    return menuSelected;
}
POS Game::getMousePos() {
    return souris;
}

//////////////////   SETTERS   ///////////////////////
void Game::setPerso(string nom) {
    perso = new User(nom.c_str());
}
void Game::setNomUser(string nom) {
    nomUser = nom;
}
void Game::setWindowPOS(int windowX,int windowY, ALLEGRO_DISPLAY *display) {
    window_x = windowX;
    window_y = windowY;
    al_set_window_position(display, window_x, window_y);
}
void Game::setWindowX(int windowX, ALLEGRO_DISPLAY *display) {
    window_x = windowX;
    al_set_window_position(display, window_x, window_y);
}
void Game::setWindowY(int windowY, ALLEGRO_DISPLAY *display) {
    window_y = windowY;
    al_set_window_position(display, window_x, window_y);
}
void Game::setGameOver(bool isOver) {
    gameOver = isOver;
}
void Game::setMenuSelected(bool menu) {
    menuSelected = menu;
}
void Game::setMousePos(int x, int y) {
    souris.x = x;
    souris.y = y;
}

/////////////   METHODES AUTRES   ////////////////////
void Game::erreur(const char* txt)
{
    ALLEGRO_DISPLAY* d = al_is_system_installed() ? al_get_current_display() : NULL;
    const char* button = "Quitter";
    char titre[50];
    sprintf(titre,"Erreur : %s", txt);
    int result = al_show_native_message_box(d, titre, "Ne fonctionne pas", "Cliquer pour quitter, régler le problème puis recompiler !", button, ALLEGRO_MESSAGEBOX_ERROR); // ALLEGRO_MESSAGEBOX_YES_NO, ALLEGRO_MESSAGEBOX_WARN, ALLEGRO_MESSAGEBOX_ERROR, ALLEGRO_MESSAGEBOX_QUESTION et ALLEGRO_MESSAGEBOX_OK_CANCEL

    if (result == 1)
        gameOver=true;
}
void Game::charge_polices() {
    int i=0;
    ALLEGRO_FONT* font=NULL;
    string dossier="datas/polices/";
    string nom;

    nom="Arial.ttf"; font = al_load_font((dossier+nom).c_str(), 36, 0);             if(font) { polices[i]=font; i++;font=NULL; }
    nom="Arial Bold.ttf"; font = al_load_font((dossier+nom).c_str(), 36, 0);        if(font) { polices[i]=font; i++;font=NULL; }
    nom="MarioAndLuigi.ttf"; font = al_load_font((dossier+nom).c_str(), 40, 0);     if(font) { polices[i]=font; i++;font=NULL; }
    nom="Arial Bold.ttf"; font = al_load_font((dossier+nom).c_str(), 20, 0);        if(font) { polices[i]=font; i++;font=NULL; }
    nom="Arial Bold.ttf"; font = al_load_font((dossier+nom).c_str(), 15, 0);        if(font) { polices[i]=font; i++;font=NULL; }
}
void Game::detruit_polices() {
    for (int i = 0; i < NBR_FONTS; ++i)
        if(polices[i]) al_destroy_font(polices[i]);
}

void Game::begin(ALLEGRO_EVENT_QUEUE *event_queue) {
    do {
        nomUser = saisirUserName();
    } while (nomUser.size()<2);
    while(!al_is_event_queue_empty(event_queue))
        al_flush_event_queue(event_queue);
    changeMap();
    started = true;
}
string Game::saisirUserName(void)
{
    ALLEGRO_DISPLAY *d = NULL;
    ALLEGRO_EVENT_QUEUE *evt_queue = NULL;
    ALLEGRO_FONT *font = NULL;
    int largeur=400, hauteur=200;

    al_set_new_display_flags(ALLEGRO_WINDOWED);
    d = al_create_display(largeur,hauteur);
    if (!d) {
        fprintf(stderr, "Erreur lors de la création de la fenêtre.\n");
        return "erreur";
    }
    al_set_window_title(d, "PSEUDO");

    font = al_load_ttf_font("datas/polices/Arial Bold.ttf", 25, 0);
    if (!font) {
        fprintf(stderr, "Erreur lors du chargement de la police de caracteres.\n");
        return "erreur";
    }

    evt_queue = al_create_event_queue();
    if (!evt_queue) {
        fprintf(stderr, "Erreur lors de la création de la file d'evenements.\n");
        return "erreur";
    }

    al_register_event_source(evt_queue, al_get_display_event_source(d));
    al_register_event_source(evt_queue, al_get_keyboard_event_source());

    ALLEGRO_EVENT evt;
    string nom="_";
    
    al_set_target_backbuffer(d);

    while (1) {
        al_clear_to_color(NOIR);

        al_draw_text(font, ROUGE, largeur/2, hauteur/4, ALLEGRO_ALIGN_CENTER, "PSEUDO");
        al_draw_filled_rectangle(50,3*hauteur/4-30,largeur-50,3*hauteur/4+30, GRIS);
        al_draw_text(font, BLANC, largeur/2, 3*hauteur/4-12, ALLEGRO_ALIGN_CENTER, nom.c_str());

        al_flip_display();

        al_wait_for_event(evt_queue, &evt);

        if (evt.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        } 
        else if (evt.type == ALLEGRO_EVENT_KEY_CHAR) {
            if (evt.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                nom.pop_back();
                break;
            } 
            else if (evt.keyboard.keycode == ALLEGRO_KEY_BACKSPACE) {
                if (nom.size() > 0) {
                    nom.pop_back();
                    nom.pop_back();
                    nom+="_";
                }
            } 
            else {
                if(nom.size()>0)
                    nom.pop_back();
                if (nom.size() < sizeof(nom) - 1 && nom.size()<20) {
                    char c[2] = {(char)evt.keyboard.unichar, '\0'};
                    nom+=c;
                    nom+="_";
                }
            }
        }
    }
    while(!al_is_event_queue_empty(evt_queue))
        al_flush_event_queue(evt_queue);
    al_destroy_event_queue(evt_queue);
    al_destroy_font(font);
    al_destroy_display(d);

    al_set_target_backbuffer(display);

    return nom;
}

void Game::tracerAccueil() {
    ALLEGRO_FONT* font = polices[1];
    wallpaper = al_load_bitmap(WALLPAPER);
    if (!wallpaper)
        erreur("initialise image d'accueil");
    
    assert(wallpaper != 0) ;

    logo = al_load_bitmap(LOGO);
    if (!logo)
        erreur("initialise logo d'accueil");

    assert(logo != 0) ;

    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_scaled_bitmap(wallpaper, 0, 0, al_get_bitmap_width(wallpaper), al_get_bitmap_height(wallpaper), 0, 0, window_width, window_height, 0);
    int facteur = 4;
    al_draw_scaled_bitmap(logo, 0, 0, al_get_bitmap_width(logo), al_get_bitmap_height(logo), window_width/2-al_get_bitmap_width(logo)/facteur/2, 3*window_height/4-al_get_bitmap_height(logo)/facteur/2, al_get_bitmap_width(logo)/facteur, al_get_bitmap_height(logo)/facteur, 0);
    int posY = window_height-30;
    al_draw_filled_rectangle(0,posY-30,window_width,posY+30,al_map_rgba(0, 0, 0, 222));
    al_draw_text(font, ORANGE, posPrintPart, posY-20, ALLEGRO_ALIGN_LEFT, msg_accueil.c_str());
    if(posPrintPart<0)
        al_draw_text(font, ORANGE, window_width+posPrintPart, posY-20, ALLEGRO_ALIGN_LEFT, msg_accueil.c_str());
}
void Game::dessine() {
    maps[num_map].draw(window_width, window_height);
    afficherTexte();
    perso->draw(perso_num_img); // draw perso
    for (int i = 0; i < blocs.size(); ++i) 
        blocs[i].draw(); // draw blocs
}
void Game::dessineMenu() {
    // Dessiner le rectangle de flou
    al_draw_filled_rectangle(0, 0, window_width, window_height, al_map_rgba(0, 0, 0, 222));

    // Dessin de MENU
    al_draw_text(polices[1], ROUGE, window_width/2, window_height/5/2, ALLEGRO_ALIGN_CENTER, "MENU");
    
    // Dessin des boutons et rectangles
    for (int i = 0; i < NBR_BOUT; ++i) {
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
void Game::afficherTexte() {
    ALLEGRO_FONT *font = polices[3];
    int decaleBords = 40;
    ALLEGRO_BITMAP* coeur = al_load_bitmap("datas/images/coeur.png");
    if (!coeur)
        erreur("initialise image coeur");

    ALLEGRO_BITMAP* horloge = al_load_bitmap("datas/images/horloge.png");
    if (!horloge)
        erreur("initialise image horloge");

    ALLEGRO_BITMAP* coin = al_load_bitmap("datas/images/coin2.png");
    if (!coin)
        erreur("initialise image coin");

    al_draw_filled_rectangle(0,0,window_width,HAUTEUR_TEXTE, NOIR);
    
    level=num_map;

    string msgNom = "User : " + nomUser;
    al_draw_text(font, ORANGE, decaleBords, HAUTEUR_TEXTE/2-al_get_font_line_height(font)/2, ALLEGRO_ALIGN_LEFT, msgNom.c_str());

    string msgVies = "x " + to_string(vies);
    al_draw_scaled_bitmap(coeur, 0, 0, al_get_bitmap_width(coeur), al_get_bitmap_height(coeur), window_width/4-30, HAUTEUR_TEXTE/2-al_get_font_line_height(font)/2, 25, 25, 0);
    al_draw_text(font, ORANGE, window_width/4, 20, ALLEGRO_ALIGN_LEFT, msgVies.c_str());

    string msgTemps = to_string(temps.getS()) + "s";
    if(temps.getM()!=0)
        msgTemps = to_string(temps.getM()) + "m " + msgTemps;
    if(temps.getH()!=0)
        msgTemps = to_string(temps.getH()) + "h " + msgTemps;
    
    int longTxt = al_get_text_width(font, msgTemps.c_str());
    al_draw_scaled_bitmap(horloge, 0, 0, al_get_bitmap_width(horloge), al_get_bitmap_height(horloge), window_width/2-longTxt/2-35, HAUTEUR_TEXTE/2-al_get_font_line_height(font)/2, 25, 25, 0);
    al_draw_text(font, ORANGE, window_width/2, HAUTEUR_TEXTE/2-al_get_font_line_height(font)/2, ALLEGRO_ALIGN_CENTER, msgTemps.c_str());

    string msgLevel = "Level : " + to_string(level);
    al_draw_text(font, ORANGE, 3*window_width/4, HAUTEUR_TEXTE/2-al_get_font_line_height(font)/2, ALLEGRO_ALIGN_RIGHT, msgLevel.c_str());

    string msgScore =  "x " + to_string(score);
    longTxt = al_get_text_width(font, msgScore.c_str());
    al_draw_scaled_bitmap(coin, 0, 0, al_get_bitmap_width(coin), al_get_bitmap_height(coin), window_width-decaleBords-longTxt-35, HAUTEUR_TEXTE/2-al_get_font_line_height(font)/2 , 25, 25, 0);
    al_draw_text(font, ORANGE, window_width-decaleBords, HAUTEUR_TEXTE/2-al_get_font_line_height(font)/2, ALLEGRO_ALIGN_RIGHT, msgScore.c_str());
}

void Game::handleKeyPressed(int keycode, ALLEGRO_EVENT_QUEUE *event_queue) {
    switch(keycode)
    {      
        case ALLEGRO_KEY_ESCAPE : // EXIT
            menuSelected=!menuSelected;
            break;
        case ALLEGRO_KEY_ENTER :    // ACCUEIL
            if(menuSelected) {
                actionOnMenu();
                while(!al_is_event_queue_empty(event_queue))
                    al_flush_event_queue(event_queue);
            }
            else if(!started)
                begin(event_queue);
            break;
        case ALLEGRO_KEY_DOWN :    // deplacement bas
        case KEYBOARD_S :
            move["baisse"]=true;
            if(menuSelected) {
                if(boutonSelected>=-1) {
                    listeBut[boutonSelected].couleurTxt=ROUGE;
                    listeBut[boutonSelected].couleurRect=GRIS_TR;
                }
                boutonSelected++;
                if(boutonSelected>=NBR_BOUT)
                    boutonSelected=0;
                listeBut[boutonSelected].couleurTxt=JAUNE;
                listeBut[boutonSelected].couleurRect=NOIR;
            }
            break;
        case ALLEGRO_KEY_UP :    // deplacement haut
        case KEYBOARD_Z :
        case ALLEGRO_KEY_SPACE :
            move["sauter"]=true;
            if(menuSelected) {
                if(boutonSelected!=-1) {
                    listeBut[boutonSelected].couleurTxt=ROUGE;
                    listeBut[boutonSelected].couleurRect=GRIS_TR;
                }
                boutonSelected--;
                if(boutonSelected<0)
                    boutonSelected=listeBut.size()-1;
                listeBut[boutonSelected].couleurTxt=JAUNE;
                listeBut[boutonSelected].couleurRect=NOIR;
            }
            break;
        case ALLEGRO_KEY_RIGHT :    // deplacement droite
        case KEYBOARD_D :
            move["droite"]=true;
            break;
        case ALLEGRO_KEY_LEFT :    // deplacement gauche
        case KEYBOARD_Q :
            move["gauche"]=true;
            break;
        
        case KEYBOARD_R :
            break;

        case KEYBOARD_F :
            break;

        case KEYBOARD_N :
            break;
        case KEYBOARD_A :
            break;
        case KEYBOARD_C :
            break;
        case KEYBOARD_B :
            break;
        case ALLEGRO_KEY_PAD_0 :
        case ALLEGRO_KEY_0 :
            break;
        case ALLEGRO_KEY_PAD_1 :
        case ALLEGRO_KEY_1 :
            break;
        case ALLEGRO_KEY_PAD_2 :
        case ALLEGRO_KEY_2 :
            break;
        case ALLEGRO_KEY_PAD_3 :
        case ALLEGRO_KEY_3 :
            break;
    }
    if(perso->getSpeed().x+ACCELERATION < MAX_SPEED && perso->getSpeed().x-ACCELERATION > -MAX_SPEED) {
        perso->setSpeed(perso->getSpeed().x+ACCELERATION*(move["droite"]-move["gauche"]) , perso->getSpeed().y-jump_force*move["sauter"] );
    }
}
void Game::handleKeyUnPressed(int keycode, ALLEGRO_EVENT_QUEUE *event_queue) {
    switch(keycode)
    {      
        case ALLEGRO_KEY_ESCAPE : // EXIT
            break;
        case ALLEGRO_KEY_ENTER :    // ACCUEIL
            break;
        case ALLEGRO_KEY_DOWN :    // deplacement bas
        case KEYBOARD_S :
            move["baisse"]=false;
            break;
        case ALLEGRO_KEY_UP :    // deplacement haut
        case KEYBOARD_Z :
        case ALLEGRO_KEY_SPACE :
            move["sauter"]=false;
            break;
        case ALLEGRO_KEY_RIGHT :    // deplacement droite
        case KEYBOARD_D :
            move["droite"]=false;
            break;
        case ALLEGRO_KEY_LEFT :    // deplacement gauche
        case KEYBOARD_Q :
            move["gauche"]=false;
            break;
        
        case KEYBOARD_R :
            break;

        case KEYBOARD_F :
            break;

        case KEYBOARD_N :
            break;
        case KEYBOARD_A :
            break;
        case KEYBOARD_C :
            break;
        case KEYBOARD_B :
            break;
        case ALLEGRO_KEY_PAD_0 :
        case ALLEGRO_KEY_0 :
            break;
        case ALLEGRO_KEY_PAD_1 :
        case ALLEGRO_KEY_1 :
            break;
        case ALLEGRO_KEY_PAD_2 :
        case ALLEGRO_KEY_2 :
            break;
        case ALLEGRO_KEY_PAD_3 :
        case ALLEGRO_KEY_3 :
            break;
    }
}
void Game::checkMouseOnButton() {
    bouton monBut;
    // bool boutonNewSelected=false;
    for (int i = 0; i < NBR_BOUT; ++i) {
        monBut = listeBut[i];
        if(souris.x>monBut.posX && souris.x<monBut.posX+monBut.w && souris.y>monBut.posY && souris.y<monBut.posY+monBut.h) {
            listeBut[i].couleurTxt=JAUNE;
            listeBut[i].couleurRect=NOIR;
            boutonSelected=i;
            // boutonNewSelected =true;
        }
        else {
            listeBut[i].couleurTxt=ROUGE;
            listeBut[i].couleurRect=GRIS_TR;
        }
    }
    // if(!boutonNewSelected) // si aucun bouton selectionné on remet à 0
    //     boutonSelected = -1;
}
void Game::actionOnMenu() {
    switch(boutonSelected)
    {
        case 0 :
            boutonSelected=-1;
            menuSelected=false;
            break;
        case 1 :
            afficheCommandes();
            // cout << "Controls : \n\tECHAP -> Menu Pause\n\tF -> plein écran\n\tZ/UP/SPACE -> Sauter\n\tQ/Left -> gauche\n\tD/Right -> droite\n\tS/DOWN -> se baisser\n\tC -> saisir objet\n\tB -> boule de feu" << endl;
            break;
        case 2 :
            sounds_on=!sounds_on;
            // playSound=!playSound;
            break;
        case 3 :
            gameOver=true;
            break;
        default :
            break;
    }
}
void Game::afficheCommandes() {
    ALLEGRO_DISPLAY *d = NULL;
    ALLEGRO_EVENT_QUEUE *evt_queue = NULL;
    ALLEGRO_BITMAP *targetBitmap = NULL;
    ALLEGRO_BITMAP* img = NULL;
    ALLEGRO_EVENT evt;
    int largeur=1200, hauteur=600;

    al_destroy_bitmap(img);
    img=al_load_bitmap("datas/images/COMMANDES.png");
    if(!img)
        erreur("chargement image commandes");

    al_set_new_display_flags(ALLEGRO_WINDOWED);
    d = al_create_display(largeur,hauteur);
    if (!d)
        erreur("création de la fenêtre");

    al_set_window_title(d, "COMMANDES");

    evt_queue = al_create_event_queue();
    if (!evt_queue)
         erreur("création de la file d'événements");

    targetBitmap = al_get_backbuffer(d); // Obtient la cible de rendu par défaut

    al_register_event_source(evt_queue, al_get_display_event_source(d));
    al_register_event_source(evt_queue, al_get_keyboard_event_source());

    al_set_target_bitmap(targetBitmap); // Définit la cible de rendu
    al_clear_to_color(GRIS);

    al_draw_scaled_bitmap(img,0,0,al_get_bitmap_width(img),al_get_bitmap_height(img),0,0,largeur,hauteur,0);
    al_draw_text(polices[3], ORANGE, 10, 10, ALLEGRO_ALIGN_LEFT, "LISTE DES COMMANDES");

    al_flip_display();
    bool fin=false;
    
    while(!fin) {
        al_wait_for_event(evt_queue, &evt);

        switch (evt.type)
        {
            case ALLEGRO_EVENT_DISPLAY_CLOSE :
            case ALLEGRO_EVENT_KEY_CHAR :
                fin=true;
                break;
        }
    }
    al_destroy_event_queue(evt_queue);
    al_destroy_display(d);
    al_set_target_backbuffer(display);
}

void Game::update() {

    // Joue musique
    if(sounds_on)
        mesSons["music"]->play();
    else
        mesSons["music"]->stop();

    // Gestion du temps
    if(num_map > 0) {
        cmptFrames++;
        if( cmptFrames>FRAME_RATE) {
            temps += Duree(0,0,1);
            cmptFrames = 0;
        }
    }
    
    if(started) {
        // FREINAGE
        if(!move["droite"] && !move["gauche"] && perso->getSpeed().x!=0) {
            if(perso->getSpeed().x>0)
                perso->setSpeedX(perso->getSpeed().x-FREIN);
            else if(perso->getSpeed().x<0)
                perso->setSpeedX(perso->getSpeed().x+FREIN);
        }
        // Limite perso
        if(perso->getPos().y < window_height-base_sol-perso->getH()) // condition provisoire du perso limite descente
            perso->setSpeedY(perso->getSpeed().y+gravity);
        else {
            perso->setPosY(window_height-base_sol-perso->getH()-1);
            perso->setSpeedY(0);
        }

        // acualise coord
        perso->actualisePos();

    } else {
        // Défilement message accueil
        posPrintPart -= TEXT_SPEED;
        if (posPrintPart < -al_get_text_width(polices[1], msg_accueil.c_str()))
            posPrintPart = window_width+posPrintPart;  // Réinitialisation à droite une fois que le texte a dépassé l'écran
    }
}


// ---------------  MAPS ------------------//
void Game::load_maps() {
    char nom[50];
    maps.clear();
    for (int i = 0; i < NB_MAPS; ++i) {
        sprintf(nom, "datas/maps/map_%d.png", i);
        Map *carte = new Map(nom);
        if(!carte)
            erreur("chargement des maps");
        else
            maps.push_back(*carte);
    }
}
void Game::changeMap() 
{
    if(num_map>0) {
        maps[num_map].setMap0(false);
        maps[num_map].setBackgroundScale(2);
    }
    switch(num_map) 
    {
        case 0 : 
            base_sol=createMap0();
            break;
        // case 1 : 
        //     base_sol=createMap1();
        //     break;
        // case 2 : 
        //     base_sol=createMap2();
        //     break;
        // case 3 : 
        //     base_sol=createMap3();
        //     break;
        // case 4 : 
        //     base_sol=createMap4();
        //     break;
        // case 5 : 
        //     base_sol=createMap5();
        //     break;
        default: 
            base_sol=createMap0();
            break;
    }
    maps[num_map-1].setBackgroundX(0);
}
int Game::createMap0() 
{
    int Le,He;
    POS newCoord;
    sol=60;

    blocs.clear(); // efface tous les blocs de la map précédente

    // SOL
    blocs.push_back(Bloc("datas/images/terre.png",0,0,ZERO,0.5*RATIO_FRAME,false,false,TERRE));
        sol=blocs.at(0).getH();
    blocs.pop_back();
    blocs.push_back(Bloc("datas/images/terre.png",0,window_height-sol,ZERO,0.5*RATIO_FRAME,false,false,TERRE));

    while( blocs.back().getCoord().x + blocs.back().getW() < window_width )
        blocs.push_back(Bloc("datas/images/terre.png",blocs.back().getCoord().x+blocs.back().getW(),window_height-sol,ZERO,0.5*RATIO_FRAME,false,false,TERRE));

    nbrBlocsSol = blocs.size();

    blocs.push_back((Bloc("datas/images/logo_mario2.png",window_width/2-150,window_height-sol-150,ZERO,0.1*RATIO_FRAME,false,false,PERSO)));
    blocs.push_back((Bloc("datas/images/logo_luigi2.png",window_width/2+150-blocs.back().getW(),blocs.back().getCoord().y,ZERO,0.1*RATIO_FRAME,false,false,PERSO)));

// --- CHATEAU --------
    blocs.push_back((Bloc("datas/images/chateau_bas.png",0,0,ZERO,0.2*RATIO_FRAME,false,false,CHATEAU)));
        newCoord.x = window_width-blocs.back().getW(); 
        newCoord.y = window_height-sol-blocs.back().getH();
        blocs.back().setCoord(newCoord);
    blocs.push_back((Bloc("datas/images/chateau_haut.png",0,0,ZERO,0.2*RATIO_FRAME,false,false,CHATEAU)));
        newCoord.x = blocs[blocs.size()-2].getCoord().x+blocs[blocs.size()-2].getW()/2-blocs.back().getW()/2; 
        newCoord.y = blocs[blocs.size()-2].getCoord().y-blocs.back().getH()+20;
        blocs.back().setCoord(newCoord);

    Bloc tmp1=blocs[blocs.size()-2];
    Bloc tmp2=blocs.back();
    blocs.pop_back();
    blocs.pop_back();
    blocs.push_back(tmp2);
    blocs.push_back(tmp1);
    sortie=blocs.size()-1;

// ----- ENTREE ----------
    entree=0;

    return sol;
}


// ---------------  COLLISIONS ------------------//


