const Cairo = imports.cairo;

const Pango = imports.gi.Pango;
const PangoCairo = imports.gi.PangoCairo;

const RADIUS = 150;
const N_WORDS = 10;
const FONT = "Sans Bold 27";

function drawText(cr) {
    // Center coordinates on the middle of the region we are drawing
    cr.translate(RADIUS, RADIUS);
    // Create a PangoLayout, set the font and text
    let layout = PangoCairo.create_layout(cr);
    layout.set_text("Text", - 1);
    let desc = Pango.Font.description_from_string(FONT);
    layout.set_font_description(desc);

    // Draw the layout N_WORDS times in a circle
    for (let i = 0; i < N_WORDS; ++i) {
        let angle = (360. * i) / N_WORDS;
        cr.save();
        // Gradient from red at angle == 60 to blue at angle == 240
        let red = (1 + Math.cos ((angle - 60) * Math.PI / 180.)) / 2;
        cr.setSourceRGB(red, 0, 1.0 - red);
        cr.rotate(angle * Math.PI / 180.);
        // Inform Pango to re-layout the text with the new transformation
        PangoCairo.update_layout(cr, layout);
        [width, height] = layout.get_size();
        cr.moveTo(- (width / Pango.SCALE) / 2, - RADIUS);
        PangoCairo.show_layout(cr, layout);
        cr.restore();
    }
}

function main() {
    if (ARGV.length == 0) {
      printerr ("Usage: pango OUTPUT_FILENAME\n");
      return 1;
    }
    let filename = ARGV[0];
    let surface = new Cairo.ImageSurface(Cairo.Format.ARGB32,
                                         2 * RADIUS, 2 * RADIUS);
    let cr = new Cairo.Context(surface);
    cr.setSourceRGB(1.0, 1.0, 1.0);
    cr.paint();
    drawText(cr);
    surface.writeToPNG(filename);
    return 0;
}

main();
