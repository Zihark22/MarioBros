import os
from PIL import Image

def process_png(image_path):
    try:
        with Image.open(image_path) as img:
            # Vérifie si l'image a un profil ICC et le supprime
            if "icc_profile" in img.info:
                print(f"Profil ICC trouvé et supprimé pour : {image_path}")
                img.save(image_path, icc_profile=None)
            else:
                print(f"Pas de profil ICC pour : {image_path}")
    except Exception as e:
        print(f"Erreur lors du traitement de l'image {image_path}: {e}")

def process_images_in_directory(directory):
    # Parcourt tous les sous-dossiers et fichiers du répertoire
    for root, dirs, files in os.walk(directory):
        for file in files:
            if file.lower().endswith(".png"):
                image_path = os.path.join(root, file)
                process_png(image_path)

# Spécifiez ici le répertoire où se trouvent vos images PNG
directory = "datas/images/"
process_images_in_directory(directory)