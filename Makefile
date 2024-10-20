OS = LINUX # MAC, LINUX ou WINDOWS
CC = g++

ifeq ($(CC),g++)
	EXT := cpp
else
	EXT := c
endif

SRC_DIR = src
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
INC_DIR = include

ifeq ($(OS),WINDOWS)
	EXEC := $(BUILD_DIR)/mario_app.exe
else
	EXEC := $(BUILD_DIR)/mario_app
endif

CFLAGS = -g -I$(INC_DIR) -std=c++17 # Wall donne les warnings et g génère des infos de débogage
LIBS_ALLEGRO = -lallegro -lallegro_main -lallegro_primitives -lallegro_font -lallegro_image -lallegro_color -lallegro_ttf -lallegro_dialog -lallegro_audio -lallegro_acodec -lallegro_video

SRC = $(wildcard $(SRC_DIR)/*.$(EXT))
OBJ = $(patsubst $(SRC_DIR)/%.$(EXT), $(OBJ_DIR)/%.o, $(SRC))
DEPS = $(patsubst $(SRC_DIR)/%.$(EXT), $(OBJ_DIR)/%.d, $(SRC))



all: clean_screen $(BUILD_DIR) $(OBJ_DIR) $(EXEC) run #clean

# Lier les fichiers objets pour créer l'exécutable
$(EXEC): $(OBJ)
	@$(CC) -o $(EXEC) $(OBJ) $(LIBS_ALLEGRO)
	@echo "\033[36mCompilation successful \033[0m"

# Règle pour compiler les fichiers .c en .o et générer les dépendances
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.$(EXT) | $(OBJ_DIR)
	@$(CC) $(CFLAGS) -o $@ -c $<
	@echo "\033[35mCompilation $< successful\033[0m"

# Générer les fichiers de dépendances .d
$(OBJ_DIR)/%.d: $(SRC_DIR)/%.$(EXT) | $(OBJ_DIR)
	@$(CC) -MM -MF $@ -MT $(@:.d=.o) $(CFLAGS) $<


# Crée le dossier build s'il n'existe pas
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Crée le dossier obj s'il n'existe pas
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

-include $(DEPS)

# Appel la cible nothing si rien à faire
ifneq ($(OBJ),)
    all: nothing
endif

clean:
	rm -f $(OBJ_DIR)/*.o $(DEPS)

rmexec:clean
	rm -rf $(EXEC)

run:
	./$(EXEC)

clean_screen:
	clear && printf '\e[3J' ;

nothing: # Affiche un message si nothing to do
	@echo "Rien à faire ! \033[35mLes fichiers sont à jour\033[0m"

# .PHONY pour créer des cible non fichiers et evite les conflits
.PHONY: clean rmexec run clean_screen nothing
