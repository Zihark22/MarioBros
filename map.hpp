#ifndef MAP_HPP
#define MAP_HPP

#include "parametres.hpp"
#include "element.hpp"

class Map : public Element
{
    public:
    	Map(string chemin_img); // Constructeur
    	~Map(); // Destructeur

		// Setters
		void setBackgroundScale(float scale);
		void setBackgroundX(int pos);
		void setMap0(bool Map0);

		// Getters
		float getBackgroundScale();
		int getBackgroundX();
		bool isMap0();

    	void draw(int width, int height);

	private :
		float backgroundScale;
		int backgroundX;
		bool Map0;
};

#endif