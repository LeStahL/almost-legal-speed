#include "level_importer.h"

#include <fstream>

enum ImporterState {
    BlockDescriptor,
    LevelDescriptor
};

constexpr auto level_importer::level_importer(std::string& pathToFile)
{
    std::ifstream infile(pathToFile);

    int state = 0;



    std::vector<Level> level;
    std::vector<Block> blocks;
    ImporterState state;


    for(std::string line; getline( infile, line ); )
    {
        if (state == BlockLength) {
            blocks.emplace_back()
        }

    }


    return nullptr;
}
