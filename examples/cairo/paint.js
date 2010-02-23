const Cairo = imports.cairo;

let surface = new Cairo.ImageSurface(Cairo.Format.ARGB32, 120, 120);
let cr = new Cairo.Context(surface);
cr.scale(120, 120);

cr.setSourceRGB(0.0, 0.0, 0.0);
cr.paintWithAlpha(0.5);

surface.writeToPNG("paint.png");
