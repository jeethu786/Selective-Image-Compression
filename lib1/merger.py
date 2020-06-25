
import cv2
import numpy as np

img1=cv2.imread('rd.jpg',1)
img2=cv2.imread('gn.jpg',1)
img3=cv2.imread('bl.jpg',1)

img4=cv2.imread('my_pic2.jpg',1)

(b,g,r)=cv2.split(img4)



#img5=cv2.imread('rd.jpg',1)
#img6=cv2.imread('gn.jpg',1)
#img7=cv2.imread('bl.jpg',1)

img1=cv2.resize(img1,(512,512))
img2=cv2.resize(img2,(512,512))
img3=cv2.resize(img3,(512,512))

#img5=cv2.resize(img5,(512,512))
#img6=cv2.resize(img6,(512,512))
#img7=cv2.resize(img7,(512,512))

#cv2.imshow('my_pic3.jpg',img1)
#cv2.imshow('my_pic4.jpg',img2)
#cv2.imshow('my_pic5.jpg',img3)

#cv2.imshow('my_pic6.jpg',img5)
#cv2.imshow('my_pic7.jpg',img6)
#cv2.imshow('my_pic8.jpg',img7)
#img4=cv2.resize(img4,(3840,2400))

img1 = cv2.cvtColor(img1, cv2.COLOR_BGR2GRAY)
img2 = cv2.cvtColor(img2, cv2.COLOR_BGR2GRAY)
img3 = cv2.cvtColor(img3, cv2.COLOR_BGR2GRAY)

#img5 = cv2.cvtColor(img5, cv2.COLOR_BGR2GRAY)
#img6 = cv2.cvtColor(img6, cv2.COLOR_BGR2GRAY)
#img7 = cv2.cvtColor(img7, cv2.COLOR_BGR2GRAY)

res = cv2.merge((img3,img2,img1))
res1=cv2.merge((b,g,r))

res1=cv2.resize(res1,(512,512))

#img4=cv2.resize(img4,(512,512))
#res=cv2.resize(res,(3840,2400))

#cv2.imshow('my_pic2.jpg',img4)
cv2.imshow('res1',res1)
cv2.imshow('res',res)
cv2.imwrite('result.jpg',res)
cv2.waitKey(0)

cv2.destroyAllWindows()


