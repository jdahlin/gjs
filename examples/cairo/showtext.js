const Cairo = imports.cairo;

let surface = new Cairo.ImageSurface(Cairo.Format.ARGB32, 120, 120);
let cr = new Cairo.Context(surface);
cr.scale(120, 120);

cr.setSourceRGB(0, 0, 0);
cr.selectFontFace("Georgia", Cairo.FontSlant.NORMAL, Cairo.FontWeight.BOLD);
cr.setFontSize(1.2);

//let te = cr.text_extents("a");
//cr.moveTo(0.5 - te.width / 2 - te.xBearing,
//          0.5 - te.height / 2 - te.yBearing);
cr.showText("a");

surface.writeToPNG("showtext.png");

