#-------------
#   Allegro
#-------------
#Compiling and executing a file <nom_fichier.cpp> with Allegro
#command : ./CompileExecute.sh <nom_fichier>

clear && printf '\e[3J' ;

compile=false;

if [ $1 ]; then 
  if [ $2 ]; then
    # compilation des classes
    g++ -c map.cpp -o map.o
    g++ -c personnage.cpp -o personnage.o
    g++ -c bloc.cpp -o bloc.o
    g++ -c element.cpp -o element.o
    g++ -c sound.cpp -o sound.o
    g++ -c piege.cpp -o piege.o
    g++ -c user.cpp -o user.o
    g++ -c objectlance.cpp -o objectlance.o
    g++ -c mechant.cpp -o mechant.o
    
    # compilation du main
    g++ -c $2.cpp -o $2.o
    g++ -c $1.cpp -o $1.o
    g++ map.o personnage.o bloc.o element.o sound.o piege.o user.o objectlance.o mechant.o $2.o $1.o -o exec -lallegro -lallegro_main -lallegro_primitives -lallegro_font -lallegro_image -lallegro_color -lallegro_ttf -lallegro_dialog -lallegro_audio -lallegro_acodec -lallegro_video
    if [ $? -eq 0 ]; then
     echo "\033[36mCompilation successful \033[0m"
     ./exec # execution
    else
     echo "\033[31mCompilation failed !!\033[0m"
    fi
  fi
   
else 
   echo "\033[38;5;208mErreur nom du fichier manquant (sans extension .cpp) : ./compileExecute.sh <nom_fichier>\033[0m"
fi