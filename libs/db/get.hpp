#ifndef QUAMTUM_libs_GET_HPP_
#define QUAMTUM_libs_GET_HPP_
#include <string>

static int callback(void *data, int argc, char **argv, char **azColName);
int run_sql(std::string pkg, std::string version);
std::string get_url(std::string package, std::string version);
#endif // QUAMTUM_libs_GET_HPP_
