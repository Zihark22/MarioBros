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
    agrandi_fact=1;

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

    // Repères
    base_sol = nbrBlocsSol = entree = sortie = num_map = cmptFrames = cmptAnim = souris.x = souris.y = isOrientedLeft = 0;

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
    setWindowPOS(screenWmac-50,screenHmac-50,display); // Déplace la fenetre hors du cadre
    do {
        nomUser = saisirUserName();
    } while (nomUser.size()<2);
    while(!al_is_event_queue_empty(event_queue))
        al_flush_event_queue(event_queue);
    setWindowPOS(50,50,display); // Remet fenetre au centre
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
    maps[num_map].draw(window_width, window_height);    // draw maps
    for (int i = 0; i < blocs.size(); ++i) 
        blocs[i].draw();                                // draw blocs
    if(blocs[sortie].getType()==CHATEAU) {
        int xaffich, yaffich;
        xaffich=blocs[sortie].getCoord().x+blocs[sortie].getW()/2;
        yaffich=blocs[sortie].getCoord().y+25;
        al_draw_filled_rectangle(xaffich-25, yaffich, xaffich+25, yaffich+20, BLANC);
        al_draw_text(polices[3], ROUGE, xaffich, yaffich, ALLEGRO_ALIGN_CENTER, "EXIT");
    }
    afficherTexte();                                    // draw infos
    perso->draw();                                      // draw perso
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
        case ALLEGRO_KEY_S :
            move["baisser"]=true;
            if(isOrientedLeft)
                perso->changeActualImg("baisserG");
            else
                perso->changeActualImg("baisserD");
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
        case ALLEGRO_KEY_Z :
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
        case ALLEGRO_KEY_D :
            move["droite"]=true;
            isOrientedLeft=false;
            perso->changeActualImg("droite");
            break;
        case ALLEGRO_KEY_LEFT :    // deplacement gauche
        case ALLEGRO_KEY_Q :
            move["gauche"]=true;
            isOrientedLeft=true;
            perso->changeActualImg("gauche");
            break;
        
        case ALLEGRO_KEY_R :
            break;

        case ALLEGRO_KEY_F :
            break;

        case ALLEGRO_KEY_N :
            break;
        case ALLEGRO_KEY_A :
            break;
        case ALLEGRO_KEY_C :
            break;
        case ALLEGRO_KEY_B :
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
void Game::handleKeyUnPressed(int keycode, ALLEGRO_EVENT_QUEUE *event_queue) {
    switch(keycode)
    {      
        case ALLEGRO_KEY_ESCAPE : // EXIT
            break;
        case ALLEGRO_KEY_ENTER :    // ACCUEIL
            break;
        case ALLEGRO_KEY_DOWN :    // deplacement bas
        case ALLEGRO_KEY_S :
            move["baisser"]=false;
            if(isOrientedLeft)
                perso->changeActualImg("gauche");
            else
                perso->changeActualImg("droite");
            break;
        case ALLEGRO_KEY_UP :    // deplacement haut
        case ALLEGRO_KEY_Z :
        case ALLEGRO_KEY_SPACE :
            move["sauter"]=false;
            break;
        case ALLEGRO_KEY_RIGHT :    // deplacement droite
        case ALLEGRO_KEY_D :
            move["droite"]=false;
            perso->changeActualImg("droite");
            break;
        case ALLEGRO_KEY_LEFT :    // deplacement gauche
        case ALLEGRO_KEY_Q :
            move["gauche"]=false;
            perso->changeActualImg("gauche");
            break;
        
        case ALLEGRO_KEY_R :
            break;

        case ALLEGRO_KEY_F :
            break;

        case ALLEGRO_KEY_N :
            break;
        case ALLEGRO_KEY_A :
            break;
        case ALLEGRO_KEY_C :
            break;
        case ALLEGRO_KEY_B :
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
    cmptFrames++;
    if(cmptFrames>FRAME_RATE) {
        if(num_map > 0) 
            temps += Duree(0,0,1);
        cmptFrames = 0;
    }
    
    if(started) 
    {
        // Gravité
        if(perso->getPos().y + perso->getH() < window_height-sol)
            perso->setSpeedY(perso->getSpeed().y+gravity);
        else
            perso->setSpeedY(0);

        //Deplacements
        if(move["droite"] && perso->getSpeed().x+ACCELERATION <= MAX_SPEED)
            perso->setSpeedX(perso->getSpeed().x+ACCELERATION);
        if(move["gauche"] && perso->getSpeed().x-ACCELERATION > -MAX_SPEED)
            perso->setSpeedX(perso->getSpeed().x-ACCELERATION);
        if(move["sauter"] && perso->getSpeed().y==0) 
            perso->setSpeedY(-jump_force);

        // Effet courir
        if(move["droite"]) {
            if((cmptFrames>FRAME_RATE/6 && cmptFrames<2*FRAME_RATE/4) || cmptFrames>3*FRAME_RATE/4)
                perso->changeActualImg("courirD");
            else
                perso->changeActualImg("droite");
        }
        else if(move["gauche"]) {
            if((cmptFrames>FRAME_RATE/6 && cmptFrames<FRAME_RATE/3) || (cmptFrames>FRAME_RATE/2 && cmptFrames>2*FRAME_RATE/3) || cmptFrames>5*FRAME_RATE/6)
                perso->changeActualImg("courirG");
            else
                perso->changeActualImg("gauche");
        }

        // Freinage
        if(!move["droite"] && !move["gauche"] && perso->getSpeed().x!=0) {
            if(perso->getSpeed().x>0)
                perso->setSpeedX(perso->getSpeed().x-FREIN);
            else if(perso->getSpeed().x<0)
                perso->setSpeedX(perso->getSpeed().x+FREIN);
        }

        // Gestion des collisions
        handleCollisions();

        // Actualise coord
        perso->actualisePos();

        // Gestion des bords
        if(perso->getPos().x<0)
            perso->setPosX(0);
        if(perso->getPos().x+perso->getW()>window_width)
            perso->setPosX(window_width-perso->getW());
        if(perso->getPos().y>window_height)
            perso->setPosY(0);

        // Animations de sortie ou d'entrée
        checkAnimations();

    } else {
        // Défilement message accueil
        posPrintPart -= TEXT_SPEED;
        if (posPrintPart < -al_get_text_width(polices[1], msg_accueil.c_str()))
            posPrintPart = window_width+posPrintPart;  // Réinitialisation à droite une fois que le texte a dépassé l'écran
    }
}
void Game::masqueRGB(ALLEGRO_DISPLAY *display, ALLEGRO_BITMAP *image, bool R, bool G, bool B) {
    ALLEGRO_COLOR color, newColor;
    int pR,pG,pB;
    unsigned char red, green, blue;
    int largeur = al_get_bitmap_width(image);
    int hauteur = al_get_bitmap_height(image);

    al_set_target_bitmap(image);

    if (al_lock_bitmap(image, al_get_bitmap_format(image), ALLEGRO_LOCK_READWRITE) != NULL)
    {
        for (int K = 0; K < largeur; K++) {
            for (int L = 0; L < hauteur; L++) {
                // Obtenir la couleur du piKel
                color = al_get_pixel(image, K, L);

                al_unmap_rgb(color, &red, &green, &blue);
                //fprintf(stderr, "origine (K=%d,L=%d): \tR=%d G=%d B=%d\n",K,L,(int)red,(int)green,(int)blue);

                pR = R ? 0 : (int)red; 
                pG = G ? 0 : (int)green; 
                pB = B ? 0 : (int)blue; 

                newColor = al_map_rgb(pR, pG, pB);
                //fprintf(stderr, "new (K=%d,L=%d):     \tR=%d G=%d B=%d\n",K,L,pR,pG,pB);

                // Dessiner le piKel avec la nouvelle couleur
                al_put_pixel(K, L, newColor);
            }
        }
    } 
    al_unlock_bitmap(image);
    al_set_target_backbuffer(display);
}
void Game::checkAnimations() {
    if(anim_fin) // fin de niveau via porte ou chateau
    { 
        perso->setSpeedY(0);
        perso->setPosY(window_height-sol-perso->getH());
        if(blocs[sortie].getType()==CHATEAU) 
        {
            mesSons["music"]->setGain(0.2);
            if(sounds_on) 
                 mesSons["son_finish"]->play();
            if(perso->getPos().x+perso->getW()/2>=blocs[sortie].getCoord().x+blocs[sortie].getW()/2) { // si perso au nvx de la porte
                perso->changeActualImg("entrer");
                perso->setSpeedX(0);
                cmptAnim++;
            }
            else
                perso->setSpeedX(2); // avance jusqu a la porte
        }
        else  // porte
        {
            cmptAnim++;
            perso->changeActualImg("entrer");
            perso->setSpeedX(0);
        }
        
        if(cmptAnim>50) {
            if(num_map>=NB_MAPS-1) {
                // finish();
                anim_fin=false;
                anim_entree=false;
                cmptAnim=0;
            }
            else {
                num_map++;
                changeMap();
                perso = new User(perso->getNom());
                perso->setPos(blocs[entree-1].getCoord().x+blocs[entree-1].getW()/2-perso->getW()/2 , blocs[entree-1].getCoord().y);
                anim_fin=false;
                anim_entree=true;
                cmptAnim=0;
            }
        }
        else if(cmptAnim>15)
            masqueRGB(display, perso->getImg(), true, true, true);
        else if(cmptAnim>10)
            masqueRGB(display, perso->getImg(), true, true, false);
        else if(cmptAnim>5)
            masqueRGB(display, perso->getImg(), true, false, false);
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
        case 1 : 
            base_sol=createMap1();
            break;
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
int Game::createMap1() 
{
    int tmpH;
    POS newCoord;
    int diffBords=0;

    blocs.clear(); // efface tous les blocs de la map précédente

    // -------- SOL ---------------

    blocs.push_back(Bloc("datas/images/sol_terre.png",0,0,ZERO,1,false,false,TERRE));
        const int sol = blocs.back().getH();
        blocs.back().setCoord( (POS) { 0, window_height-sol } );

    while( blocs.back().getCoord().x + blocs.back().getW() < maps[num_map].getW()*maps[num_map].getBackgroundScale() )
    {
        blocs.push_back(Bloc("datas/images/sol_terre.png",blocs.back().getCoord().x+blocs.back().getW(),window_height-sol,ZERO,1*RATIO_FRAME,false,false,TERRE));
    }
    
    nbrBlocsSol = blocs.size();

    // -------- JEU ---------------
    
    blocs.push_back(Bloc("datas/images/bloc.png",350,window_height-sol-150,ZERO,1*RATIO_FRAME,false,false,BLOC));
    blocs.push_back(Bloc("datas/images/bloc_mystere.png",blocs.back().getCoord().x+blocs.back().getW(),blocs.back().getCoord().y,ZERO,1*RATIO_FRAME,false,true,MYSTERE));
    blocs.push_back(Bloc("datas/images/coin2.png",0,0,ZERO,0.3*RATIO_FRAME,true,false,COIN)); 
        newCoord.x = blocs[blocs.size()-2].getCoord().x+blocs[blocs.size()-2].getW()/2-blocs.back().getW()/2;
        newCoord.y = blocs[blocs.size()-2].getCoord().y-blocs.back().getH()-2;
        blocs.back().setCoord(newCoord);
        blocs.back().disable();
        Bloc tmpBloc=blocs[blocs.size()-2];
        blocs[blocs.size()-2]=blocs.back();
        blocs.back()=tmpBloc;
    blocs.push_back(Bloc("datas/images/bloc.png",blocs.back().getCoord().x+blocs.back().getW(),blocs.back().getCoord().y-blocs.back().getH()+5,ZERO,1*RATIO_FRAME,false,false,BLOC));
    blocs.push_back(Bloc("datas/images/bloc_mystere.png",blocs.back().getCoord().x+blocs.back().getW()+200,blocs.back().getCoord().y-20,ZERO,1*RATIO_FRAME,false,true,MYSTERE));
    blocs.push_back(Bloc("datas/images/fleur.png",0,0,ZERO,0.4*RATIO_FRAME,true,false,FLEUR));
        newCoord.x = blocs[blocs.size()-2].getCoord().x+blocs[blocs.size()-2].getW()/2-blocs.back().getW()/2;
        newCoord.y = blocs[blocs.size()-2].getCoord().y-blocs.back().getH()-2;
        blocs.back().setCoord(newCoord);
        blocs.back().disable();
        tmpBloc=blocs[blocs.size()-2];
        blocs[blocs.size()-2]=blocs.back();
        blocs.back()=tmpBloc;
    blocs.push_back(Bloc("datas/images/coin2.png",0,0,ZERO,0.3*RATIO_FRAME,true,false,COIN)); 
        newCoord.x = 300;
        newCoord.y = window_height-sol-100;
        blocs.back().setCoord(newCoord);
    blocs.push_back(Bloc("datas/images/coin2.png",0,0,ZERO,0.3*RATIO_FRAME,true,false,COIN)); 
        newCoord.x = 600;
        newCoord.y = window_height-sol-150;
        blocs.back().setCoord(newCoord);

// -------- ENTREE ---------------

    blocs.push_back(Bloc("datas/images/tuyau_haut.png",0,0,INVERSION,1*RATIO_FRAME,false,false,TUYAU));
        diffBords = blocs.back().getW();
    blocs.push_back(Bloc("datas/images/tuyau_bas.png",0,0,INVERSION,1*RATIO_FRAME,false,false,TUYAU));
        diffBords -= blocs.back().getW();
        diffBords /=2;
        blocs.back().setCoord( (POS) { diffBords, HAUTEUR_TEXTE} );
    blocs.push_back(Bloc("datas/images/tuyau_haut.png",0,0,INVERSION,1*RATIO_FRAME,false,false,TUYAU));
        blocs.back().setCoord( (POS) { 0, blocs[blocs.size()-2].getCoord().y+blocs[blocs.size()-2].getH() } );
        // entree = blocs.size()-1;
        entree = 0;

// -------- SORTIE ---------------

    blocs.push_back(Bloc("datas/images/tuyau_haut.png",0,0,ZERO,1*RATIO_FRAME,false,false,TUYAU));
        diffBords = blocs.back().getW();
    blocs.push_back(Bloc("datas/images/tuyau_bas.png",0,0,GAUCHE,1*RATIO_FRAME,false,false,TUYAU));
        diffBords -= blocs.back().getW();
        diffBords /=2;
        tmpH=blocs.back().getH(); blocs.back().setH(blocs.back().getW()); blocs.back().setW(tmpH);
        blocs.back().setCoord( (POS) { (int) (maps[num_map].getW() - blocs.back().getW()) , window_height-blocs.back().getH()-sol } );
    blocs.push_back(Bloc("datas/images/tuyau_haut.png",0,0,GAUCHE,1*RATIO_FRAME,false,false,TUYAU));
        tmpH=blocs.back().getH(); blocs.back().setH(blocs.back().getW()); blocs.back().setW(tmpH);
        blocs.back().setCoord( (POS) { (int) (maps[num_map].getW() - blocs[blocs.size()-2].getW()-blocs.back().getW())+20 , window_height-sol-blocs.back().getH()+diffBords } );
        sortie = blocs.size()-1;

//--------------- MECHANTS -----------------
/* 
    mechants[nbrMechants] = new Mechant("goomba");
        mechants[nbrMechants].setTaille(0.06*RATIO_FRAME);
        mechants[nbrMechants].actualiseSize(0);
        mechants[nbrMechants].setSpeedX(-3);
        mechants[nbrMechants].setPos(200,window_height-sol-mechants[nbrMechants].getH()-2);
        nbrMechants++;

    mechants[nbrMechants] = new Mechant("koopa");
        mechants[nbrMechants].setTaille(0.03*RATIO_FRAME);
        mechants[nbrMechants].actualiseSize(0);
        mechants[nbrMechants].setSpeedX(-3);
        mechants[nbrMechants].setPos(500,window_height-sol-mechants[nbrMechants].getH());
        nbrMechants++;
 */
    return sol;
}


// ---------------  COLLISIONS ------------------//
float Game::conv_to_Rad(float const& degrees) {
    return degrees*PI/180;
}
float Game::conv_to_Deg(float const& rad) {
    return rad*180/PI;
}
float Game::calculateAngle(VECT2D const& vectorA, VECT2D const& vectorB) {
    float dotProduct = vectorA.x * vectorB.x + vectorA.y * vectorB.y;
    float magnitudeA = sqrt(vectorA.x * vectorA.x + vectorA.y * vectorA.y);
    float magnitudeB = sqrt(vectorB.x * vectorB.x + vectorB.y * vectorB.y);
    float cosAngle = dotProduct / (magnitudeA * magnitudeB);
    float sinAngle = sqrt(1.0f - pow(cosAngle,2));

    float angle = acos(cosAngle);

    if(vectorA.x*vectorB.y-vectorA.y*vectorB.x < 0.0f)
        angle = 2*PI-angle;

    return angle; // conv_to_Deg(angle);
}
int Game::collisionPersoBloc(User const& perso, Bloc const& bloc) 
{
    // Positions
    int px = perso.getPos().x;
    int py = perso.getPos().y;
    int bx = bloc.getCoord().x;
    int by = bloc.getCoord().y;

    // Tailles
    int pw = perso.getW();
    int ph = perso.getH();
    int bw = bloc.getW();
    int bh = bloc.getH();

    // Centre
    int bcx = bx+bw/2;
    int bcy = by+bh/2;
    int pcx = px+pw/2;
    int pcy = py+ph/2;

    // Distances
    int dcx = abs(bcx-pcx);
    int dcy = abs(bcy-pcy);

    VECT2D vect_bloc_perso = {(float)pcx-bcx , (float)pcy-bcy , 0};
    vect_bloc_perso.norme = sqrt( pow(vect_bloc_perso.x,2) + pow(vect_bloc_perso.y,2) );

    VECT2D vect_bloc = { (float)bcx+bw/2-bcx , (float)bcy-bcy , 1 };

    float angle = calculateAngle(vect_bloc_perso, vect_bloc);

    if(dcx<=(pw+bw)/2 && dcy<=(ph+bh)/2 && vect_bloc_perso.norme < sqrt( pow((pw+bw)/2,2) + pow((ph+bh)/2,2) ))  // Collision
    {
        if(angle>7*PI/6 and angle<11*PI/6)
            return NORD;
        else if(angle>PI/6 and angle<5*PI/6)
            return SUD;
        else if(angle<1*PI/6 or angle>11*PI/6)
            return OUEST;
        else if(angle>5*PI/6 and angle<7*PI/6)
            return EST;
        else {
            cerr << "erreur perso/bloc" << endl;
            return FIN;
        }
    }
    else 
        return FIN; 
}

void Game::handleCollisions()
{
    int i=0, j=0, k=0, nbrColl=0, dirCollision=0, tmpNbrObjets=0;
    vector<ObjectLance> tmpObjets;
    POS tmpCoord;
    int cmptNoCollisions=0;

    // Collisions perso/blocs
    if(anim_sortie==false && anim_entree==false && anim_fin==false) 
    { 
        for(i=0;i<blocs.size();i++)
        {
            dirCollision = collisionPersoBloc(*perso , blocs[i]);
            // afficheOrientation(dirCollision);
            if(dirCollision!=FIN && i<nbrBlocsSol) { // si collision avec le sol
                if(move["sauter"]==false)
                    perso->setSpeedY(0);  // pas de déplacement en Y sauf si saut
                sol=base_sol;
                if(perso->getPos().y<window_height-sol-perso->getH() && sounds_on) 
                    mesSons["son_sol"]->play();
                perso->setPosY(window_height-sol-perso->getH());
            }
            else if(dirCollision!=FIN && blocs[i].isObject()==true && blocs[i].isHiding()==false && num_map!=0) {  //   tape le bloc mystère par en dessous
                if(blocs[i].is_enable()==true) 
                {
                    blocs[i].disable();
                    blocs[i].setCoord(-50,-50);
                    switch(blocs[i].getType()) 
                    {
                        case CHAMPI :
                            agrandi_fact=1;
                            gravity=2;
                            perso->setMessage(">");
                            if(sounds_on) 
                                mesSons["son_powerUp"]->play();
                            break;
                        case CHAMPI_GEANT :
                            agrandi_fact=2;
                            gravity=2;
                            if(sounds_on) 
                                mesSons["son_powerUp"]->play();
                            break;
                        case COIN :
                            score++;
                            if(score%10==0 and score!=0) {
                                perso->setMessage("Zup");
                                vies++;
                                score=0;
                            }
                            else {
                                perso->setMessage("+1");
                                if(sounds_on) 
                                    mesSons["son_coin"]->play();
                            }
                            break;
                        case FLEUR :
                            tmpCoord = perso->getPos();
                            perso = new User("marioFire");
                            perso->setPos(tmpCoord);
                            agrandi_fact=1;
                            gravity=2;
                            if(sounds_on) 
                                mesSons["son_powerUp"]->play();
                            break;
                        case CHAMPI_MINI :
                            tmpCoord = perso->getPos();
                            if(perso->getNom().compare("marioFire")==0)
                                perso = new User("mario");
                            perso->setPos(tmpCoord);
                            agrandi_fact=0.5;
                            gravity=1;
                            if(sounds_on) 
                                mesSons["son_powerUp"]->play();
                            break;
                        case CHAMPI_DOWN :
                            break;
                        case ETOILE :
                            break;
                    }
                }
            }
            else if(dirCollision!=FIN && blocs[i].getType()==DOOR_CLOSED) {
                if(move["sauter"]) {
                    perso->setSpeedY(0);
                    perso->changeActualImg("entrer");
                    POS coordSortie = blocs[sortie].getCoord();
                    blocs[sortie] = Bloc("datas/images/Door_open.png",0,0,ZERO,1,false,false,DOOR_OPEN);
                    blocs[sortie].setCoord(coordSortie);
                    perso->setPosX(blocs[sortie].getCoord().x+blocs[sortie].getW()/2-perso->getW()/2);
                    anim_fin=true;
                }
            }
            else if(dirCollision!=FIN && blocs[i].getType()==DOOR_OPEN) {
                if(move["sauter"]) {
                    al_rest(0.5);
                    num_map = num_map >= NB_MAPS-1 ? 1 : num_map+1;
                    changeMap();
                }
            }
            else 
            {
                switch(dirCollision) 
                {
                    case SUD :
                        if(i==sortie && blocs[sortie].getType()==TUYAU && blocs[sortie].getAngle()==ZERO && move["baisser"]) { // sortie de map dans le tuyau
                            anim_sortie=true;
                        }
                        else {
                            if(!move["sauter"]) 
                                perso->setSpeedY(0);
                            sol=window_height-blocs[i].getCoord().y-1;
                            if(perso->getPos().y<window_height-sol-perso->getH() && sounds_on) 
                                mesSons["son_sol"]->play();
                            perso->setPosY(window_height-sol-perso->getH());
                            
                        }
                        break;

                    case EST :
                        if(i==sortie && blocs[sortie].getType()==TUYAU && (blocs[sortie].getAngle()==GAUCHE || blocs[sortie].getAngle()==DROITE) ) { // sortie de map dans le tuyau
                            anim_sortie=true;
                        }
                        else if(i==sortie && sortie !=0 && blocs[i].getType()==CHATEAU && perso->getNom()!="stickman" && perso->getPos().x+perso->getW()>blocs[sortie].getCoord().x && perso->getPos().y+perso->getH()>window_height-base_sol-5) { // sortie par le chateau
                            anim_fin=true;
                            // On remet les valeurs d'origine à la fin pour recommencer
                            jump_force=25; 
                            gravity=2;
                        }
                        else {
                            perso->setPosX(blocs[i].getCoord().x-perso->getW()-1);
                            if(perso->getSpeed().x!=0) 
                                perso->setSpeedX(0);
                        }
                        break;

                    case OUEST :
                        perso->setPosX(blocs[i].getCoord().x+blocs[i].getW()+1);
                        if(perso->getSpeed().x!=0) 
                            perso->setSpeedX(0);
                        break;

                    case NORD :
                        perso->setPosY(blocs[i].getCoord().y+blocs[i].getH()+1);
                        perso->setSpeedY(0);
                        tmpCoord = perso->getPos();
                        if(i==0+nbrBlocsSol && num_map==0) {
                            perso = new User("mario");
                            perso->setPos(tmpCoord);
                            if(sounds_on) 
                                mesSons["son_mario"]->play();
                        }
                        else if(i==1+nbrBlocsSol && num_map==0) { 
                            perso = new User("luigi");
                            perso->setPos(tmpCoord);
                            if(sounds_on) 
                                mesSons["son_luigi"]->play();
                        }
                        else if(blocs[i].getType()==MYSTERE && blocs[i].isHiding()==true && blocs[i-1].isObject()==true && blocs[i-1].is_enable()==false) { // tape le bloc mystère par en dessous
                            // masqueRGB(display, blocs[i].getImg(), false, true, true);
                            blocs[i]= Bloc("datas/images/bloc_vide.png",blocs[i].getCoord().x,blocs[i].getCoord().y,ZERO,1,false,false,BLOC_VIDE);
                            blocs[i-1].enable();
                            blocs[i-1].setSortieObjet(true);
                            blocs[i-1].setCoord(blocs[i-1].getCoord().x , blocs[i].getCoord().y);
                            blocs[i].setHiding(false);
                        }
                        break;

                    default : // dirCollision = FIN
                        cmptNoCollisions++;
                        break;
                } // fin switch
            }
        } // fin du for
        if(cmptNoCollisions>=blocs.size())
            sol=base_sol;
    }  // fin bool

}