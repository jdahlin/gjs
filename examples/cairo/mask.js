const Cairo = imports.cairo;

let surface = new Cairo.ImageSurface(Cairo.Format.ARGB32, 120, 120);
let cr = new Cairo.Context(surface);
cr.scale(120, 120);

let linpat = new Cairo.LinearGradient(0, 0, 1, 1);
linpat.addColorStopRGB(0, 0, 0.3, 0.8);
linpat.addColorStopRGB(1, 0, 0.8, 0.3);

let radpat = new Cairo.RadialGradient(0.5, 0.5, 0.25, 0.5, 0.5, 0.75);
radpat.addColorStopRGBA(0, 0, 0, 0, 1);
radpat.addColorStopRGBA(0.5, 0, 0, 0, 0);

cr.setSource(linpat);

cr.mask(radpat);

surface.writeToPNG("mask.png");
