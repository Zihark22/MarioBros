#!/bin/bash

# Créez le répertoire de sortie s'il n'existe pas
mkdir -p build/obj

# Compilez tous les fichiers source .cpp en fichiers objets .o
for file in src/*.cpp; do
    filename=$(basename "$file")
    g++ -c "$file" -Iinclude -o "build/obj/${filename%.cpp}.o"
done
