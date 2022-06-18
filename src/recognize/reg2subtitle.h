#pragma once

#include "string"
class SrtFile;

namespace SQ
{
std::string AliReg2Srt(const std::string reg_str);
void AliReg2Srt(const std::string &lang, const std::string &reg_str, SrtFile &srt_file);

} // end namespace SQ