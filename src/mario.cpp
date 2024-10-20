
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
    // Initialiser Allegro
    if (!al_init()) {
        fprintf(stderr, "Erreur: Impossible d'initialiser Allegro.\n");
        return -1;
    }

    // Initialiser l'addon pour les images
    if (!al_init_image_addon()) {
        fprintf(stderr, "Erreur: Impossible d'initialiser l'addon image.\n");
        return -1;
    }

    // Initialiser l'addon pour les primitives (formes géométriques)
    if (!al_init_primitives_addon()) {
        fprintf(stderr, "Erreur: Impossible d'initialiser l'addon des primitives.\n");
        return -1;
    }

    // Installer le clavier
    if (!al_install_keyboard()) {
        fprintf(stderr, "Erreur: Impossible d'installer le clavier.\n");
        return -1;
    }

    // Installer la souris
    if (!al_install_mouse()) {
        fprintf(stderr, "Erreur: Impossible d'installer la souris.\n");
        return -1;
    }

    // Initialiser l'addon des polices de caractère
    if (!al_init_font_addon()) {
        fprintf(stderr, "Erreur: Impossible d'initialiser l'addon de police.\n");
        return -1;
    }

    // Initialiser l'addon des polices TrueType
    if (!al_init_ttf_addon()) {
        fprintf(stderr, "Erreur: Impossible d'initialiser l'addon TTF.\n");
        return -1;
    }

    // Installer le système audio
    if (!al_install_audio()) {
        fprintf(stderr, "Erreur: Impossible d'installer le système audio.\n");
        return -1;
    }

    // Réserver un nombre minimum d'échantillons audio (ici 1)
    if (!al_reserve_samples(1)) {
        fprintf(stderr, "Erreur: Impossible de réserver des échantillons audio.\n");
        return -1;
    }

    // Initialiser l'addon pour les codecs audio
    if (!al_init_acodec_addon()) {
        fprintf(stderr, "Erreur: Impossible d'initialiser l'addon des codecs audio.\n");
        return -1;
    }

    return 0;
}

int main(int argc, char **argv)
{
    if(init()==-1) { // initialisation des librairies Allegro
        return -1;
    }
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue=NULL;
    {
        bool dessine = false;
        ALLEGRO_EVENT ev;
        ALLEGRO_KEYBOARD_STATE keyboard_state;
        ALLEGRO_MOUSE_STATE mouse_state;

        // printAllegroVersion(); // check Allegro version

        // Déclaration du jeu
        shared_ptr<Game> jeu = make_shared<Game>();
        jeu->setPerso("stickman");
        
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
        al_register_event_source(event_queue, al_get_display_event_source(jeu->getDisplay()));

        // Défini la cible de rendu
        al_set_target_backbuffer(jeu->getDisplay());

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

            if(dessine && !jeu->isGameOver()) {

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
        
    }
    try {
        al_destroy_timer(timer);
        al_destroy_event_queue(event_queue);
        
        // Désinstaller les addons dans l'ordre inverse de l'installation
        al_shutdown_ttf_addon();        // Désinstaller l'addon TTF
        al_shutdown_font_addon();       // Désinstaller l'addon des polices
        al_shutdown_image_addon();      // Désinstaller l'addon des images
        al_uninstall_audio();           // Désinstaller le système audio
        al_uninstall_keyboard();
        al_uninstall_mouse();
        al_uninstall_system();          // Désinstaller Allegro (libère les primitives, clavier, souris, etc.)
    }
    catch(exception const& e)	 //On rattrape les exceptions standard de tous types
    {
        cerr << "ERREUR : " << e.what() << endl; //On affiche la description de l'erreur
    }
    

    return 0;
}