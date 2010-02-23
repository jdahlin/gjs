const Cairo = imports.cairo;

let surface = new Cairo.ImageSurface(Cairo.Format.ARGB32, 120, 120);
let cr = new Cairo.Context(surface);
cr.scale(120, 120);

cr.setSourceRGB(0, 0, 0);
cr.moveTo(0, 0);
cr.lineTo(1, 1);
cr.moveTo(1, 0);
cr.lineTo(0, 1);
cr.setLineWidth(0.2);
cr.stroke();

cr.rectangle(0, 0, 0.5, 0.5);
cr.setSourceRGBA(1, 0, 0, 0.80);
cr.fill();

cr.rectangle(0, 0.5, 0.5, 0.5);
cr.setSourceRGBA(0, 1, 0, 0.60);
cr.fill();

cr.rectangle(0.5, 0, 0.5, 0.5);
cr.setSourceRGBA(0, 0, 1, 0.40);
cr.fill();

surface.writeToPNG("setsourcergba.png");

