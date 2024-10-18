
#include "../include/mario.hpp"

/* 

Compilation+Execution : 
-----------------------
    ./compileExecute.sh

Ameliorations :
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
    - passage en full C++
    - sounds_on(false) -> true
    - créer une classe pour le jeu

*/

void printAllegroVersion() {
    // Get the Allegro version
    int version = ALLEGRO_VERSION_INT;

    // Extract the different parts of the version
    int major = (version >> 24) & 0xFF;
    int minor = (version >> 16) & 0xFF;
    int revision = (version >> 8) & 0xFF;
    int release = version & 0xFF;

    // Print the version
    cout << "Allegro version: " << major << "." << minor << "." << revision << "." << release << endl;
}
int init(void) {
    try 
    {
        al_init();
        al_init_image_addon();
        al_init_primitives_addon();
        al_install_keyboard();
        al_install_mouse();
        al_init_font_addon();
        al_init_ttf_addon();
        al_install_audio();
        al_reserve_samples(1);
        al_init_acodec_addon();
        // al_init_video_addon();
    }
    catch(exception const& e)	 //On rattrape les exceptions standard de tous types
    {
        cerr << "ERREUR : " << e.what() << endl; //On affiche la description de l'erreur
    }
    return 0;
}

int main(int argc, char **argv)
{
    init(); // initialisation des librairies Allegro

    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_EVENT ev;
    ALLEGRO_KEYBOARD_STATE keyboard_state;
    ALLEGRO_MOUSE_STATE mouse_state;
    ALLEGRO_EVENT_QUEUE *event_queue=NULL;

    bool dessine = false;

    printAllegroVersion();

    // Création de la fenêtre
    al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE);  // ALLEGRO_FULLSCREEN_WINDOW
    display = al_create_display(WIDTH, HEIGHT);
    assert(display != 0) ;
    al_set_window_title(display, "Mario Game");
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA); // effet de transparence
    al_clear_to_color(NOIR); // Clear

    // Déclaration du jeu
    Game *jeu = new Game(display);
    jeu->setPerso("stickman");

    // Obtient position fenetre
    int wx=0, wy=0;
    al_get_window_position(display, &wx, &wy);

    // Création de la file d'attente d'événements
    event_queue = al_create_event_queue();
    assert(event_queue != 0) ;

    // Création d'un timer
    timer = al_create_timer(TIME_STEP);
    assert(timer != 0) ;

    // Liste des types d'événements
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_display_event_source(display));

    // Défini la cible de rendu
    al_set_target_backbuffer(display);

    // Début du timer
    al_start_timer(timer);

    while(!jeu->isGameOver())
    {
        al_wait_for_event(event_queue, &ev); // attente d'un des événements
        switch (ev.type)
        {
            case ALLEGRO_EVENT_DISPLAY_RESIZE:
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE: // clique sur la croix rouge
                jeu->setGameOver(true);
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                jeu->handleKeyPressed(ev.keyboard.keycode, event_queue);
                break;

            case ALLEGRO_EVENT_KEY_UP:
                jeu->handleKeyUnPressed(ev.keyboard.keycode, event_queue);
                break;

            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                if(ev.mouse.button==1) {
                    jeu->actionOnMenu();
                    while(!al_is_event_queue_empty(event_queue))
                        al_flush_event_queue(event_queue);
                    //fprintf(stdout, "Vous avez cliqué à gauche avec la souris.\n");
                } else if(ev.mouse.button==2) {
                    //fprintf(stdout, "Vous avez cliqué à droite avec la souris.\n");
                }
                break;

            case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
                //fprintf(stdout, "Vous avez relâché la souris. Mouse button %d up at (%d, %d)\n", ev.mouse.button, ev.mouse.x, ev.mouse.y);
                break;

            case ALLEGRO_EVENT_MOUSE_AXES:
                if(jeu->getMenuSelected()) {
                    al_get_mouse_state(&mouse_state);
                    if(abs(jeu->getMousePos().x-mouse_state.x)>10 || abs(jeu->getMousePos().y-mouse_state.y)>10)
                    {
                        jeu->setMousePos(mouse_state.x , mouse_state.y);
                        jeu->checkMouseOnButton();
                    }
                }
                break;

            case ALLEGRO_EVENT_TIMER :
                if(!jeu->getMenuSelected()) // si pas de Menu on update sinon reste figé
                    jeu->update();
                dessine = true;
                break;

            default:
                break;
        }

        if(dessine==true && al_is_event_queue_empty(event_queue) && !jeu->isGameOver()){

            // Clear
            al_clear_to_color(NOIR);

            // Dessiner
            if(jeu->getStarted())   // Si le jeu est commencé
                jeu->dessine();        // Dessine le jeu (maps, blocs, perso, ...)
            else
                jeu->tracerAccueil();  // dessine Accueil

            if(jeu->getMenuSelected()) // si le menu est activé
                jeu->dessineMenu();    // dessine Menu

            // Actualise affichage
            al_flip_display();

            dessine=false;
        }
    }
    try {
        delete(jeu);
        al_destroy_timer(timer);
        al_destroy_event_queue(event_queue);
        al_destroy_display(display);
        al_uninstall_audio();
        al_uninstall_keyboard();
        al_uninstall_mouse();
        al_uninstall_system();
    }
    catch(exception const& e)	 //On rattrape les exceptions standard de tous types
    {
        cerr << "ERREUR : " << e.what() << endl; //On affiche la description de l'erreur
    }
    

    return 0;
}