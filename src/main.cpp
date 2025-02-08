/*!
 * \brief Battleship Puzzle Generator
 * \author Selan
 * \date March 15th, 2024.
 *
 * This program generates and outputs to a file a bat of
 * battleship puzzles.
 * https://en.wikipedia.org/wiki/Battleship_(puzzle)
 *
 * The puzzles should be between 10x10 and 16x16 boards,
 * with 10 ships.
 * The ships might be:
 *  - 01 Battleship (4 pieces long)
 *  - 02 Cruisers (3 pieces long)
 *  - 03 Destroyers (2 pieces long)
 *  - 04 Submarines (1 piece long)
 *
 * The puzzles file should be read by another application, the
 * battleship puzzle game.
 */

#include <iostream> // std::cout, std::endl
#include <string>
#include <cstdlib> // exit

#include "include/bpg.h"
#include "include/common.h"
#include "include/file.h"

/*!
 * Displays the welcome message for the Battleship Puzzle Game.
 */
void welcome_msg() {
  std::cout << "------Battleship Puzzle Game------" << std::endl << "-------------Welcome--------------" << std::endl;
  std::cout << "------Here are your puzzles:------" << std::endl << std::endl;
}

/*!
 * Displays the error message for the Battleship Puzzle Game.
 */
void error_msg() {
  std:: cout << "Usage: [<options>] <number_of_puzzles>" << std::endl << std::endl;
    std:: cout << "Program options are:" << std::endl << std::endl;
    std:: cout << "       --rows <num>	Specify the number of rows for the matrix," << std::endl << "                        with `<num>` in the range [7, 16 ]." << std::endl << "                        The default value is 10." << std::endl;
    std:: cout << "       --cols <num>	Specify the number of columns for the matrix," << std::endl << "                        with `<num>` in the range [7,16]." << std::endl << "                        The default value is 10." << std::endl << std::endl;    
    std:: cout << "Requested input is:" << std::endl << std::endl;
    std:: cout << "       number_of_puzzles	The number of puzzles to be generated" << std::endl << "                                in the range [1,100]."<< std::endl << std::endl;
}

/*!
 * Prints possible errors based on the given error type.
 *
 * @param tipo The type of error:
 *             - 1: Syntax error
 *             - 2: Invalid number of rows
 *             - 3: Invalid number of columns
 *             - 4: Invalid number of puzzles
 */
void possibleErrors(int tipo){
    switch (tipo){
    case 1: std::cout << "Syntax error" << std::endl; break;
    case 2: std::cout << "Invalid number of rows" << std::endl; break;
    case 3: std::cout << "Invalid number of columns" << std::endl; break;
    case 4: std::cout << "Invalid number of puzzles" << std::endl; break;
    }
}

/*!
 * Validates the input arguments for the Battleship Puzzle Game.
 * @param argc The number of command-line arguments.
 * @param argv An array of strings containing the command-line arguments.
 * @return A struct containing the validated input options (rows, columns, and number of puzzles).
 */
RunningOpt validate_input(int argc, char *argv[]) {
  RunningOpt saida;
  saida.rows = 10;
  saida.cols = 10;
  std :: string rows = "--rows";
  std :: string cols = "--cols";
  switch (argc){
    case 2:      // ./bpg n_puzzles
        try {
            saida.n_puzzles = std::stoi(argv[1]);
            if (saida.n_puzzles > 100) {
                possibleErrors(4);
                throw std::invalid_argument("Invalid number of puzzles");
            }
        } catch (const std::exception& e) {
            error_msg();
            exit(1);
        }
        break;

    case 4:     // ex: ./bpg --rows x puzzles || ./bpg --cols x puzzles
        if (argv[1] == rows ){
            try {
                saida.rows = std::stoi(argv[2]);
                if (saida.rows < 7 || saida.rows > 16) {
                    possibleErrors(2);
                    throw std::invalid_argument("Invalid number of rows");
                }
                saida.n_puzzles = std::stoi(argv[3]);
                if (saida.n_puzzles > 100) {
                    possibleErrors(4);
                    throw std::invalid_argument("Invalid number of puzzles");
                }
            } catch (const std::exception& e) {
                error_msg();
                exit(1);
            }

        }else if (argv[1] == cols){
            try {
                saida.cols = std::stoi(argv[2]);
                if (saida.cols < 7 || saida.cols > 16) {
                    possibleErrors(3);
                    throw std::invalid_argument("Invalid number of columns");
                }
                saida.n_puzzles = std::stoi(argv[3]);
                if (saida.n_puzzles > 100) {
                    possibleErrors(4);
                    throw std::invalid_argument("Invalid number of puzzles");
                }
            } catch (const std::exception& e) {
                error_msg();
                exit(1);
            }
        }else{
            std::cout << "bpg ERROR"<< std::endl;
            possibleErrors(1);
            error_msg();
            exit(1);
        }
        break;
   
    case 6: // ex: ./bpg --rows x --cols y puzzles
        if ((argv[1] == rows && argv[3] == cols)){
            try {
                saida.rows = std::stoi(argv[2]);
                if (saida.rows < 7 || saida.rows > 16) {
                    possibleErrors(2);
                    throw std::invalid_argument("Invalid number of rows");
                }
                saida.cols = std::stoi(argv[4]);
                if (saida.cols < 7 || saida.cols > 16) {
                    possibleErrors(3);
                    throw std::invalid_argument("Invalid number of columns");
                }
                saida.n_puzzles = std::stoi(argv[5]);
                if (saida.n_puzzles > 100) {
                    possibleErrors(4);
                    throw std::invalid_argument("Invalid number of puzzles");
                }
            } catch (const std::exception& e) {
                error_msg();
                exit(1);
            }
        }else{
            std::cout << "bpg ERROR"<< std::endl;
            possibleErrors(1);
            error_msg();
            exit(1);
        }
        break;

    default: // wrong number of arguments
        possibleErrors(1);
        error_msg();
        exit(1);
        break;
    }
  return saida;
}

/*!

 * Saves the puzzles in both Armada and Matrix formats.
 
 * @param run_opt The running options containing the dimensions of the puzzle.
 * @param puzzles The list of puzzles to be saved.
*/
void save_puzzles(const RunningOpt &run_opt, const std::list<bpg::Puzzle> &puzzles) {
    SaveArmada(run_opt, puzzles);
    SaveMatrix(run_opt, puzzles);
}

int main(int argc, char *argv[]) {
  // [1] Print Welcome message.
  welcome_msg();
 
  // [2] Read and validate the running options passed as arguments.
  RunningOpt run_opt = validate_input(argc, argv);
  
  // [3] Generate all puzzles.
    auto puzzles = bpg::Generator::generate(run_opt);

  // [4] Send puzzles to output
    save_puzzles(run_opt, puzzles);
  std::cout << ">>> Job finished!\n\n";

  return EXIT_SUCCESS;
}