#include "mario.hpp"

void erreur(const char* txt)
{
    ALLEGRO_DISPLAY* d = al_is_system_installed() ? al_get_current_display() : NULL;
    const char* button = "Quitter";
    char titre[50];
    sprintf(titre,"Erreur : %s", txt);
    int result = al_show_native_message_box(d, titre, "Ne fonctionne pas", "Cliquer pour quitter, régler le problème puis recompiler !", button, ALLEGRO_MESSAGEBOX_ERROR); // ALLEGRO_MESSAGEBOX_YES_NO, ALLEGRO_MESSAGEBOX_WARN, ALLEGRO_MESSAGEBOX_ERROR, ALLEGRO_MESSAGEBOX_QUESTION et ALLEGRO_MESSAGEBOX_OK_CANCEL

    // Vérifie le résultat de la boîte de dialogue
    if (result == 1) {
        //printf("L'utilisateur a cliqué sur Quitter.\n");
        // exit(EXIT_FAILURE);
        fin=true;
    } 
}
bool confirmQuit(void)
{
    ALLEGRO_DISPLAY* d = al_is_system_installed() ? al_get_current_display() : NULL;
    int result = al_show_native_message_box(d, "Confirmation", "Attention", "Voulez-vous réellement quitter ?", "OUI|NON", ALLEGRO_MESSAGEBOX_YES_NO); // ALLEGRO_MESSAGEBOX_YES_NO, ALLEGRO_MESSAGEBOX_WARN, ALLEGRO_MESSAGEBOX_ERROR, ALLEGRO_MESSAGEBOX_QUESTION et ALLEGRO_MESSAGEBOX_OK_CANCEL

    // Vérifie le résultat de la boîte de dialogue
    if (result == 1) { // si oui  NON=2 et 0=Rien
        return true;
    }
    return false;
}
int initialisation(void) {
    if (!al_init()) {
        erreur("initialise Allegro5");
    }
    if (!al_init_image_addon()) {
        erreur("initialise Allegro5_image");
    }
    if (!al_init_primitives_addon()) {
        erreur("initialise Allegro5_primitives pour dessiner");
    }
    if (!al_install_keyboard()) {
        erreur("initialise clavier");
    }
    if (!al_install_mouse()) {
        erreur("initialise souris");
    }
    if (!al_init_font_addon()) {
        erreur("initialise police");
    }
    if(!al_init_ttf_addon()) {
        erreur("initialise ttf polices");
    }
    if (!al_install_audio()) {
        erreur("initialise audio");
    }
    if (!al_reserve_samples(1)) {
        erreur("réservation de l'échantillon audio");
    }
    if (!al_init_acodec_addon()) {
        erreur("initialise codecs");
    }
    if (!al_init_video_addon()) {
        erreur("initialise vidéo");
    }
    return 0;
}
void charge_polices() {
    int i=0;
    ALLEGRO_FONT* font=NULL;
    string dossier="polices/";
    string nom;

    nom="Arial.ttf"; font = al_load_font((dossier+nom).c_str(), 36, 0);             if(font) { polices[i]=font; i++;font=NULL; }
    nom="Arial Bold.ttf"; font = al_load_font((dossier+nom).c_str(), 36, 0);        if(font) { polices[i]=font; i++;font=NULL; }
    nom="MarioAndLuigi.ttf"; font = al_load_font((dossier+nom).c_str(), 40, 0);     if(font) { polices[i]=font; i++;font=NULL; }
    nom="Arial Bold.ttf"; font = al_load_font((dossier+nom).c_str(), 20, 0);        if(font) { polices[i]=font; i++;font=NULL; }
    nom="Arial Bold.ttf"; font = al_load_font((dossier+nom).c_str(), 15, 0);        if(font) { polices[i]=font; i++;font=NULL; }
}
void detruit_polices() {
    for (int i = 0; i < NBR_FONTS; ++i)
    {
        if(polices[i]) al_destroy_font(polices[i]);
    }
}
bool MouseOnButton(int mouseX, int mouseY, bouton monBut) {
    if(mouseX>monBut.posX && mouseX<monBut.posX+monBut.w && mouseY>monBut.posY && mouseY<monBut.posY+monBut.h)
        return true;
    else
        return false;
}
string choixPerso(void)
{
    string mario="mario", luigi="luigi";
    ALLEGRO_DISPLAY* d = al_is_system_installed() ? al_get_current_display() : NULL;
    al_set_window_title(d, "PERSO");
    int result = al_show_native_message_box(d, "Choix Perso", "Choisir personnage", "Quel personnage voulez-vous ?", "MARIO|LUIGI", ALLEGRO_MESSAGEBOX_YES_NO); // ALLEGRO_MESSAGEBOX_YES_NO, ALLEGRO_MESSAGEBOX_WARN, ALLEGRO_MESSAGEBOX_ERROR, ALLEGRO_MESSAGEBOX_QUESTION et ALLEGRO_MESSAGEBOX_OK_CANCEL
    if (result == 1)
        return mario;
    else
        return luigi;
}
ALLEGRO_BITMAP* tracerAccueil(ALLEGRO_FONT* font) {
    ALLEGRO_BITMAP* wallpaper = al_load_bitmap(WALLPAPER);
    if (!wallpaper)
        erreur("initialise image d'accueil");

    ALLEGRO_BITMAP *logo = al_load_bitmap(LOGO);
    if (!logo)
        erreur("initialise logo d'accueil");

    al_clear_to_color(al_map_rgb(0, 0, 0));

    al_draw_scaled_bitmap(wallpaper, 0, 0, al_get_bitmap_width(wallpaper), al_get_bitmap_height(wallpaper), 0, 0, window_width, window_height, 0);
    int facteur = 4;
    al_draw_scaled_bitmap(logo, 0, 0, al_get_bitmap_width(logo), al_get_bitmap_height(logo), window_width/2-al_get_bitmap_width(logo)/facteur/2, 3*window_height/4-al_get_bitmap_height(logo)/facteur/2, al_get_bitmap_width(logo)/facteur, al_get_bitmap_height(logo)/facteur, 0);
    int posY = window_height-30;
    al_draw_filled_rectangle(0,posY-30,window_width,posY+30,al_map_rgba(0, 0, 0, 222));
    al_draw_text(font, ORANGE, textX, posY-20, ALLEGRO_ALIGN_LEFT, txt.c_str());
    if(textX<0)
        al_draw_text(font, ORANGE, window_width+textX, posY-20, ALLEGRO_ALIGN_LEFT, txt.c_str());

    return wallpaper;
}
void masqueRGB(ALLEGRO_DISPLAY *display, ALLEGRO_BITMAP *image, bool R, bool G, bool B) {
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
void load_maps() {
    char nom[50];
    for (int i = 0; i < NB_MAPS; ++i) {
        sprintf(nom, "maps/map_%d.png", i);
        maps[i] = new Map(nom);
        if(!maps[i])
            erreur("chargement des maps");
    }
}
void afficheOrientation(int num) {
    string orientation = "";
    switch(num)
    {
        case 0 :
            orientation = "SUD";
            break;
        case 1 :
            orientation = "EST";
            break;
        case 2 :
            orientation = "OUEST";
            break;
        case 3 :
            orientation = "NORD";
            break;
        default :
            orientation = "ERROR";
            break;
    }
    cout << orientation << endl;
}
float conv_to_Rad(float degrees) {
    return degrees*PI/180;
}
void afficherBarreProgression(int pourcentage) {
    // Efface la ligne précédente
    cout << "\033[2K";

    // Déplace le curseur au début de la ligne
    cout << "\r";

    pourcentage = pourcentage * 100 / LOADING_ELEMENTS;

    // Choix de la couleur en fonction du pourcentage
    if (pourcentage <= 25)
        cout << SET_COLOR_BLANC;
    else if (pourcentage <= 50)
        cout << SET_COLOR_RED;
    else if (pourcentage <= 75)
        cout << SET_COLOR_YELLOW;
    else if (pourcentage <= 95)
        cout << SET_COLOR_VIOLET;
    else
        cout << SET_COLOR_GREEN;

    // Affiche la barre de progression
    if(pourcentage==100)
    cout << "Load completed: [";
    else
    cout << "Loading… : [";
    int nombreCaracteres = 50;
    int nombreMarques = pourcentage * nombreCaracteres / 100;
    for (int i = 0; i < nombreCaracteres; ++i) {
    if (i < nombreMarques)
      cout << "◼︎";
    else
      cout << "◻︎";
    }
    cout << "] " << pourcentage << "%" << RESET_COLOR;

    // Rafraîchit la sortie
    cout.flush();
    usleep(50000);
}
string saisirUserName(void)
{
    ALLEGRO_DISPLAY *d = NULL;
    ALLEGRO_EVENT_QUEUE *evt_queue = NULL;
    ALLEGRO_FONT *font = NULL;
    ALLEGRO_BITMAP *targetBitmap = NULL;
    int largeur=400, hauteur=200;

    al_set_new_display_flags(ALLEGRO_WINDOWED);
    d = al_create_display(largeur,hauteur);
    if (!d) {
        fprintf(stderr, "Erreur lors de la création de la fenêtre.\n");
        return "erreur";
    }
    al_set_window_title(d, "PSEUDO");

    font = al_load_ttf_font("polices/Arial Bold.ttf", 25, 0);
    if (!font) {
        fprintf(stderr, "Erreur lors du chargement de la police de caractères.\n");
        return "erreur";
    }

    evt_queue = al_create_event_queue();
    if (!evt_queue) {
        fprintf(stderr, "Erreur lors de la création de la file d'événements.\n");
        return "erreur";
    }

    targetBitmap = al_get_backbuffer(d); // Obtient la cible de rendu par défaut

    al_register_event_source(evt_queue, al_get_display_event_source(d));
    al_register_event_source(evt_queue, al_get_keyboard_event_source());

    ALLEGRO_EVENT evt;
    string nom="_";

    al_set_target_bitmap(targetBitmap); // Définit la cible de rendu

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

    al_destroy_event_queue(evt_queue);
    al_destroy_font(font);
    al_destroy_display(d);
    return nom;

}
void afficheCommandes(void) {
    ALLEGRO_DISPLAY *d = NULL;
    ALLEGRO_EVENT_QUEUE *evt_queue = NULL;
    ALLEGRO_FONT *font = NULL;
    ALLEGRO_BITMAP *targetBitmap = NULL;
    ALLEGRO_BITMAP* img = NULL;
    ALLEGRO_EVENT evt;
    int largeur=1200, hauteur=600;

    al_destroy_bitmap(img);
    img=al_load_bitmap("images/COMMANDES.png");
    if(!img) {
        erreur("chargement du perso");
    }

    al_set_new_display_flags(ALLEGRO_WINDOWED);
    d = al_create_display(largeur,hauteur);
    if (!d) {
        fprintf(stderr, "Erreur lors de la création de la fenêtre.\n");
    }
    al_set_window_title(d, "COMMANDES");

    font = al_load_ttf_font("polices/Arial Bold.ttf", 25, 0);
    if (!font) {
        fprintf(stderr, "Erreur lors du chargement de la police de caractères.\n");
    }

    evt_queue = al_create_event_queue();
    if (!evt_queue) {
        fprintf(stderr, "Erreur lors de la création de la file d'événements.\n");
    }

    targetBitmap = al_get_backbuffer(d); // Obtient la cible de rendu par défaut

    al_register_event_source(evt_queue, al_get_display_event_source(d));
    al_register_event_source(evt_queue, al_get_keyboard_event_source());

    al_set_target_bitmap(targetBitmap); // Définit la cible de rendu
    al_clear_to_color(GRIS);

    al_draw_scaled_bitmap(img,0,0,al_get_bitmap_width(img),al_get_bitmap_height(img),0,0,largeur,hauteur,0);
    al_draw_text(font, ORANGE, 10, 10, ALLEGRO_ALIGN_LEFT, "LISTE DES COMMANDES");

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
    al_destroy_font(font);
    al_rest(0.5); // Attendez
}
void afficherTexte(ALLEGRO_FONT *font) {
    int decaleBords = 40;
    ALLEGRO_BITMAP* coeur = al_load_bitmap("images/coeur.png");
    if (!coeur)
        erreur("initialise image coeur");

    ALLEGRO_BITMAP* horloge = al_load_bitmap("images/horloge.png");
    if (!horloge)
        erreur("initialise image horloge");

    ALLEGRO_BITMAP* coin = al_load_bitmap("images/coin2.png");
    if (!coin)
        erreur("initialise image coin");

    al_draw_filled_rectangle(0,0,window_width,HAUTEUR_TEXTE, NOIR);
    int minutes, secondes, heures;
    level=num_map;
    minutes=secondes=heures=0;

    if(temps>=3600) {
        heures=temps/60;
        minutes=temps%60;
    }
    else {
        heures=0;
        minutes=temps;
    }

    if(minutes>=60) {
        secondes=minutes%60;
        minutes=minutes/60;
    }
    else {
        secondes=temps%60;
        minutes=temps/60;
    }

    string msgNom = "User : " + nomUser;
    al_draw_text(font, ORANGE, decaleBords, HAUTEUR_TEXTE/2-al_get_font_line_height(font)/2, ALLEGRO_ALIGN_LEFT, msgNom.c_str());

    string msgVies = "x " + to_string(vies);
    al_draw_scaled_bitmap(coeur, 0, 0, al_get_bitmap_width(coeur), al_get_bitmap_height(coeur), window_width/4-30, HAUTEUR_TEXTE/2-al_get_font_line_height(font)/2, 25, 25, 0);
    al_draw_text(font, ORANGE, window_width/4, 20, ALLEGRO_ALIGN_LEFT, msgVies.c_str());

    string msgTemps = to_string(secondes)+ "s";
    if(minutes!=0)
        msgTemps = to_string(minutes) + "m " + msgTemps;
    if(heures!=0)
        msgTemps = to_string(heures) + "h " + msgTemps;
    
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
void afficheTypeBloc(int numBloc) {
    switch(blocs[numBloc].getType())
    {
        case TUYAU:
            cout << "bloc: num=" << numBloc <<", type=TUYAU" << endl;
            break;
        case TERRE:
            cout << "bloc: num=" << numBloc <<", type=TERRE" << endl;
            break;
        case CHATEAU:
            cout << "bloc: num=" << numBloc <<", type=CHATEAU" << endl;
            break;
        case MYSTERE:
            cout << "bloc: num=" << numBloc <<", type=MYSTERE" << endl;
            break;
        case BLOC:
            cout << "bloc: num=" << numBloc <<", type=BLOC" << endl;
            break;
        case BLOC_VIDE:
            cout << "bloc: num=" << numBloc <<", type=BLOC_VIDE" << endl;
            break;
        case DOOR_CLOSED:
            cout << "bloc: num=" << numBloc <<", type=DOOR_CLOSED" << endl;
            break;
        case FLEUR:
            cout << "bloc: num=" << numBloc <<", type=FLEUR" << endl;
            break;
        case PERSO:
            cout << "bloc: num=" << numBloc <<", type=PERSO" << endl;
            break;
        case ESCALIER:
            cout << "bloc: num=" << numBloc <<", type=ESCALIER" << endl;
            break;
        case DOOR_OPEN:
            cout << "bloc: num=" << numBloc <<", type=DOOR_OPEN" << endl;
            break;
        case CHAMPI_ROYAL:
            cout << "bloc: num=" << numBloc <<", type=CHAMPI_ROYAL" << endl;
            break;
        case CHAMPI:
            cout << "bloc: num=" << numBloc <<", type=CHAMPI" << endl;
            break;
        case ETOILE:
            cout << "bloc: num=" << numBloc <<", type=ETOILE" << endl;
            break;
        case CHAMPI_MINI:
            cout << "bloc: num=" << numBloc <<", type=CHAMPI_MINI" << endl;
            break;
        case CHAMPI_GEANT:
            cout << "bloc: num=" << numBloc <<", type=CHAMPI_GEANT" << endl;
            break;
        default:
            cout << "bloc inconnu" << endl;
            break;
    }
}
int perdu(int vies)
{
    // ALLEGRO_DISPLAY* d = al_is_system_installed() ? al_get_current_display() : NULL;
    int result;
    if(vies>0) {
        // result = al_show_native_message_box(d, "PERDU", "Dommage", "Il vous reste des vies. Voulez-vous recommencer ?", "OUI|NON", ALLEGRO_MESSAGEBOX_QUESTION); // ALLEGRO_MESSAGEBOX_YES_NO, ALLEGRO_MESSAGEBOX_WARN, ALLEGRO_MESSAGEBOX_ERROR, ALLEGRO_MESSAGEBOX_QUESTION et ALLEGRO_MESSAGEBOX_OK_CANCEL
        return 1;
    }
    else {
        // al_show_native_message_box(d, "PERDU", "Dommage", "Il ne vous reste plus de vies. Au revoir !", "OK", ALLEGRO_MESSAGEBOX_QUESTION); // ALLEGRO_MESSAGEBOX_YES_NO, ALLEGRO_MESSAGEBOX_WARN, ALLEGRO_MESSAGEBOX_ERROR, ALLEGRO_MESSAGEBOX_QUESTION et ALLEGRO_MESSAGEBOX_OK_CANCEL
        return 2;
    }
}
void restart() {
    int retry=-1;
    POS tmpCoord;
    vies--;
    retry=perdu(vies);  // message de fin + choix
    score=0;
    temps=0;
    JUMP_FORCE=-25;
    GRAVITY=2;
    if(retry==2 || vies==0) {
        tracerAccueil(polices[1]);
        AGRANDI_FACT=1;
        num_map=0;
        base_sol=changeMap();
        perso = new User("stickman");
        perso->setPos(0,0);
        perso->setSpeed(0,0);
        music->stop();
        son_finish->stop();
        son_jump->stop();
        son_tuyau->stop();
        playSound=false;
        enter=false;
    }
    else {
        num_map=1;
        base_sol=changeMap();
        perso = new User("mario");
        perso->setPosX(blocs[entree].getCoord().x+blocs[entree].getW()/2-perso->getW()/2);
        perso->setPosY(blocs[entree].getCoord().y);
        perso->setSpeed(0,0);
        AGRANDI_FACT=0.7;
        anim_entree=true;
    }
    // cout << "newCoord=" << perso->getPos().x << "," << perso->getPos().y << endl;
    stop=true;
    key[KEY_RIGHT]=false;
    key[KEY_D]=false;
    key[KEY_LEFT]=false;
    key[KEY_Q]=false;
    key[KEY_DOWN]=false;
    key[KEY_S]=false;
    key[KEY_UP]=false;
    key[KEY_Z]=false;
    key[KEY_SPACE]=false;
    retry=-1;
    al_flush_event_queue(event_queue);
}
void finish() {
    al_clear_to_color(NOIR);
    al_draw_text(polices[1], ORANGE, window_width/2, window_height/2-20, ALLEGRO_ALIGN_CENTER, "BRAVO, VOUS AVEZ TERMINÉ LE JEU !");
    al_flip_display();
    ALLEGRO_DISPLAY* d = al_is_system_installed() ? al_get_current_display() : NULL;
    al_set_window_title(d, "FINISH");
    int result = al_show_native_message_box(d, "FELICITATIONS", "BRAVO", "Vous avez terminé avec succès le jeu Mario Bros !", "CONTINUER|QUITTER", ALLEGRO_MESSAGEBOX_YES_NO); // ALLEGRO_MESSAGEBOX_YES_NO, ALLEGRO_MESSAGEBOX_WARN, ALLEGRO_MESSAGEBOX_ERROR, ALLEGRO_MESSAGEBOX_QUESTION et ALLEGRO_MESSAGEBOX_OK_CANCEL
    if (result == 1) {
        tracerAccueil(polices[1]);
        AGRANDI_FACT=1;
        num_map=0;
        base_sol=changeMap();
        perso = new User("stickman");
        perso->setPos(0,0);
        perso->setSpeed(0,0);
        music->stop();
        son_finish->stop();
        son_jump->stop();
        son_tuyau->stop();
        playSound=false;
        enter=false;
        
        stop=true;
        key[KEY_RIGHT]=false;
        key[KEY_D]=false;
        key[KEY_LEFT]=false;
        key[KEY_Q]=false;
        key[KEY_DOWN]=false;
        key[KEY_S]=false;
        key[KEY_UP]=false;
        key[KEY_Z]=false;
        key[KEY_SPACE]=false;
        al_flush_event_queue(event_queue);
    }
    else {
        fin=true;
    }
}

// ---------------  COLLISIONS -------------------------//
float conv_to_Deg(float rad) {
    return rad*180/PI;
}
float calculateAngle(VECT2D vectorA, VECT2D vectorB) {
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
int collisionPersoBloc(User *perso, Bloc *bloc) 
{
    // Positions
    int px = perso->getPos().x;
    int py = perso->getPos().y;
    int bx = bloc->getCoord().x;
    int by = bloc->getCoord().y;

    // Tailles
    int pw = perso->getW();
    int ph = perso->getH();
    int bw = bloc->getW();
    int bh = bloc->getH();

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
            cout << "erreur perso/bloc" << endl;
            return FIN;
        }
    }
    else 
        return FIN; 
}
int collisionPersoMechant(User *perso, Mechant *mechant) 
{
    // Positions
    int px = perso->getPos().x;
    int py = perso->getPos().y;
    int mx = mechant->getPos().x;
    int my = mechant->getPos().y;

    // Tailles
    int pw = perso->getW();
    int ph = perso->getH();
    int mw = mechant->getW();
    int mh = mechant->getH();

    // Centre
    int mcx = mx+mw/2;
    int mcy = my+mh/2;
    int pcx = px+pw/2;
    int pcy = py+ph/2;

    // Distances
    int dcx = abs(mcx-pcx);
    int dcy = abs(mcy-pcy);

    VECT2D vect_bloc_perso = { (float) pcx-mcx , (float) pcy-mcy , 0};
    vect_bloc_perso.norme = sqrt( pow(vect_bloc_perso.x,2) + pow(vect_bloc_perso.y,2) );

    VECT2D vect_bloc = { (float) mcx+mw/2-mcx , (float) mcy-mcy , 1 };

    float angle = calculateAngle(vect_bloc_perso, vect_bloc);

    if(dcx<=(pw+mw)/2 && dcy<=(ph+mh)/2 && vect_bloc_perso.norme < sqrt( pow((pw+mw)/2,2) + pow((ph+mh)/2,2) ))  // Collision
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
            cout << "erreur perso/mechant" << endl;
            return FIN;
        }
    }
    else 
        return FIN;
}
int collision2Mechants(Mechant *mechant1, Mechant *mechant2) 
{
    // Positions
    int px = mechant1->getPos().x;
    int py = mechant1->getPos().y;
    int bx = mechant2->getPos().x;
    int by = mechant2->getPos().y;

    // Tailles
    int pw = mechant1->getW();
    int ph = mechant1->getH();
    int bw = mechant2->getW();
    int bh = mechant2->getH();

    // Centre
    int bcx = bx+bw/2;
    int bcy = by+bh/2;
    int pcx = px+pw/2;
    int pcy = py+ph/2;

    // Distances
    int dcx = abs(bcx-pcx);
    int dcy = abs(bcy-pcy);

    VECT2D vect_bloc_perso = { (float) pcx-bcx , (float) pcy-bcy , 0};
    vect_bloc_perso.norme = sqrt( pow(vect_bloc_perso.x,2) + pow(vect_bloc_perso.y,2) );

    VECT2D vect_bloc = { (float) bcx+bw/2-bcx , (float) bcy-bcy , 1 };

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
            // cout << "erreur 2 mechants" << endl;
            return FIN;
        }
    }
    else 
        return FIN; 
}
int collisionBlocMechant(Bloc *bloc, Mechant *mechant) 
{
    // Positions
    int px = mechant->getPos().x;
    int py = mechant->getPos().y;
    int bx = bloc->getCoord().x;
    int by = bloc->getCoord().y;

    // Tailles
    int pw = mechant->getW();
    int ph = mechant->getH();
    int bw = bloc->getW();
    int bh = bloc->getH();

    // Centre
    int bcx = bx+bw/2;
    int bcy = by+bh/2;
    int pcx = px+pw/2;
    int pcy = py+ph/2;

    // Distances
    int dcx = abs(bcx-pcx);
    int dcy = abs(bcy-pcy);

    VECT2D vect_bloc_perso = { (float) pcx-bcx , (float) pcy-bcy , 0};
    vect_bloc_perso.norme = sqrt( pow(vect_bloc_perso.x,2) + pow(vect_bloc_perso.y,2) );

    VECT2D vect_bloc = { (float) bcx+bw/2-bcx , (float) bcy-bcy , 1 };

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
            cout << "erreur mechant/bloc" << endl;
            return FIN;
        }
    }
    else 
        return FIN; 
}
int collisionObjetBloc(ObjectLance *objet, Bloc *bloc) 
{
    // Positions
    int px = objet->getPos().x;
    int py = objet->getPos().y;
    int bx = bloc->getCoord().x;
    int by = bloc->getCoord().y;

    // Tailles
    int pw = objet->getW();
    int ph = objet->getH();
    int bw = bloc->getW();
    int bh = bloc->getH();

    // Centre
    int bcx = bx+bw/2;
    int bcy = by+bh/2;
    int pcx = px+pw/2;
    int pcy = py+ph/2;

    // Distances
    int dcx = abs(bcx-pcx);
    int dcy = abs(bcy-pcy);

    if(dcx<=(/* pw+ */bw)/2 && dcy<=(ph+bh)/2)  // Collision
    {
        return 0;
    }
    return -1;
}
int collisionObjetMechant(ObjectLance *objet, Mechant *mechant) 
{
    // Positions
    int px = objet->getPos().x;
    int py = objet->getPos().y;
    int bx = mechant->getPos().x;
    int by = mechant->getPos().y;

    // Tailles
    int pw = objet->getW();
    int ph = objet->getH();
    int bw = mechant->getW();
    int bh = mechant->getH();

    // Centre
    int bcx = bx+bw/2;
    int bcy = by+bh/2;
    int pcx = px+pw/2;
    int pcy = py+ph/2;

    // Distances
    int dcx = abs(bcx-pcx);
    int dcy = abs(bcy-pcy);

    if(dcx<=(pw+bw)/2 && dcy<=(ph+bh)/2)  // Collision
    {
        return 0;
    }
    return -1;
}
void handleCollisions() 
{
    int i=0, j=0, k=0, nbrColl=0, dirCollision=0, tmpNbrObjets=0;
    ObjectLance *tmpObjets[MAX_OBJETS];
    POS tmpCoord;

    // Collisions perso/blocs
    if(anim_sortie==false && anim_entree==false && anim_fin==false) 
    { 
        for(i=0;i<blocs.size();i++)
        {
            dirCollision = collisionPersoBloc(perso , &blocs[i]);
            // afficheOrientation(dirCollision);
            if(dirCollision!=FIN && i<indice_sol+nbrBlocsSol) { // si collision avec le sol
                perso->setSpeedY(0);  // pas de déplacement en Y sauf si saut
                sol=base_sol;
                if(perso->getPos().y<window_height-sol-perso->getH() && sounds_on) 
                    son_sol->play();
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
                            AGRANDI_FACT=1;
                            GRAVITY=2;
                            perso->setMessage(">");
                            if(sounds_on) son_powerUp->play();
                            break;
                        case CHAMPI_GEANT :
                            AGRANDI_FACT=2;
                            GRAVITY=2;
                            if(sounds_on) son_powerUp->play();
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
                                if(sounds_on) son_coin->play();
                            }
                            break;
                        case FLEUR :
                            tmpCoord = perso->getPos();
                            perso = new User("marioFire");
                            perso->setPos(tmpCoord);
                            AGRANDI_FACT=1;
                            GRAVITY=2;
                            if(sounds_on) son_powerUp->play();
                            break;
                        case CHAMPI_MINI :
                            tmpCoord = perso->getPos();
                            if(perso->getNom().compare("marioFire")==0)
                                perso = new User("mario");
                            perso->setPos(tmpCoord);
                            AGRANDI_FACT=0.5;
                            GRAVITY=1;
                            if(sounds_on) son_powerUp->play();
                            break;
                        case CHAMPI_DOWN :
                            break;
                        case ETOILE :
                            break;
                    }
                }
            }
            else if(dirCollision!=FIN && blocs[i].getType()==DOOR_CLOSED) {
                if(key[KEY_UP]) {
                    perso->setSpeedY(0);
                    perso_num_img=NORD;
                    POS coordSortie = blocs[sortie].getCoord();
                    blocs[sortie] = Bloc("images/Door_open.png",0,0,ZERO,1,false,false,DOOR_OPEN);
                    blocs[sortie].setCoord(coordSortie);
                    perso->setPosX(blocs[sortie].getCoord().x+blocs[sortie].getW()/2-perso->getW()/2);
                    anim_fin=true;
                }
            }
            else if(dirCollision!=FIN && blocs[i].getType()==DOOR_OPEN) {
                if(key[KEY_UP]) {
                    al_rest(0.5);
                    num_map = num_map >= NB_MAPS-1 ? 1 : num_map+1;
                    base_sol=changeMap();
                }
            }
            else 
            {
                switch(dirCollision) 
                {
                    case SUD :
                        if(i==sortie && blocs[sortie].getType()==TUYAU && blocs[sortie].getAngle()==ZERO && baisse==true) { // sortie de map dans le tuyau
                            anim_sortie=true;
                        }
                        else {
                            if(!jump) 
                                perso->setSpeedY(0);
                            sol=window_height-blocs[i].getCoord().y-1;
                            if(perso->getPos().y<window_height-sol-perso->getH() && sounds_on) 
                                son_sol->play();
                            perso->setPosY(window_height-sol-perso->getH());
                            
                        }
                        break;

                    case EST :
                        if(i==sortie && blocs[sortie].getType()==TUYAU && (blocs[sortie].getAngle()==GAUCHE || blocs[sortie].getAngle()==DROITE) ) { // sortie de map dans le tuyau
                            anim_sortie=true;
                        }
                        else if(i==sortie && sortie !=0 && blocs[i].getType()==CHATEAU && perso->getNom()!="stickman" && perso->getPos().x+perso->getW()>blocs[sortie].getCoord().x && perso->getPos().y+perso->getH()>window_height-base_sol-5) { // sortie par le chateau
                            anim_fin=true;
                            JUMP_FORCE=-25;
                            GRAVITY=2;
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
                            if(sounds_on) son_mario->play();
                        }
                        else if(i==1+nbrBlocsSol && num_map==0) { 
                            perso = new User("luigi");
                            perso->setPos(tmpCoord);
                            if(sounds_on) son_luigi->play();
                        }
                        else if(blocs[i].getType()==MYSTERE && blocs[i].isHiding()==true && blocs[i-1].isObject()==true && blocs[i-1].is_enable()==false) { // tape le bloc mystère par en dessous
                            // masqueRGB(display, blocs[i].getImg(), false, true, true);
                            blocs[i]= Bloc("images/bloc_vide.png",blocs[i].getCoord().x,blocs[i].getCoord().y,ZERO,1,false,false,BLOC_VIDE);
                            blocs[i-1].enable();
                            blocs[i-1].setSortieObjet(true);
                            blocs[i-1].setCoord(blocs[i-1].getCoord().x , blocs[i].getCoord().y);
                            blocs[i].setHiding(false);
                        }
                        break;

                    default : // dirCollision = FIN
                        break;
                } // fin switch
            }
        } // fin du for
    }  // fin bool

