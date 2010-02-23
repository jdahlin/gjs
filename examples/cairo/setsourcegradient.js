const Cairo = imports.cairo;

let surface = new Cairo.ImageSurface(Cairo.Format.ARGB32, 120, 120);
let cr = new Cairo.Context(surface);
cr.scale(120, 120);

let radpat = new Cairo.RadialGradient(0.25, 0.25, 0.1, 0.5, 0.5, 0.5);
radpat.addColorStopRGB(0, 1.0, 0.8, 0.8);
radpat.addColorStopRGB(1, 0.9, 0.0, 0.0);

for (let i = 1; i < 10; ++i) {
    for (let j = 1; j < 10; ++j) {
        cr.rectangle(i/10.0 - 0.04, j/10.0 -0.04, 0.08, 0.08);
    }
}

cr.setSource(radpat);
cr.fill();

let linpat = new Cairo.LinearGradient(0.25, 0.35, 0.75, 0.65);
linpat.addColorStopRGBA(0.00, 1, 1, 1, 0);
linpat.addColorStopRGBA(0.25, 0, 1, 0, 0.5);
linpat.addColorStopRGBA(0.50, 1, 1, 1, 0);
linpat.addColorStopRGBA(0.75, 0, 0, 1, 0.5);
linpat.addColorStopRGBA(1.00, 1, 1, 1, 0);

cr.rectangle(0.0, 0.0, 1, 1);
cr.setSource(linpat);
cr.fill();

surface.writeToPNG("setsourcegradient.png");

