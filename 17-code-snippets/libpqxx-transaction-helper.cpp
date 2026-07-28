#include <iostream>
#include <pqxx/pqxx>

int main(int argc, char *argv[]) {
    try {
        std::string conn_str = (argc > 1) ? argv[1] : "dbname=postgres";
        pqxx::connection conn{conn_str};

        pqxx::work txn{conn};

        // Execute query and iterate
        pqxx::result res = txn.exec("SELECT schemaname, tablename FROM pg_tables LIMIT 10");

        std::cout << "Tables in database:\n";
        for (auto const &row : res) {
            std::cout << row[0].c_str() << "." << row[1].c_str() << "\n";
        }

        txn.commit();
    } catch (std::exception const &e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}