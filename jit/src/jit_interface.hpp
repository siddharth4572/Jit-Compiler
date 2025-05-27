#pragma once
#include <string>

std::string generateIR(const std::string& source_code);
std::string executeJIT(const std::string& source_code);

std::string compile_and_get_ir(const std::string& source_code);
std::string run_jit_and_get_output(const std::string& source_code);

