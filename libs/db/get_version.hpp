#ifndef QUAMTUM_libs_GET_VERSION_HPP_
#define QUAMTUM_libs_GET_VERSION_HPP_
#include <string>

static int callback(void *data, int argc, char **argv, char **azColName);
int run_sql(std::string pkg);
std::string get_version(std::string package);
#endif // QUAMTUM_libs_GET_VERSION_HPP_
