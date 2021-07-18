use gio::prelude::*;
use glib::clone;
use gtk::prelude::*;
use gtk::{
    ApplicationWindow, 
    Builder, 
    Button, 
    ToolButton,
    FileChooserButton,
    MessageDialog,
};

struct State
{
    assetpack_fn: *mut str,
    svg_fn: *mut str,
    output_dir: *mut str,
    track_width: f32,
    runoff_width: f32,
    wall_width: f32,
    kappa_thr: f32,
    origin_com: bool,
    gen_preview: bool,
}

fn tool_save_clicked(btn: &gtk::ToolButton)
{
    println!("tool_save");
}

fn tool_open_clicked(btn: &gtk::ToolButton)
{
    println!("tool_save");
}

fn tool_generate_clicked(btn: &gtk::ToolButton)
{
    println!("tool_save");
}

fn build_ui(app: &gtk::Application)
{
    let glade_src = include_str!("svg2track-gui.ui");
    let builder = Builder::from_string(glade_src);

    let window: ApplicationWindow = builder.object("window1").expect("Couldn't get window1");
    window.set_application(Some(app));

    /*
    let button: Button = builder.object("button1").expect("couldn't get button1");
    let dialog: MessageDialog = builder
        .object("messagedialog1")
        .expect("couldn't get diag 1");

    dialog.connect_delete_event(|dialog, _| {
        dialog.hide();
        gtk::Inhibit(true)
    });

    button.connect_clicked(glib::clone!(@weak dialog => move |_| dialog.show_all()));
    */

    let tool_save: ToolButton = builder.object("tool_save").expect("bad ui");
    tool_save.connect_clicked(tool_save_clicked);

    let tool_open: ToolButton = builder.object("tool_open").expect("bad ui");
    tool_open.connect_clicked(tool_open_clicked);

    let tool_generate: ToolButton = builder.object("tool_generate").expect("bad ui");
    tool_generate.connect_clicked(tool_generate_clicked);

    window.show_all();
}

fn main()
{
    println!("Hello, world!");
    let app = gtk::Application::new(
        Some("com.github.irishpatrick.mode7.svg2track-gui"),
        Default::default()
    );
    
    app.connect_activate(build_ui);
    app.run();
}
