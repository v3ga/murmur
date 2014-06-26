PImage logo = loadImage("murmur_logo_cartouche.png");
size(logo.width, logo.height, P2D);
background(0);
logo.filter(INVERT);
image(logo,0,0);
saveFrame("murmur_logo_cartouche_invert.png");
