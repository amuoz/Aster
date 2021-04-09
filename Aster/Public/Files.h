#pragma once

#include <stdio.h>
#include <fstream>
#include <sstream>
#include <vector>

void ReadFileLines(const char* file, std::vector<std::vector<unsigned int> >& lines); 
