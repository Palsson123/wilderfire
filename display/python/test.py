import os, sys
import Image

im = Image.open("a.png")
x = 30
y = 30

pix = im.load()
print pix[x,y]
