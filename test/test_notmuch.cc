# define BOOST_TEST_DYN_LINK
# define BOOST_TEST_MODULE TestNotmuch
# include <boost/test/unit_test.hpp>
# include <boost/filesystem.hpp>

# include "test_common.hh"

# include <notmuch.h>

namespace bfs = boost::filesystem;

BOOST_AUTO_TEST_SUITE(Notmuch)

  BOOST_AUTO_TEST_CASE(read_all_threads)
  {

    bfs::path path_db = bfs::absolute (bfs::path("./test/mail/test_mail"));

    notmuch_database_t * nm_db;

    notmuch_status_t s =
      notmuch_database_open (
        path_db.c_str(),
        notmuch_database_mode_t::NOTMUCH_DATABASE_MODE_READ_ONLY,
        &nm_db);


    BOOST_CHECK (s == NOTMUCH_STATUS_SUCCESS);

    /* read all messages */
    std::cout << "db: running test query.." << endl;
    auto q = notmuch_query_create (nm_db, "*");

    unsigned int c;
    notmuch_status_t st = notmuch_query_count_messages_st (q, &c);

    BOOST_CHECK (st == NOTMUCH_STATUS_SUCCESS);

    std::cout << "query: " << notmuch_query_get_query_string (q) << ", approx: "
         << c << " messages." << endl;

    notmuch_messages_t * messages;
    notmuch_message_t  * message;

    for (st = notmuch_query_search_messages_st (q, &messages);

         (st == NOTMUCH_STATUS_SUCCESS) &&
         notmuch_messages_valid (messages);

         notmuch_messages_move_to_next (messages))
    {
      message = notmuch_messages_get (messages);

      std::cout << "thread:" << notmuch_message_get_thread_id (message) << ", message: " << notmuch_message_get_header (message, "Subject") << endl;

      notmuch_message_destroy (message);
    }


    notmuch_database_close (nm_db);
  }

BOOST_AUTO_TEST_SUITE_END()

