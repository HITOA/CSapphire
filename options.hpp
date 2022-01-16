#pragma once
#include <boost/program_options.hpp>

namespace bpo = boost::program_options;

/*
Options:
	[-h, --help]			Produce help message
	[-V, --version]			Display version information
	[-u8, --utf8]			Read input file with u8 encoding
	[-u16, --utf16]			Read input file with u16 encoding
	[-u32, --utf32]			Read input file with u32 encoding
	[-i, --input]			Specify input file
	[-t, --tokenfile]       Produce token file
*/

class Options {
public:
	enum class Encoding {
		utf8,
		utf16,
		utf32
	};
public:
	Options();
	~Options() = default;

	Options(const Options& rhs) = delete;
	Options& operator=(const Options& rhs) = delete;
public:
	inline Encoding GetEncoding() const { return u8encoding ? Encoding::utf8 : u16encoding ? Encoding::utf16 : Encoding::utf32; }
	inline std::string GetInputFile() const { return inputFile; }
	inline bool TokenFile() const { return tokenfile; }
	inline bool AstFile() const { return astfile; }
public:
	bool Parse(int argc, const char* argv[]);
private:
	void Init();
private:
	bpo::options_description desc;
	//Options
	bool u8encoding;
	bool u16encoding;
	bool u32encoding;
	std::string inputFile;
	bool tokenfile;
	bool astfile;
};