#include "options.hpp"
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/format.hpp>
#include <iostream>
#include "versioninfo.h"

Options::Options()
{
	Init();
}
bool Options::Parse(int argc, const char* argv[])
{
	bpo::variables_map vMap;
	bpo::store(bpo::parse_command_line(argc, argv, desc), vMap);
	bpo::notify(vMap);

	if (vMap.count("help")) {
		std::cout << desc << std::endl;
		return false;
	}

	if (vMap.count("version")) {
		std::cout << boost::format("%1% : %2%.%3%.%4%.%5%") 
			% VInfo::name 
			% VInfo::major 
			% VInfo::minor 
			% VInfo::bfix 
			% VInfo::build << std::endl;
		return false;
	}

	if (!(u8encoding || u16encoding || u32encoding))
		u8encoding = true;

	if (!(u8encoding ^ u16encoding ^ u32encoding))
		throw std::logic_error(std::string("Conflicting encoding."));

	if (vMap.count("input") && !vMap["input"].defaulted()) {
		boost::algorithm::trim(inputFile);
		if (!boost::filesystem::is_regular_file(inputFile)) {
			throw std::runtime_error("Input is not a valid file.");
		}
	}
	else {
		throw std::logic_error(std::string("No input file specified."));
	}

	return true;
}

void Options::Init()
{
	bpo::options_description options("Options");

	options.add_options()
		("help,h", "Produce help message")
		("version,V", "Display version information")
		("utf8", bpo::bool_switch(&u8encoding)->default_value(false), "Read input file with u8 encoding (default)")
		("utf16", bpo::bool_switch(&u16encoding)->default_value(false), "Read input file with u16 encoding")
		("utf32", bpo::bool_switch(&u32encoding)->default_value(false), "Read input file with u32 encoding")
		("input,i", bpo::value<std::string>(&inputFile), "Specify input file")
		("tokenfile", bpo::bool_switch(&tokenfile)->default_value(false), "Produce token file");

	desc.add(options);
}
