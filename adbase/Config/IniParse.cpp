#include <adbase/Config.hpp>
#include <adbase/Utility.hpp>
#include <fstream>
#include <iostream>

namespace adbase {
// {{{ IniConfig IniParse::load()

//生成一个变量
IniConfig IniParse::load(const std::string &iniStr) {
	//解析数据成一行一行
	std::vector<std::string> lines = explode(iniStr, '\n');
	IniConfig config;
	std::string section = "";
	for (auto &line : lines) {
		if (isSection(line)) {//判断是否是section
			//得到section，添加section
			section = parseSection(line);
			config.addSection(section);
		} else if (isOption(line)) {//判断是否是option
			//得到section，和option，处理他们
			std::pair<std::string, std::string> option = parseOption(line);
			config.addOption(section, option.first, option.second);
		} else if (isEmptyLine(line) || isComment(line)) {
			//忽略空行和注释
			// ignore it
		} else {
			//不可用，抛出读取的异常
			std::string msg = "Invalid line: " + line;
			throw IniParseReadException(msg.c_str());
		}
	}

	return config;
}

// }}}
// {{{ IniConfig IniParse::loadFile()
//读取文件内容，解析数据
IniConfig IniParse::loadFile(const std::string &filename) {
	std::string result;
	readFile(filename, 65536, &result);
	return load(result);
}

// }}}
// {{{ void IniParse::write()
//输出ini到文件
void IniParse::write(IniConfig &config, const std::string &filename) {
	std::ofstream os;
	//打开文件
	os.open(filename.c_str());
	if (!os.is_open()) {
		std::string msg = "Unable to write " + filename;
		throw IniParseWriteException(msg.c_str());
	}

	try {
		//获取所有的section
		std::vector<std::string> sections = config.sections();
		std::vector<std::string>::const_iterator s = sections.begin();
		for (; s != sections.end(); ++s) {
			//写入section
			os << createSection(*s) << std::endl;
			std::vector<std::string> opts = config.options(*s);
			std::vector<std::string>::const_iterator o = opts.begin();
			for (; o != opts.end(); ++o) {
				std::string value = config.getOption(*s, *o);
				//写入option
				os << createOption(*o, value) << std::endl;
			}
			os << std::endl;
		}
	} catch (...) {//异常处理
		os.close();
		throw;
	}
}

// }}}
// {{{ bool IniParse::isSection()
//判断是section
bool IniParse::isSection(const std::string &str) {
	std::string trimmedStr = trim(str);
	if (trimmedStr[0] == '[' && trimmedStr[trimmedStr.size() - 1] == ']') {
		return true;
	}

	return false;
}

// }}}
// {{{ std::string IniParse::parseSection()
//获取section
std::string IniParse::parseSection(const std::string &str) {
	std::string trimmedStr = trim(str);
	return trimmedStr.substr(1, trimmedStr.size() - 2);
}

// }}}
// {{{ bool IniParse::isOption()
//判断是option
bool IniParse::isOption(const std::string &str) {
	std::string trimmedStr = trim(str);
	std::string::size_type s = trimmedStr.find_first_of("=");
	if (s == std::string::npos) {
		return false;
	}
	std::string key = trim(trimmedStr.substr(0, s));
	if (key == ""){
		return false;	
	}

	return true;
}

// }}}
// {{{ std::pair<std::string, std::string> IniParse::parseOption()
//解析option
std::pair<std::string, std::string> IniParse::parseOption(const std::string &str) {
	std::string trimmedStr = trim(str);
	std::string::size_type s = trimmedStr.find_first_of("=");
	std::string key = trim(trimmedStr.substr(0, s));
	std::string value = trim(trimmedStr.substr(s + 1));

	return std::pair<std::string, std::string>(key, value);
}

// }}}
// {{{ bool IniParse::isComment()

bool IniParse::isComment(const std::string &str) {
	std::string trimmedStr = leftTrim(str);
	if (trimmedStr[0] == ';') {
		return true;
	}

	return false;
}

// }}}
// {{{ bool IniParse::isEmptyLine()
//判断空行
bool IniParse::isEmptyLine(const std::string &str) {
	std::string trimmedStr = leftTrim(str);
	if (trimmedStr == "") {
		return true;
	}

	return false;
}

// }}}
// {{{ std::string IniParse::createSection()
//创建一个section
std::string IniParse::createSection(const std::string &section) {
	return "[" + section + "]";
}

// }}}
// {{{ std::string IniParse::createOption()
//创建一个option
std::string IniParse::createOption(const std::string &optionName, const std::string &optionValue) {
	return optionName + "=" + optionValue;
}

// }}}
}
