# pragma once

# include <vector>

# include "astroid.hh"
# include "proto.hh"

using namespace std;

namespace Astroid {
  class Account {
    public:
      ustring id;
      ustring name;
      ustring email;
      ustring gpgkey;
      ustring sendmail;

      bool isdefault;

      bool save_sent;
      ustring save_sent_to;


      ustring full_address ();
  };

  class AccountManager {
    public:
      AccountManager ();
      ~AccountManager ();

      vector<Account> accounts;
      int default_account;
      Account * get_account_for_address (ustring);
  };
}

