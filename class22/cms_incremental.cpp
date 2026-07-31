#include <cryptominisat5/cryptominisat.h>
#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace CMSat;

static std::vector<int> parse_lits(const std::string& line, std::size_t start) {
    std::istringstream in(line.substr(start));
    std::vector<int> out;
    int x;
    while (in >> x && x) out.push_back(x);
    return out;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "usage: cms_incremental FILE.xcnf [seconds_per_branch=10] [threads=1] [passes=1]\n";
        return 2;
    }
    const std::string path = argv[1];
    const double sec = argc > 2 ? std::stod(argv[2]) : 10.0;
    const unsigned threads = argc > 3 ? std::stoul(argv[3]) : 1;
    const int passes = argc > 4 ? std::stoi(argv[4]) : 1;

    SATSolver solver;
    solver.set_num_threads(threads);
    solver.set_verbosity(0);
    solver.set_find_xors(false);
    solver.set_sampl_vars({0,1,2,3,4,5,6,7});

    std::ifstream f(path);
    if (!f) { std::cerr << "cannot open " << path << "\n"; return 2; }
    std::string line;
    uint64_t clauses = 0, xors = 0;
    unsigned nvars = 0;
    auto start = std::chrono::steady_clock::now();
    while (std::getline(f, line)) {
        if (line.empty() || line[0] == 'c') continue;
        if (line[0] == 'p') {
            std::istringstream in(line);
            std::string p, cnf; uint64_t ncls;
            in >> p >> cnf >> nvars >> ncls;
            solver.new_vars(nvars);
            continue;
        }
        if (line[0] == 'x') {
            auto xs = parse_lits(line, 1);
            std::vector<Lit> lits; lits.reserve(xs.size());
            for (int x: xs) lits.emplace_back((unsigned)std::abs(x)-1, x < 0);
            if (!solver.add_xor_clause(lits, true)) {
                std::cout << "BASE_UNSAT while adding xor\n";
                return 20;
            }
            xors++;
        } else {
            auto xs = parse_lits(line, 0);
            std::vector<Lit> lits; lits.reserve(xs.size());
            for (int x: xs) lits.emplace_back((unsigned)std::abs(x)-1, x < 0);
            if (!solver.add_clause(lits)) {
                std::cout << "BASE_UNSAT while adding clause\n";
                return 20;
            }
            clauses++;
        }
    }
    const auto loaded = std::chrono::duration<double>(std::chrono::steady_clock::now()-start).count();
    std::cerr << "loaded vars=" << nvars << " clauses=" << clauses << " xors=" << xors << " sec=" << loaded << "\n";

    std::vector<int> order;
    for (int v: {17,72,163}) order.push_back(v);
    for (int v=0; v<256; v++) if (std::find(order.begin(),order.end(),v)==order.end()) order.push_back(v);
    std::array<char,256> state{}; // 1 unsat, 2 unknown

    for (int pass=0; pass<passes; pass++) {
        int n_unsat=0, n_unknown=0;
        for (int v: order) {
            if (state[v] == 1) { n_unsat++; continue; }
            std::vector<Lit> assumps;
            for (unsigned b=0;b<8;b++) assumps.emplace_back(b, ((v>>b)&1)==0);
            solver.set_max_time(sec * (1u << std::min(pass, 8)));
            auto t0=std::chrono::steady_clock::now();
            lbool ret=solver.solve(&assumps);
            double dt=std::chrono::duration<double>(std::chrono::steady_clock::now()-t0).count();
            if (ret == l_False) {
                state[v]=1; n_unsat++;
                std::cout << "UNSAT value=" << v << " pass=" << pass << " sec=" << dt
                          << " conflicts=" << solver.get_last_conflicts() << "\n" << std::flush;
            } else if (ret == l_True) {
                const auto& m=solver.get_model();
                std::cout << "SAT value=" << v << " pass=" << pass << " sec=" << dt << "\nMODEL";
                for (unsigned i=0;i<std::min<unsigned>(376,m.size());i++)
                    std::cout << ' ' << (m[i]==l_True ? (int)i+1 : -(int(i)+1));
                std::cout << " 0\n";
                return 10;
            } else {
                state[v]=2; n_unknown++;
                std::cout << "UNKNOWN value=" << v << " pass=" << pass << " sec=" << dt
                          << " conflicts=" << solver.get_last_conflicts() << "\n" << std::flush;
            }
        }
        std::cout << "SUMMARY pass=" << pass << " unsat=" << n_unsat << " unknown=" << n_unknown
                  << " total_conflicts=" << solver.get_sum_conflicts() << "\n" << std::flush;
        if (n_unsat==256) return 20;
    }
    return 0;
}