// ------------------- MECHANTS ---------------------
    
    for(i=0;i<nbrMechants;i++) 
    {
        // Collisions perso/mechants
        if(retreci==false && grabObject==false) 
        {
            dirCollision = collisionPersoMechant(perso, mechants[i]);
            if(AGRANDI_FACT>1 && dirCollision!=FIN) {
                mechants[i]->setAfficher(false);
                mechants[i]->setPos(-50,-50);
            }
            else if(dirCollision == SUD && mechants[i]->getEcrase()==false) {
                mechants[i]->setEcrase(true);
                mechants[i]->setTaille(0.06);
                mechants[i]->actualiseSize(2);
                mechants[i]->setPos(mechants[i]->getPos().x,window_height-base_sol-mechants[i]->getH());
                perso->setSpeedY(JUMP_FORCE);
                if(sounds_on) {
                    if(mechants[i]->getNom().compare("koopa")==0)
                        son_koopa_shell->play();
                    else
                        son_ecrase->play();
                }
            }
            else if(dirCollision!=FIN) {
                if(mechants[i]->getNom().compare("koopa")==0 && mechants[i]->getEcrase()==true && mechants[i]->getSpeed().x==0 && key[KEY_C]==true) {
                    grabObject=true;
                }
                else if(dirCollision==SUD && mechants[i]->getNom().compare("koopa")==0 && mechants[i]->getEcrase()==true && key[KEY_C]==false) {
                    perso->setSpeedY(JUMP_FORCE);
                    if(mechants[i]->getSpeed().x!=0)
                        mechants[i]->setSpeedX(0);
                }
                else {
                    if(AGRANDI_FACT==1) {
                        if(perso->getNom().compare("marioFire")==0)
                            perso = new User("mario");
                        perso->setPosX(mechants[i]->getPos().x+mechants[i]->getW()+10);
                        if(sounds_on) son_powerDown->play();
                        AGRANDI_FACT=0.7;
                        retreci=true;
                    }
                    else {
                        anim_perteVie=true;
                        if(sounds_on) son_over->play();
                    }
                }
            }
        }

        // méchants aux limites fenêtres
        mechants[i]->actualisePos();
        if(mechants[i]->getPos().x<0) {
            mechants[i]->changeDir();
        }
        else if(mechants[i]->getPos().x+mechants[i]->getW()>window_width) {
            mechants[i]->changeDir();
        }
        if(mechants[i]->getPos().y+mechants[i]->getH()>window_height-base_sol)
            mechants[i]->setPos(mechants[i]->getPos().x,window_height-base_sol-mechants[i]->getH());

        
        // collisions mechant avec les blocs
        for (j = 0; j < blocs.size(); ++j)
        {
            if(blocs[j].isObject()==false) 
            {
                dirCollision=collisionBlocMechant(&blocs[j],mechants[i]);
                switch(dirCollision) 
                {
                    case SUD :
                        break;
                    case NORD :
                        break;
                    case EST :
                    case OUEST :
                        if(j>indice_sol+nbrBlocsSol && blocs[j].getType()!=DOOR_CLOSED && blocs[j].getType()!=DOOR_OPEN)
                            mechants[i]->changeDir();
                        break;
                    default:
                        break;
                }
            }
        }

        // collisions mechant entre eux
        for (j=0; j<nbrMechants; j++) 
        {
            if(j!=i) 
            {
                dirCollision=collision2Mechants(mechants[i],mechants[j]);
                switch(dirCollision)
                {
                    case SUD :
                        break;
                    case NORD :
                        break;
                    case EST :
                    case OUEST :
                        if(mechants[i]->getNom().compare("koopa")==0 && mechants[i]->getEcrase()==true) {
                            mechants[j]->setAfficher(false);
                            mechants[j]->setPos(-50,-50);
                        }
                        else if(mechants[j]->getNom().compare("koopa")==0 && mechants[j]->getEcrase()==true) {
                            mechants[i]->setAfficher(false);
                            mechants[i]->setPos(-50,-50);
                        }
                        else {
                            mechants[i]->changeDir();
                            mechants[j]->changeDir();
                        }
                        break;
                    default:
                        break;
                }
            }
        }
    }

