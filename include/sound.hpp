#ifndef SOUND_HPP
#define SOUND_HPP

#include "parametres.hpp"

class Sound
{
    public:
    	Sound();  // Constructeur
    	Sound(const char* newChemin, ALLEGRO_MIXER *mixer);
    	Sound(const char* newChemin, int playmode, float gain, float vitesse, ALLEGRO_MIXER *mixer);
    	~Sound(); // Destructeur

    // Méthodes (prototypes)
    	// Getters
    	int getGain() const;
    	int getSpeed() const;
    	string getChemin() const;
    	int getPlaymode() const;
    	bool is_played() const;

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

#endif