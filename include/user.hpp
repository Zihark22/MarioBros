#ifndef USER_HPP
#define USER_HPP

#include "parametres.hpp"
#include "personnage.hpp"

class User : public Personnage
{
    public:
		User() : Personnage() {
			imgs.clear();
			this->h=0;
			this->w=0;
			this->msg="";
		}
		User(string nom) : Personnage(nom) // constructeur 
		{
			string chemin, orientation, extension, filename;
			if(nom=="_") {
				imgs.clear();
				this->h=0;
				this->w=0;
			}
			else {
				chemin="datas/persos/"+this->nom+"/";
				for (const auto& entry : directory_iterator(chemin)) 
				{
					if (entry.is_regular_file()) {
						filename = entry.path().filename();
						if(filename!=".DS_Store") {
							orientation = filename.substr(nom.size()+1, filename.size()-nom.size()-(3+1+1)); // -3 pour l'extension   -1 pour le _ et -1 pour le .
							extension = filename.substr(filename.size()-3);
                			transform(orientation.begin(), orientation.end(), orientation.begin(), ::toupper); // passage en majuscule

							// cout << "perso=" << nom << " , orientation=" << orientation << ", extension=" << extension << endl;
							imgs[orientation] = al_load_bitmap((chemin+filename).c_str());
							if(!imgs[orientation]) {
								cerr << "Erreur : chargement du perso = " << this->nom << " et de l'image = " << chemin << endl;
								this->h=0;
								this->w=0;
							}
							else {
								this->h=al_get_bitmap_height(imgs[orientation]);
								this->w=al_get_bitmap_width(imgs[orientation]);
							}
						}
					}
				}
			}
			this->msg="";
		}
    	~User() { // Destructeur
			map<string,ALLEGRO_BITMAP*>::iterator it;  //Un itérateur
			for (it=imgs.begin();it!=imgs.end();++it) {
				if(it->second) {
					al_destroy_bitmap(it->second); // destruction des images
				}
			}
		}
		User(User const& userACopier); // constructeur de copie

    // Méthodes (prototypes)
    	// Getters
		ALLEGRO_BITMAP* getImg(int indice) const;
    	string getMessage() const;

    	// Setters
		void setImg(const char* chemin, int indice);
		void setImg(ALLEGRO_BITMAP* img, int indice);
    	void setMessage(string message);

    	// Autres
    	void actualiseSize(int indice);
		void draw(int indice);
    	void afficherMessage(ALLEGRO_FONT *font);

    protected :
	    // Attributs
	    // ALLEGRO_BITMAP *imgs[IMGS_PERSOS];
        map <string,ALLEGRO_BITMAP *> imgs;
		string msg;
};


#endif