// ------------------- OBJETS ---------------------
    
    tmpNbrObjets = nbrObjets;
    for ( i = 0; i < nbrObjets; ++i)
    {
        if(lanceObjet) 
        {
            // Actualise Objets aux limites (type boule de feu)
            if(objets[i]->getPos().x<window_width && objets[i]->getPos().x+objets[i]->getW()>0 && objets[i]->getPos().y+objets[i]->getH()<window_height-base_sol) { // Objet dans la fenêtre
                objets[i]->setPos(objets[i]->getPos().x+objets[i]->getSpeed().x, objets[i]->getPos().y+objets[i]->getSpeed().y);
            }
            else { // Objet aux limites
                for (j = 0; j < MAX_OBJETS; ++j)  // Copy la liste
                    tmpObjets[j]=objets[j];
                for (j = i; j < nbrObjets-1; ++j) // Enlève l'indice i et déplacement de la liste pour supprimer que la boule qui sort
                    objets[j]=tmpObjets[j+1];
                tmpNbrObjets--;
            }

            // collisions objets avec les blocs
            for (j = 0; j < blocs.size(); ++j)
            {
                dirCollision=collisionObjetBloc(objets[i],&blocs[j]);
                if(dirCollision==0 && blocs[j].getType()!=COIN && blocs[j].isObject()==false && blocs[j].getType()!=DOOR_CLOSED) 
                {
                    coordCol = (POS) {  objets[i]->getPos().x , objets[i]->getPos().y };
                    objetsCol[i]=true;
                    for (k = 0; k < MAX_OBJETS; ++k) // copy la liste
                        tmpObjets[k]=objets[k];
                    for (k = i; k < tmpNbrObjets-1; ++k) // enlève l'indice i et déplacement de la liste pour supprimer que la boule qui sort
                        objets[k]=tmpObjets[k+1];
                    tmpNbrObjets--;
                }
            }
            if(tmpNbrObjets<0)
                cout << "erreur" << endl;

            // collisions objets avec les mechants
            for (j = 0; j < nbrMechants; ++j)
            {
                dirCollision=collisionObjetMechant(objets[i],mechants[j]);
                if(dirCollision==0) 
                {
                    for (k = 0; k < MAX_OBJETS; ++k) // copy la liste
                        tmpObjets[k]=objets[k];
                    for (k = i; k < tmpNbrObjets-1; ++k) // enlève l'indice i et déplacement de la liste pour supprimer que la boule qui sort
                        objets[k]=tmpObjets[k+1];
                    tmpNbrObjets--;

                    blocs.push_back((Bloc("images/coin2.png",mechants[j]->getPos().x,mechants[j]->getPos().y,ZERO,0.3,true,false,COIN)));
                    if(sounds_on) son_fireBallHit->play();

                    mechants[j]->setAfficher(false);
                    mechants[j]->setPos(-50,-50);
                }
            }
        }
        else {
            objets[i]->setPos(perso->getPos().x+VIT_OBJ,perso->getPos().y+perso->getH()/2);
        }
    }
    nbrObjets=tmpNbrObjets;
}

