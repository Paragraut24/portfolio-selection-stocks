#ifndef IO_H
#define IO_H

#include "utils.h"
#include <string>
#include <vector>

std::vector<Stock> load_csv(const std::string& path);
void save_csv(const std::string& path, const std::vector<Stock>& stocks);

#endif
