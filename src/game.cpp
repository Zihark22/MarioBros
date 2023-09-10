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

    // Jeu
    perso = new User("_");
    gameOver = false;
    level = 0;
    fullscreen = false;
    playsound  = false;
    gravity = 2;
    jump_force = -25;
    maps.clear();
    blocs.clear();
    mechants.clear();
    objets.clear();

    // Repères
    base_sol = nbrBlocsSol = entree = sortie = num_map = 0;

    // Accueil
    msg_accueil = "Bienvenu dans Mario Bros. !! Appuyez sur ENTRER pour JOUER";
    posHidePart = 0;
    posPrintPart= 0;

    // Moments du jeu
    enter = false; 

    // Animations
    anim_fin      = false;
    anim_tuyau    = false;
    anim_entree   = false;
    anim_sortie   = false;
    anim_perteVie = false;

    // Methodes
    charge_polices();
    load_maps();


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


    // Initialisation des boutons
    listeBut.push_back( (bouton) { "CONTINUER" , window_width/4, (int)((listeBut.size())*window_height/(NBR_BOUT+2)+(listeBut.size()*20)) , 2*WIDTH/4 , HEIGHT/(NBR_BOUT+2) , polices[1] , ROUGE , GRIS_TR } );
    listeBut.push_back( (bouton) { "COMMANDES" , window_width/4, (int)((listeBut.size())*window_height/(NBR_BOUT+2)+(listeBut.size()*20)) , 2*WIDTH/4 , HEIGHT/(NBR_BOUT+2) , polices[1] , ROUGE , GRIS_TR } );
    listeBut.push_back( (bouton) { "SONS"      , window_width/4, (int)((listeBut.size())*window_height/(NBR_BOUT+2)+(listeBut.size()*20)) , 2*WIDTH/4 , HEIGHT/(NBR_BOUT+2) , polices[1] , ROUGE , GRIS_TR } );
    listeBut.push_back( (bouton) { "QUITTER"   , window_width/4, (int)((listeBut.size())*window_height/(NBR_BOUT+2)+(listeBut.size()*20)) , 2*WIDTH/4 , HEIGHT/(NBR_BOUT+2) , polices[1] , ROUGE , GRIS_TR } );
    
    // Création de la fenêtre d'accueil
    posHidePart = al_get_text_width(polices[1], msg_accueil.c_str());  // Position de la partie cachée du texte
    posPrintPart = window_width/2 - posHidePart/2; // Position de la partie affichée du texte
    tracerAccueil();

}
Game::~Game() {
    try 
    {
        // Jeu
        maps.clear();
        blocs.clear();
        mechants.clear();
        objets.clear();

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

/////////////   METHODES AUTRES   ////////////////////
void Game::begin(ALLEGRO_EVENT_QUEUE *event_queue) {
    do {
        nomUser = saisirUserName();
    } while (nomUser.size()<2);
    while(!al_is_event_queue_empty(event_queue))
        al_flush_event_queue(event_queue);
    changeMap();
}
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
    
    al_flip_display();
}

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
void Game::update() {
    maps[num_map].draw(window_width, window_height);
    perso->draw(0); // draw perso
    for (int i = 0; i < blocs.size(); ++i) 
        blocs[i].draw(); // draw blocs
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



// ---------------  MAPS ------------------//
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
