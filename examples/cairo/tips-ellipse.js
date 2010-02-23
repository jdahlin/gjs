const Cairo = imports.cairo;

let surface = new Cairo.ImageSurface(Cairo.Format.ARGB32, 120, 120);
let cr = new Cairo.Context(surface);
cr.scale(120, 120);

cr.setLineWidth(0.10);

cr.save();
cr.scale(0.5, 1);
cr.arc(0.5, 0.5, 0.40, 0, 2 * Math.PI);
cr.stroke();

cr.translate(1, 0);
cr.arc(0.5, 0.5, 0.40, 0, 2 * Math.PI);
cr.restore();
cr.stroke();

surface.writeToPNG("tips-ellipse.png");

