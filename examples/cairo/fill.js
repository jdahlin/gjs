const Cairo = imports.cairo;

let surface = new Cairo.ImageSurface(Cairo.Format.ARGB32, 120, 120);
let cr = new Cairo.Context(surface);
cr.scale(120, 120);

cr.setSourceRGB(0, 0, 0);
cr.rectangle(0.25, 0.25, 0.5, 0.5);
cr.fill();

surface.writeToPNG("fill.png");
