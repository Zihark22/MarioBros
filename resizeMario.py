import cv2
import numpy as np

nom_img = "/Users/badajozj/Documents/Coding/C++/Allegro/accueil-jeu/persos/Mario2/Mario2_4"
ext = ".png"

img = cv2.imread(nom_img+ext, cv2.IMREAD_UNCHANGED)

b,g,r,a = cv2.split(img)

alpha = np.ones(a.shape, dtype=a.dtype)*255

alpha[:,:] = a

result = cv2.merge((b,g,r,alpha))

print(img.shape)

height, width = img.shape[:2]

newwidth = 300
newheight = int(np.round(height * newwidth/width))

dim = (newwidth,newheight)
print(dim)

resized = cv2.resize(result,dim, interpolation = cv2.INTER_LINEAR)  # INTER_NEAREST=augmenter
print(resized.shape)

cv2.imwrite(nom_img+"-2"+ext,resized)

# cv2.imshow("resized", resized)
# cv2.waitKey(0)
# cv2.destroyAllWindows()
