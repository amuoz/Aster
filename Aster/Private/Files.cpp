#include "Files.h"

#include <stdio.h>
#include <fstream>
#include <sstream>
#include <vector>

void ReadFileLines(std::string file, std::vector<std::vector<unsigned int> >& lines)
{
  unsigned int word;
  std::string line;
  std::ifstream fstream(file);
  
  if (fstream)
  {
      while (std::getline(fstream, line)) // read each line from file
      {
          std::istringstream sstream(line);
          std::vector<unsigned int> row;
          while (sstream >> word) // read each word separated by spaces
          {
              row.push_back(word);
          }
          lines.push_back(row);
      }
  }
}
