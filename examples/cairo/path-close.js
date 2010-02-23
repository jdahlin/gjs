const Cairo = imports.cairo;

let surface = new Cairo.ImageSurface(Cairo.Format.ARGB32, 120, 120);
let cr = new Cairo.Context(surface);
cr.scale(120, 120);

cr.setLineWidth(0.1);
cr.setSourceRGB(0, 0, 0);

cr.moveTo(0.25, 0.25);
cr.lineTo(0.5, 0.375);
cr.relLineTo(0.25, -0.125);
cr.arc(0.5, 0.5, 0.25 * Math.sqrt(2), -0.25 * Math.PI, 0.25 * Math.PI);
cr.relCurveTo(-0.25, -0.125, -0.25, 0.125, -0.5, 0);
cr.closePath();

cr.stroke();

surface.writeToPNG("path-close.png");