// ---------------  MAPS -------------------------//
int changeMap() 
{
    int base_sol=0;
    if(num_map>0) {
        maps[num_map]->setMap0(false);
        maps[num_map]->setBackgroundScale(2);
    }
    switch(num_map) 
    {
        case 0 : 
            base_sol=createMap0();
            break;
        case 1 : 
            base_sol=createMap1();
            break;
        case 2 : 
            base_sol=createMap2();
            break;
        case 3 : 
            base_sol=createMap3();
            break;
        case 4 : 
            base_sol=createMap4();
            break;
        case 5 : 
            base_sol=createMap5();
            break;
        default: 
            base_sol=createMap1();
            break;
    }
    maps[num_map-1]->setBackgroundX(0);
    return base_sol;
}
int createMap0() 
{
    int Le,He;
    POS newCoord;
    sol=60;
    nbrMechants=0;

    blocs.clear(); // efface tous les blocs de la map précédente

    // SOL
    blocs.push_back(Bloc("images/terre.png",0,0,ZERO,0.5*RATIO_FRAME,false,false,TERRE));
        sol=blocs.at(0).getH();
    blocs.pop_back();
    blocs.push_back(Bloc("images/terre.png",0,window_height-sol,ZERO,0.5*RATIO_FRAME,false,false,TERRE));

    while( blocs.back().getCoord().x + blocs.back().getW() < window_width )
        blocs.push_back(Bloc("images/terre.png",blocs.back().getCoord().x+blocs.back().getW(),window_height-sol,ZERO,0.5*RATIO_FRAME,false,false,TERRE));

    nbrBlocsSol = blocs.size();

    blocs.push_back((Bloc("images/logo_mario2.png",window_width/2-150,window_height-sol-150,ZERO,0.1*RATIO_FRAME,false,false,PERSO)));
    blocs.push_back((Bloc("images/logo_luigi2.png",window_width/2+150-blocs.back().getW(),blocs.back().getCoord().y,ZERO,0.1*RATIO_FRAME,false,false,PERSO)));

// --- CHATEAU --------
    blocs.push_back((Bloc("images/chateau_bas.png",0,0,ZERO,0.2*RATIO_FRAME,false,false,CHATEAU)));
        newCoord.x = window_width-blocs.back().getW(); 
        newCoord.y = window_height-sol-blocs.back().getH();
        blocs.back().setCoord(newCoord);
    blocs.push_back((Bloc("images/chateau_haut.png",0,0,ZERO,0.2*RATIO_FRAME,false,false,CHATEAU)));
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
int createMap1() 
{
    int tmpH;
    POS newCoord;
    nbrMechants=0;
    int diffBords=0;

    blocs.clear(); // efface tous les blocs de la map précédente

    // -------- SOL ---------------

    blocs.push_back(Bloc("images/sol_terre.png",0,0,ZERO,1,false,false,TERRE));
        const int sol = blocs.back().getH();
        blocs.back().setCoord( (POS) { 0, window_height-sol } );

    while( blocs.back().getCoord().x + blocs.back().getW() < maps[num_map]->getW()*maps[num_map]->getBackgroundScale() )
    {
        blocs.push_back(Bloc("images/sol_terre.png",blocs.back().getCoord().x+blocs.back().getW(),window_height-sol,ZERO,1*RATIO_FRAME,false,false,TERRE));
    }
    
    nbrBlocsSol = blocs.size();

    // -------- JEU ---------------
    
    blocs.push_back(Bloc("images/bloc.png",350,window_height-sol-150,ZERO,1*RATIO_FRAME,false,false,BLOC));
    blocs.push_back(Bloc("images/bloc_mystere.png",blocs.back().getCoord().x+blocs.back().getW(),blocs.back().getCoord().y,ZERO,1*RATIO_FRAME,false,true,MYSTERE));
    blocs.push_back(Bloc("images/coin2.png",0,0,ZERO,0.3*RATIO_FRAME,true,false,COIN)); 
        newCoord.x = blocs[blocs.size()-2].getCoord().x+blocs[blocs.size()-2].getW()/2-blocs.back().getW()/2;
        newCoord.y = blocs[blocs.size()-2].getCoord().y-blocs.back().getH()-2;
        blocs.back().setCoord(newCoord);
        blocs.back().disable();
        Bloc tmpBloc=blocs[blocs.size()-2];
        blocs[blocs.size()-2]=blocs.back();
        blocs.back()=tmpBloc;
    blocs.push_back(Bloc("images/bloc.png",blocs.back().getCoord().x+blocs.back().getW(),blocs.back().getCoord().y-blocs.back().getH()+5,ZERO,1*RATIO_FRAME,false,false,BLOC));
    blocs.push_back(Bloc("images/bloc_mystere.png",blocs.back().getCoord().x+blocs.back().getW()+200,blocs.back().getCoord().y-20,ZERO,1*RATIO_FRAME,false,true,MYSTERE));
    blocs.push_back(Bloc("images/fleur.png",0,0,ZERO,0.4*RATIO_FRAME,true,false,FLEUR));
        newCoord.x = blocs[blocs.size()-2].getCoord().x+blocs[blocs.size()-2].getW()/2-blocs.back().getW()/2;
        newCoord.y = blocs[blocs.size()-2].getCoord().y-blocs.back().getH()-2;
        blocs.back().setCoord(newCoord);
        blocs.back().disable();
        tmpBloc=blocs[blocs.size()-2];
        blocs[blocs.size()-2]=blocs.back();
        blocs.back()=tmpBloc;
    blocs.push_back(Bloc("images/coin2.png",0,0,ZERO,0.3*RATIO_FRAME,true,false,COIN)); 
        newCoord.x = 300;
        newCoord.y = window_height-sol-100;
        blocs.back().setCoord(newCoord);
    blocs.push_back(Bloc("images/coin2.png",0,0,ZERO,0.3*RATIO_FRAME,true,false,COIN)); 
        newCoord.x = 600;
        newCoord.y = window_height-sol-150;
        blocs.back().setCoord(newCoord);

// -------- ENTREE ---------------

    blocs.push_back(Bloc("images/tuyau_haut.png",0,0,INVERSION,1*RATIO_FRAME,false,false,TUYAU));
        diffBords = blocs.back().getW();
    blocs.push_back(Bloc("images/tuyau_bas.png",0,0,INVERSION,1*RATIO_FRAME,false,false,TUYAU));
        diffBords -= blocs.back().getW();
        diffBords /=2;
        blocs.back().setCoord( (POS) { diffBords, HAUTEUR_TEXTE} );
    blocs.push_back(Bloc("images/tuyau_haut.png",0,0,INVERSION,1*RATIO_FRAME,false,false,TUYAU));
        blocs.back().setCoord( (POS) { 0, blocs[blocs.size()-2].getCoord().y+blocs[blocs.size()-2].getH() } );
        entree = blocs.size()-1;

// -------- SORTIE ---------------

    blocs.push_back(Bloc("images/tuyau_haut.png",0,0,ZERO,1*RATIO_FRAME,false,false,TUYAU));
        diffBords = blocs.back().getW();
    blocs.push_back(Bloc("images/tuyau_bas.png",0,0,GAUCHE,1*RATIO_FRAME,false,false,TUYAU));
        diffBords -= blocs.back().getW();
        diffBords /=2;
        tmpH=blocs.back().getH(); blocs.back().setH(blocs.back().getW()); blocs.back().setW(tmpH);
        blocs.back().setCoord( (POS) { (int) (maps[num_map]->getW() - blocs.back().getW()) , window_height-blocs.back().getH()-sol } );
    blocs.push_back(Bloc("images/tuyau_haut.png",0,0,GAUCHE,1*RATIO_FRAME,false,false,TUYAU));
        tmpH=blocs.back().getH(); blocs.back().setH(blocs.back().getW()); blocs.back().setW(tmpH);
        blocs.back().setCoord( (POS) { (int) (maps[num_map]->getW() - blocs[blocs.size()-2].getW()-blocs.back().getW())+20 , window_height-sol-blocs.back().getH()+diffBords } );
        sortie = blocs.size()-1;

//--------------- MECHANTS -----------------
/* 
    mechants[nbrMechants] = new Mechant("goomba");
        mechants[nbrMechants]->setTaille(0.06*RATIO_FRAME);
        mechants[nbrMechants]->actualiseSize(0);
        mechants[nbrMechants]->setSpeedX(-3);
        mechants[nbrMechants]->setPos(200,window_height-sol-mechants[nbrMechants]->getH()-2);
        nbrMechants++;

    mechants[nbrMechants] = new Mechant("koopa");
        mechants[nbrMechants]->setTaille(0.03*RATIO_FRAME);
        mechants[nbrMechants]->actualiseSize(0);
        mechants[nbrMechants]->setSpeedX(-3);
        mechants[nbrMechants]->setPos(500,window_height-sol-mechants[nbrMechants]->getH());
        nbrMechants++;
 */
    return sol;
}
int createMap2() 
{
    int tmpH, trou=0, diffBords=0;
    nbrMechants=0;

    blocs.clear(); // efface tous les blocs de la map précédente

    // -------- SOL ---------------

    blocs.push_back(Bloc("images/sol_terre.png",0,0,ZERO,1,false,false,TERRE));
        const int base_sol = blocs.back().getH();
        blocs.back().setCoord( (POS) { 0, window_height-base_sol } );
        sol = base_sol+1;

    while( blocs.back().getCoord().x + blocs.back().getW() < maps[num_map]->getW()*maps[num_map]->getBackgroundScale() )
    {
        if(blocs.size() == 2) 
            trou = 500;
        else 
            trou = 0;

        blocs.push_back(Bloc("images/sol_terre.png",blocs.back().getCoord().x+blocs.back().getW()+trou,window_height-sol,ZERO,1*RATIO_FRAME,false,false,TERRE));
    }      
    nbrBlocsSol = blocs.size();

    // ----------- JEU ---------------

    blocs.push_back(Bloc("images/bloc.png",350,window_height-sol-250,ZERO,1,false,false,BLOC));
    blocs.push_back(Bloc("images/bloc.png",blocs.back().getCoord().x+blocs.back().getW()+200,blocs.back().getCoord().y,ZERO,1,false,false,BLOC));

    // -------- SORTIE ---------------

    blocs.push_back(Bloc("images/tuyau_haut.png",0,0,ZERO,1*RATIO_FRAME,false,false,TUYAU));
        diffBords = blocs.back().getW();
    blocs.push_back(Bloc("images/tuyau_bas.png",0,0,ZERO,1*RATIO_FRAME,false,false,TUYAU));
        diffBords -= blocs.back().getW();
        diffBords /=2;
        blocs.back().setCoord( (POS) { (int) (maps[num_map]->getW() - blocs[blocs.size()-2].getW() - blocs.back().getW())-diffBords, window_height-sol-blocs.back().getH()} );
    blocs.push_back(Bloc("images/tuyau_haut.png",0,0,ZERO,1*RATIO_FRAME,false,false,TUYAU));
        blocs.back().setCoord( (POS) { blocs[blocs.size()-2].getCoord().x - diffBords, blocs[blocs.size()-2].getCoord().y-blocs.back().getH() } );
        sortie = blocs.size()-1;

    // -------- ENTREE ---------------
    
    blocs.push_back(Bloc("images/tuyau.png",0,0,ZERO,1,false,false,TUYAU));
        blocs.back().setCoord( (POS) { 0, window_height-sol-blocs.back().getH() } );
        entree=blocs.size()-1;

    return base_sol;
}
int createMap3() 
{
    int Le,He, diffBords=0;
    POS newCoord;
    sol=60;
    nbrMechants=0;

    blocs.clear(); // efface tous les blocs de la map précédente

    // -------- SOL ---------------

    blocs.push_back(Bloc("images/terre.png",0,0,ZERO,0.5,false,false,TERRE));
        const int sol = blocs.back().getH();
        blocs.back().setCoord( (POS) { 0, window_height-sol } );
    
    while( blocs.back().getCoord().x + blocs.back().getW() < maps[num_map]->getW()*maps[num_map]->getBackgroundScale() )
    {
        blocs.push_back(Bloc("images/terre.png",blocs.back().getCoord().x+blocs.back().getW(),window_height-sol,ZERO,0.5*RATIO_FRAME,false,false,TERRE));
    }
    nbrBlocsSol = blocs.size();

    // ----------- JEU --------------

    blocs.push_back(Bloc("images/bloc_mystere.png",window_width/2,window_height-sol-150,ZERO,1,false,true,MYSTERE));
    blocs.push_back(Bloc("images/champi.png",0,0,ZERO,1,true,false,CHAMPI));
        newCoord.x = blocs[blocs.size()-2].getCoord().x+blocs[blocs.size()-2].getW()/2-blocs.back().getW()/2;
        newCoord.y = blocs[blocs.size()-2].getCoord().y-blocs.back().getH()-5;
        blocs.back().setCoord(newCoord);
        blocs.back().disable();
        Bloc tmpBloc=blocs[blocs.size()-2];
        blocs[blocs.size()-2]=blocs.back();
        blocs.back()=tmpBloc;

    // -------- SORTIE ---------------

    blocs.push_back(Bloc("images/Door_closed.png",0,0,ZERO,1,false,false,DOOR_CLOSED));
        newCoord.x = (int) ( maps[num_map]->getW() - blocs.back().getW() ) ;
        newCoord.y = window_height-sol-blocs.back().getH();
        blocs.back().setCoord(newCoord);
        sortie=blocs.size()-1;

    // -------- ENTREE ---------------

    blocs.push_back(Bloc("images/tuyau_haut.png",0,0,INVERSION,1*RATIO_FRAME,false,false,TUYAU));
        diffBords = blocs.back().getW();
    blocs.push_back(Bloc("images/tuyau_bas.png",0,0,INVERSION,1*RATIO_FRAME,false,false,TUYAU));
        diffBords -= blocs.back().getW();
        diffBords /=2;
        blocs.back().setCoord( (POS) { diffBords, HAUTEUR_TEXTE} );
    blocs.push_back(Bloc("images/tuyau_haut.png",0,0,INVERSION,1*RATIO_FRAME,false,false,TUYAU));
        blocs.back().setCoord( (POS) { 0, blocs[blocs.size()-2].getCoord().y+blocs[blocs.size()-2].getH() } );
        entree = blocs.size()-1;

    //--------- MECHANTS -------------

    mechants[nbrMechants] = new Mechant("goomba");
        mechants[nbrMechants]->setTaille(0.06);
        mechants[nbrMechants]->actualiseSize(0);
        mechants[nbrMechants]->setSpeedX(3);
        mechants[nbrMechants]->setVersDroite(true);
        mechants[nbrMechants]->setPos(window_width/4,window_height-sol-mechants[nbrMechants]->getH()-2);
        nbrMechants++;

    mechants[nbrMechants] = new Mechant("goomba");
        mechants[nbrMechants]->setTaille(0.06);
        mechants[nbrMechants]->actualiseSize(0);
        mechants[nbrMechants]->setSpeedX(-3);
        mechants[nbrMechants]->setPos(3*window_width/4,window_height-sol-mechants[nbrMechants]->getH()-2);
        nbrMechants++;

    mechants[nbrMechants] = new Mechant("koopa");
        mechants[nbrMechants]->setTaille(0.03);
        mechants[nbrMechants]->actualiseSize(0);
        mechants[nbrMechants]->setSpeedX(-3);
        mechants[nbrMechants]->setPos(window_width/2,window_height-sol-mechants[nbrMechants]->getH());
        nbrMechants++;

    return sol;
}
int createMap4() 
{
    int Le,He, diffBords=0;
    POS newCoord;
    sol=60;
    nbrMechants=0;

    blocs.clear(); // efface tous les blocs de la map précédente

    // -------- SOL ---------------

    blocs.push_back(Bloc("images/terre.png",0,0,ZERO,0.5,false,false,TERRE));
        const int sol = blocs.back().getH();
        blocs.back().setCoord( (POS) { 0, window_height-sol } );
    
    while( blocs.back().getCoord().x + blocs.back().getW() < maps[num_map]->getW()*maps[num_map]->getBackgroundScale() )
    {
        blocs.push_back(Bloc("images/terre.png",blocs.back().getCoord().x+blocs.back().getW(),window_height-sol,ZERO,0.5*RATIO_FRAME,false,false,TERRE));
    }
    nbrBlocsSol = blocs.size();

    // ----------- JEU --------------

    blocs.push_back(Bloc("images/bloc_mystere.png",window_width/2,window_height-sol-150,ZERO,1,false,true,MYSTERE));
    blocs.push_back(Bloc("images/Down_Mushroom.png",0,0,ZERO,0.05,true,false,CHAMPI_DOWN));
        newCoord.x = blocs[blocs.size()-2].getCoord().x+blocs[blocs.size()-2].getW()/2-blocs.back().getW()/2;
        newCoord.y = blocs[blocs.size()-2].getCoord().y-blocs.back().getH()-5;
        blocs.back().setCoord(newCoord);
        blocs.back().disable();
        Bloc tmpBloc=blocs[blocs.size()-2];
        blocs[blocs.size()-2]=blocs.back();
        blocs.back()=tmpBloc;

    // -------- SORTIE ---------------

    blocs.push_back(Bloc("images/Door_closed.png",0,0,ZERO,1,false,false,DOOR_CLOSED));
        newCoord.x = (int) ( maps[num_map]->getW() - blocs.back().getW() ) ;
        newCoord.y = window_height-sol-blocs.back().getH();
        blocs.back().setCoord(newCoord);
        sortie=blocs.size()-1;

    // -------- ENTREE ---------------

    blocs.push_back(Bloc("images/tuyau_haut.png",0,0,INVERSION,1*RATIO_FRAME,false,false,TUYAU));
        diffBords = blocs.back().getW();
    blocs.push_back(Bloc("images/tuyau_bas.png",0,0,INVERSION,1*RATIO_FRAME,false,false,TUYAU));
        diffBords -= blocs.back().getW();
        diffBords /=2;
        blocs.back().setCoord( (POS) { diffBords, HAUTEUR_TEXTE} );
    blocs.push_back(Bloc("images/tuyau_haut.png",0,0,INVERSION,1*RATIO_FRAME,false,false,TUYAU));
        blocs.back().setCoord( (POS) { 0, blocs[blocs.size()-2].getCoord().y+blocs[blocs.size()-2].getH() } );
        entree = blocs.size()-1;

    return sol;
}
int createMap5() 
{
    int Le,He, diffBords=0;
    POS newCoord;
    sol=60;
    nbrMechants=0;

    blocs.clear(); // efface tous les blocs de la map précédente

    // -------- SOL ---------------

    blocs.push_back(Bloc("images/terre.png",0,0,ZERO,0.5,false,false,TERRE));
        const int sol = blocs.back().getH();
        blocs.back().setCoord( (POS) { 0, window_height-sol } );
    
    while( blocs.back().getCoord().x + blocs.back().getW() < maps[num_map]->getW()*maps[num_map]->getBackgroundScale() )
    {
        blocs.push_back(Bloc("images/terre.png",blocs.back().getCoord().x+blocs.back().getW(),window_height-sol,ZERO,0.5*RATIO_FRAME,false,false,TERRE));
    }
    nbrBlocsSol = blocs.size();

    // ----------- JEU --------------

    blocs.push_back(Bloc("images/bloc_mystere.png",window_width/2,window_height-sol-150,ZERO,1,false,true,MYSTERE));
    blocs.push_back(Bloc("images/etoile.png",0,0,ZERO,0.3,true,false,ETOILE));
        newCoord.x = blocs[blocs.size()-2].getCoord().x+blocs[blocs.size()-2].getW()/2-blocs.back().getW()/2;
        newCoord.y = blocs[blocs.size()-2].getCoord().y-blocs.back().getH()-5;
        blocs.back().setCoord(newCoord);
        blocs.back().disable();
        Bloc tmpBloc=blocs[blocs.size()-2];
        blocs[blocs.size()-2]=blocs.back();
        blocs.back()=tmpBloc;

    // -------- SORTIE ---------------

    blocs.push_back(Bloc("images/Door_closed.png",0,0,ZERO,1,false,false,DOOR_CLOSED));
        newCoord.x = (int) ( maps[num_map]->getW() - blocs.back().getW() ) ;
        newCoord.y = window_height-sol-blocs.back().getH();
        blocs.back().setCoord(newCoord);
        sortie=blocs.size()-1;

    // -------- ENTREE ---------------

    blocs.push_back(Bloc("images/tuyau_haut.png",0,0,INVERSION,1*RATIO_FRAME,false,false,TUYAU));
        diffBords = blocs.back().getW();
    blocs.push_back(Bloc("images/tuyau_bas.png",0,0,INVERSION,1*RATIO_FRAME,false,false,TUYAU));
        diffBords -= blocs.back().getW();
        diffBords /= 2;
        blocs.back().setCoord( (POS) { diffBords, HAUTEUR_TEXTE} );
    blocs.push_back(Bloc("images/tuyau_haut.png",0,0,INVERSION,1*RATIO_FRAME,false,false,TUYAU));
        blocs.back().setCoord( (POS) { 0, blocs[blocs.size()-2].getCoord().y+blocs[blocs.size()-2].getH() } );
        entree = blocs.size()-1;

    return sol;
}

// -----------------------------------------  HACHAGE ----------------------------------------------------
int hachage(const char *chaine)
{
    int i = 0, nombreHache = 0;
    for (i = 0 ; chaine[i] != '\0' ; i++)
        nombreHache += chaine[i];
    nombreHache %= 100;
    return nombreHache;
}

// Fonction de hachage pour générer l'index à partir de la clé
int hashe(const char* key) 
{
    int hash = 0;
    for (int i = 0; i < strlen(key); i++)
        hash += key[i];
    return hash % TABLE_SIZE;
}

// Fonction pour ajouter une paire clé-valeur à la table de hachage
void insert(HashTable* table, const char* key, int value) 
{
    int index = hashe(key);
    strcpy(table->data[index].key, key);
    table->data[index].value = value;
}

// Fonction pour récupérer la valeur associée à une clé
int get(HashTable* table, const char* key) 
{
    int index = hashe(key);
    return table->data[index].value;
}

/*
// Main
int main() {
    HashTable table;

    // Ajouter des paires clé-valeur à la table
    insert(&table, "apple", 1);
    insert(&table, "banana", 2);
    insert(&table, "orange", 3);

    // Récupérer les valeurs à partir des clés
    printf("Value for 'apple': %d\n", get(&table, "apple"));
    printf("Value for 'banana': %d\n", get(&table, "banana"));
    printf("Value for 'orange': %d\n", get(&table, "orange"));

    return 0;
}
*/