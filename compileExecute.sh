#-------------
#   Allegro
#-------------
#Compiling and executing a file <nom_fichier.cpp> with Allegro
#command : ./CompileExecute.sh <nom_fichier>

clear && printf '\e[3J' ; # effacer l'écran du terminal (remonte l'affichage hors de vue) et si c'est un succès, nettoyage supplémentaire du terminal en utilisant des séquences d'échappement ANSI pour avoir un terminal propre

compile=false;

current_directory=$(pwd)

OS="WINDOWS"
executable="exec"

# if $OS == "WINDOWS"; then
#   executable += ".exe"
# fi

if [ $# -eq 0 ]; then # aucun paramètre
  # Créez le répertoire de sortie s'il n'existe pas
  mkdir -p build/obj

  # Compilez tous les fichiers source .cpp en fichiers objets .o
  for file in src/*.cpp; do
      filename=$(basename "$file")
      g++ -std=c++17 -c "$file" -Iinclude -o "build/obj/${filename%.cpp}.o"
      if [ $? -ne 0 ]; then
        echo -e "\033[31mErreur lors de la compilation de ${file}\033[0m"
        exit 1  # Sortie du script en cas d'erreur
      else
        echo -e "\033[35mCompilation ${file} successful\033[0m"
      fi
  done

  cd build/obj/ # changement de dossier courant

  # Liaison des fichiers objets pour créer l'exécutable
  g++ mario.o game.o duree.o map.o personnage.o bloc.o element.o sound.o piege.o user.o objectlance.o mechant.o -o ../$executable -lallegro -lallegro_main -lallegro_primitives -lallegro_font -lallegro_image -lallegro_color -lallegro_ttf -lallegro_dialog -lallegro_audio -lallegro_acodec -lallegro_video
  if [ $? -eq 0 ]; then # -ne pour une erreur sur la commande précédente et -eq pour pas d'erreur
    echo -e "\033[36mCompilation successful \033[0m"
    cd $current_directory # retour au dossier d'origine
    ./build/$executable # execution
  else
    echo -e "\033[31mCompilation failed !!\033[0m"
  fi
else 
   echo -e "\033[38;5;208mErreur aucuns paramètres requis : ./compileExecute.sh \033[0m"
fi

cd $current_directory # retour au dossier d'origine