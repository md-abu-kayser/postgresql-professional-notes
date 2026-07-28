#include <iostream>
#include <vector>
#include <pqxx/pqxx>

int main(int argc, char *argv[]) {
    try {
        std::string conn_str = (argc > 1) ? argv[1] : "dbname=postgres";
        pqxx::connection conn{conn_str};

        // Start a non-transaction pipeline (auto-commit per statement)
        pqxx::pipeline pipe(conn, "my_pipeline");

        // Submit several statements
        pipe.insert("SELECT 1");
        pipe.insert("SELECT pg_sleep(0.1), 2");  // Simulate work
        pipe.insert("SELECT 3");

        // Wait for all results, then process
        std::vector<pqxx::result> results;
        pipe.complete();

        // Retrieve results in order
        while (!pipe.empty()) {
            auto res = pipe.retrieve();
            if (res.empty())
                std::cout << "Empty result\n";
            else
                std::cout << "Result[0][0] = " << res[0][0].c_str() << "\n";
        }
    } catch (std::exception const &e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}