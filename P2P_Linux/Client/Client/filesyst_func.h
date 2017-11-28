#include <vector>
#include <string>

#ifndef FILESYST_FUNC_H
#define FILESYST_FUNC_H

void getFileList(std::vector<std::string>&);
std::string currentIP();
void split(std::vector<std::string>&,std::string);
bool vector_find(const std::vector<std::string>&,const std::string&);

#endif // FILESYST_FUNC_H
