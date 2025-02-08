#include <iostream> // std::cout, std::endl
#include <string>
#include <fstream>
#include <sstream>
#include <list>

#include "bpg.h"
#include "common.h"

namespace bpg{
    void SaveArmada(const RunningOpt &run_opt, std::list<Puzzle> puzzleList);
    std :: string SecondLineMatrix(const RunningOpt &run_opt);
    std::string MatrixString(Puzzle &puzzle, const RunningOpt &run_opt);
    void SaveMatrix(const RunningOpt &run_opt, std::list<Puzzle> puzzleList);
}
