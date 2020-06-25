from skimage import color
from skimage import io
import cv2

img=color.rgb2gray(io.imread('color.jpeg'))

rgb=color.gray2rgb(img)
cv2.imshow("gray",img)
cv2.imshow("color",rgb)
cv2.waitKey(0)
cv2.destroyAllWindows()
