# include "mode.hh"
# include "log.hh"

namespace Astroid {
  Mode::Mode (bool _interactive) :
    Gtk::Box (Gtk::ORIENTATION_VERTICAL),
    interactive (_interactive)
  {

    /* set up yes-no asker */
    if (interactive)
    {
      log << debug << "mode: setting up yes-no question." << endl;
      rev_yes_no = Gtk::manage (new Gtk::Revealer ());
      rev_yes_no->set_transition_type (Gtk::REVEALER_TRANSITION_TYPE_SLIDE_UP);

      Gtk::Box * rh = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL));

      label_yes_no = Gtk::manage (new Gtk::Label ());
      rh->pack_start (*label_yes_no, true, true, 5);
      label_yes_no->set_halign (Gtk::ALIGN_START);

      /* buttons */
      Gtk::Button * yes = Gtk::manage (new Gtk::Button("_Yes"));
      Gtk::Button * no  = Gtk::manage (new Gtk::Button("_No"));

      yes->set_use_underline (true);
      no->set_use_underline (true);

      rh->pack_start (*yes, false, true, 5);
      rh->pack_start (*no, false, true, 5);

      rev_yes_no->set_margin_top (0);
      rh->set_margin_bottom (5);

      rev_yes_no->add (*rh);
      rev_yes_no->set_reveal_child (false);
      pack_end (*rev_yes_no, false, true, 0);
    }
  }

  void Mode::ask_yes_no (
      ustring question,
      function <void (bool)> closure)
  {
    if (!interactive) throw logic_error ("mode is not interactive!");

    log << info << "mode: " << question << endl;

    yes_no_waiting = true;
    yes_no_closure = closure;

    rev_yes_no->set_reveal_child (true);
    label_yes_no->set_text (question + " [y/n]");
  }

  void Mode::answer_yes_no (bool yes) {
    if (!interactive) throw logic_error ("mode is not interactive!");

    if (yes) {
      log << info << "mode: yes-no: got yes!" << endl;
    } else {
      log << info << "mode: yes-no: got no :/" << endl;
    }

    if (yes_no_waiting) {
      if (yes_no_closure != NULL) {
        yes_no_closure (yes);
      }
    }

    yes_no_closure = NULL;
    yes_no_waiting = false;

    rev_yes_no->set_reveal_child (false);
  }

  bool Mode::mode_key_handler (GdkEventKey * event) {
    log << debug << "mode: got key press" << endl;
    if (!interactive) throw logic_error ("mode is not interactive!");

    if (yes_no_waiting) {
      switch (event->keyval) {
        case GDK_KEY_Y:
        case GDK_KEY_y:
          answer_yes_no (true);
          return true;

        case GDK_KEY_N:
        case GDK_KEY_n:
          answer_yes_no (false);
          return true;
      }
    }

    return false;
  }

  Mode::~Mode () {
  }

}

