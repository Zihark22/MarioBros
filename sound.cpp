#include "sound.hpp"

Sound::Sound() // constructeur classique
{
    this->chemin="";
    this->speed=0;
    this->gain=0;
    this->played=false;
    this->playmode=ONCE;
    spl=NULL;
    son=NULL;
}
Sound::Sound(const char* newChemin, ALLEGRO_MIXER *mixer) 
{
    bool rc;
    this->chemin=newChemin;
    this->speed=1;
    this->gain=1;
    this->played=false;
    this->playmode=ONCE;

    this->spl = al_create_sample_instance(NULL);
    al_attach_sample_instance_to_mixer(this->spl,mixer);
    this->son = al_load_sample(this->chemin.c_str());
    if (!son)
        cerr << "Erreur : chargement audio" << endl;
        // erreur("chargement audio");

    rc = al_set_sample(this->spl, this->son);
    if(rc) {
        al_set_sample_instance_gain(this->spl, this->gain);
        al_set_sample_instance_speed(this->spl, this->speed);
        al_set_sample_instance_playmode(this->spl, ALLEGRO_PLAYMODE_ONCE);
        al_set_sample_instance_playing(this->spl, false);
        this->played=false;
    }
    else
        cerr << "Erreur : attachement audio" << endl;
        // erreur("attachement audio");
}
Sound::Sound(const char* newChemin, int playmode, float gain, float vitesse, ALLEGRO_MIXER *mixer) {
    bool rc;
    this->chemin=newChemin;
    this->speed=vitesse;
    this->gain=gain;
    this->played=false;
    this->playmode=playmode;

    this->spl = al_create_sample_instance(NULL);
    al_attach_sample_instance_to_mixer(this->spl,mixer);
    this->son = al_load_sample(this->chemin.c_str());
    if (!son)
        cerr << "Erreur : chargement audio" << endl;
        // erreur("chargement audio");

    rc = al_set_sample(this->spl, this->son);
    if(rc) {
        al_set_sample_instance_gain(this->spl, this->gain);
        al_set_sample_instance_speed(this->spl, this->speed);
        setPlaymode(this->playmode);
        al_set_sample_instance_playing(this->spl, false);
        this->played=false;
    }
    else
        cerr << "Erreur : attachement audio" << endl;
        // erreur("attachement audio");
}
Sound::~Sound() // Destructeur
{
    al_destroy_sample(son);
    al_destroy_sample_instance(spl);
}
int Sound::getGain() {
    return this->gain;
}
int Sound::getSpeed() {
    return this->speed;
}
string Sound::getChemin() {
    return this->chemin;
}
int Sound::getPlaymode() {
    return this->playmode;
}
bool Sound::is_played() {
    return this->played;
}
void Sound::setChemin(const char* newChemin) {
    this->chemin=newChemin;
}
void Sound::setGain(float newGain) {
    this->gain=newGain;
    al_set_sample_instance_gain(this->spl, this->gain);
}
void Sound::setSpeed(float newSpeed) {
    this->speed=newSpeed;
    al_set_sample_instance_speed(this->spl, this->speed);
}
void Sound::setPlaymode(int playmode) {
    this->playmode=playmode;
    switch(playmode) {
        case ONCE:
            al_set_sample_instance_playmode(spl, ALLEGRO_PLAYMODE_ONCE);
            break;
        case LOOP:
            al_set_sample_instance_playmode(spl, ALLEGRO_PLAYMODE_LOOP);
            break;
        case BIDIR:
            al_set_sample_instance_playmode(spl, ALLEGRO_PLAYMODE_BIDIR);
            break;
        default:
            al_set_sample_instance_playmode(spl, ALLEGRO_PLAYMODE_ONCE);
            break;
    }
}
void Sound::stop(void) {
    al_set_sample_instance_playing(this->spl, false);
    this->played=false;
}
void Sound::play(void) {
    al_set_sample_instance_playing(this->spl, true);
    this->played=true;
}