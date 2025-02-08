#include <iostream> // std::cout, std::endl
#include <string>
#include <fstream>
#include <sstream>
#include <list>

#include "include/bpg.h"
#include "include/common.h"

namespace bpg{
    /*!
        * Saves content into a file.
        * @param FileName The name of the file to save the content.
        * @param content The content to be saved into the file.
        * @return True if the content was successfully saved, false otherwise.
    */
    bool saveInFile(const std::string &FileName, const std::string &content) {
        std::ofstream arquivo(FileName.c_str(), std::ios_base::app); // Abrindo o arquivo em modo de acrescentar (append)
        if (!arquivo.is_open()) {
            std::cerr << "Error trying to open file: " << FileName << std::endl;
            return false;
        }
        arquivo << content; // Escrevendo o conteúdo no arquivo
        arquivo << '\n';
        arquivo.close(); // Fechando o arquivo
        return true;
    }

    /*!
    * Saves the puzzles in Armada type to a file.
    * @param run_opt The running options containing the number of puzzles.
    * @param puzzleList The list of puzzles to be saved.
    */
    void SaveArmada(const RunningOpt &run_opt, std::list<Puzzle> puzzleList) {
        std::string NumberOfPuzzles = std::to_string(run_opt.n_puzzles);

        std::string rowAndCol;
        std::stringstream ss;
        ss << run_opt.rows << ' ' << run_opt.cols;
        rowAndCol = ss.str();

        saveInFile("../output/puzzles_armada.bp", NumberOfPuzzles);
        std::list<Puzzle>::iterator it;
        for (it = puzzleList.begin(); it != puzzleList.end(); ++it) {
            saveInFile("../output/puzzles_armada.bp", rowAndCol);
            size_t start = 0, end;
            while ((end = it->puzzleArmada.find('-', start)) != std::string::npos) {
                saveInFile("../output/puzzles_armada.bp", it->puzzleArmada.substr(start, end - start));
                start = end + 1; // Avança para o próximo caractere após o delimitador
            }
            saveInFile("../output/puzzles_armada.bp", " \n");
        }
    }

    /*!
    * Generates the first line for matrix puzzles to adjust the spacing of column numbers.

    * @param run_opt The running options containing the dimensions of the puzzle.

    * @return A string representing the first line with adjusted column numbers.
    */
    std :: string FirstLineMatrix(const RunningOpt &run_opt){
        std::string line1Cols;
        std::stringstream oss;
        oss << "                      ";
        for(int k = 0; k < (run_opt.cols-9); k++){
            oss << "1 ";
        }
        line1Cols = oss.str();
        return line1Cols;
    }

    /*!

    * Generates the second line for matrix puzzles to display column numbers properly.

    * @param run_opt The running options containing the dimensions of the puzzle.
    
    * @return A string representing the second line with properly formatted column numbers.
    */
    std :: string SecondLineMatrix(const RunningOpt &run_opt){
        std::string line2Cols;
        std::stringstream oss; 
        oss << "    ";
            for (int h = 0; h < run_opt.cols; h++){
                if(h+1<=9){
                    oss << h+1 << " ";
                }else{
                    oss << h - 9 << " ";
                }
            }
        line2Cols = oss.str();
        return line2Cols;
    }

    /*!
    * Generates a string representation of the puzzle in matrix format.
    *
    * @param puzzle The puzzle to be converted to a string.
    * @param run_opt The running options containing the dimensions of the puzzle.
    *
    * @return A string representing the puzzle in matrix format.
    */
    std::string MatrixString(Puzzle &puzzle, const RunningOpt &run_opt){
        std::string key = puzzle.puzzleKey;

        std::string water = "\u00B7";           // ·
        std::string square = "\u25FC";          // ◼
        std::string submarine = "\u25CF";       // ●
        std::string right_triangle = "\u25B6";   // ▶
        std::string left_triangle = "\u25C0";   // ◀
        std::string up_triangle = "\u25B2";     // ▲
        std::string down_triangle = "\u25BC";   // ▼

        std::string stringfication;
        std::stringstream oss;
        int lineCount = 1;

        
        for(int j = 0; j < int(key.length()); j++){
            if (j % run_opt.cols == 0) {
                if (lineCount < 10) {
                      oss << " " << lineCount << "[ ";
                lineCount ++;
                } else {
                  oss << lineCount << "[ ";
                }
            }
            if(j == 0){
                if (key[0] == ' ') {
                    oss << water << " ";
                } else if(key[0] == 'S'){
                    oss << submarine << " ";
                } else if(key[0] == 'B'|| key[0] == 'D'|| key[0] == 'C'){
                    if(key[1] == key[0]){
                        oss << left_triangle << " ";
                    }else if(key[run_opt.cols] == key[0]){
                        oss << up_triangle << " ";
                    }
                }
            }else{
                if (key[j] == ' ') {
                    oss << water << " ";
                } else if(key[j] == 'S'){
                    oss << submarine << " ";
                }else if(key[j] == 'B'|| key[j] == 'D'|| key[j] == 'C'){
                    if(key[j+1] == key[j] && (key[j-1] == ' ' || j-1 < 0)){
                        oss << left_triangle << " ";
                    }else if(key[j + run_opt.cols] == key[j] && (key[j - run_opt.cols] == ' ' || j - run_opt.cols< 0)){
                        oss << up_triangle << " ";
                    }else if(key[j - 1] == key[j] && (key[j+1] == ' ' || j+1 > int(key.length()))){
                        oss << right_triangle << " ";
                    }else if(key[j - run_opt.cols] == key[j] && (key[j + run_opt.cols] == ' ' || j + run_opt.cols > int(key.length()))){
                        oss << down_triangle << " ";
                    }else if(((key[j - run_opt.cols] == key[j]) && (key[j + run_opt.cols] == key[j])) || ((key[j+1] == key[j]) && (key[j-1] == key[j]))){
                        oss << square << " ";
                    }
                }
            }
            if(j % run_opt.cols== run_opt.cols - 1){
                oss << "]";
                oss << "\n";
            }
        }
        stringfication = oss.str();
        return stringfication;
    }

    /*!
    * Saves the matrix puzzles to a file.
    * @param run_opt The running options containing the number of puzzles and dimensions.
    * @param puzzleList The list of puzzles to be saved.
    */
    void SaveMatrix(const RunningOpt &run_opt, std::list<Puzzle> puzzleList) {
        std::string NumberOfPuzzles = std::to_string(run_opt.n_puzzles);

        std::string rowAndCol;
        std::stringstream ss;
        ss << run_opt.rows << ' ' << run_opt.cols;
        rowAndCol = ss.str();
        
        std:: string numOfCols = FirstLineMatrix(run_opt);
        std:: string numOfCols2 = SecondLineMatrix(run_opt);

        saveInFile("../output/puzzles_matrix.bp", NumberOfPuzzles);
        std::list<Puzzle>::iterator it;
        for (it = puzzleList.begin(); it != puzzleList.end(); ++it) {
            saveInFile("../output/puzzles_matrix.bp", rowAndCol);
            if (run_opt.cols>9){
                saveInFile("../output/puzzles_matrix.bp", numOfCols);
            }
            saveInFile("../output/puzzles_matrix.bp", numOfCols2);

            saveInFile("../output/puzzles_matrix.bp", MatrixString(*it,run_opt));
        }
    }
}