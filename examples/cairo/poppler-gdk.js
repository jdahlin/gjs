const Cairo = imports.cairo;
const Lang = imports.lang;

const Gdk = imports.gi.Gdk;
const Gtk = imports.gi.Gtk;
const Poppler = imports.gi.Poppler;

function PopplerApp(filename) {
    this._init(filename);
}

PopplerApp.prototype = {
    _init : function(filename) {
        this._uri = "file://" + filename;
        this._document = Poppler.Document.new_from_file(this._uri, "");
        this._buildUI();
    },

    _buildUI : function() {
        this._window = new Gtk.Window({ type: Gtk.WindowType.TOPLEVEL });
        this._window.connect("destroy", Lang.bind(this, this._onWindowDestroy));

        let da = new Gtk.DrawingArea();
        da.connect("expose-event", Lang.bind(this, this._onDrawingAreaExpose));
        this._window.add(da);
    },

    _onDrawingAreaExpose : function(window, event) {
        let cr = Gdk.cairo_create(window.window);
        let page = this._document.get_page(0);
        page.render(cr);
    },

    _onWindowDestroy : function(window) {
        Gtk.main_quit();
    },

    run : function() {
        this._window.show_all();
        Gtk.main();
    }

}

function main() {
    if (ARGV.length == 0) {
        printerr("Need a filename");
        return 1;
    }

    Gtk.init(0, null);

    let app = new PopplerApp(ARGV[0]);
    app.run();
    return 0;
}

main();
