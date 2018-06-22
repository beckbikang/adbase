#include <adbase/Config.hpp>

namespace adbase {
// {{{ bool IniConfig::hasSection()

//判断是否存在某个section
bool IniConfig::hasSection(const std::string &sectionName) const {
    if (config.find(sectionName) == config.end()) {
        return false;
    }

    return true;
}

// }}}
// {{{ bool IniConfig::hasOption()
//判断是否存在
bool IniConfig::hasOption(const std::string &sectionName, const std::string &optionName) const {
    if (!hasSection(sectionName)) {
        return false;
    }
    //at会作下标检查
    std::map<std::string, std::string> options = config.at(sectionName);
    //找不到报错
    if (options.find(optionName) == options.end()) {
        return false;
    }

    return true;
}

// }}}
// {{{ std::string IniConfig::getOption()
//获取option
std::string IniConfig::getOption(const std::string &sectionName, const std::string &optionName) const {
    validateOption(sectionName, optionName);

    return config.at(sectionName).at(optionName);
}

// }}}
// {{{ uint32_t IniConfig::getOptionUint32()
//读取int32
uint32_t IniConfig::getOptionUint32(const std::string &sectionName, const std::string &optionName) {
	std::string value = getOption(sectionName, optionName);

	errno = 0;
	uint32_t ivalue = static_cast<uint32_t>(strtoul(value.c_str(), nullptr, 10));
	if (errno != 0) {
		std::string msg = "Section (" + sectionName + "), Option (" + optionName + ") type is invalid";
		throw InvalidOptionException(msg.c_str());
	}
    return ivalue;
}

// }}}
// {{{ uint64_t IniConfig::getOptionUint64()
//读取int64
uint64_t IniConfig::getOptionUint64(const std::string &sectionName, const std::string &optionName) {
	std::string value = getOption(sectionName, optionName);

	errno = 0;
	uint64_t ivalue = static_cast<uint64_t>(strtoull(value.c_str(), nullptr, 10));
	if (errno != 0) {
		std::string msg = "Section (" + sectionName + "), Option (" + optionName + ") type is invalid";
		throw InvalidOptionException(msg.c_str());
	}
    return ivalue;
}

// }}}
// {{{ bool IniConfig::getOptionBool()
//读取bool
bool IniConfig::getOptionBool(const std::string &sectionName, const std::string &optionName) {
	std::string value = getOption(sectionName, optionName);
	if (value.compare("yes") == 0) {
		return true;
	} else {
		return false;
	}
}

// }}}
// {{{ std::vector<std::string> IniConfig::sections() const
//返回所有的section
std::vector<std::string> IniConfig::sections() const {
    std::vector<std::string> sections;
    std::map<std::string, std::map<std::string, std::string> >::const_iterator i = config.begin();
    for (; i != config.end(); ++i) {
        sections.push_back(i->first);
    }
    return sections;
}

// }}}
// {{{ std::vector<std::string> IniConfig::options()
//返回所有的options
std::vector<std::string> IniConfig::options(const std::string &sectionName) const {
    validateSection(sectionName);

    std::map<std::string, std::string> opts = config.at(sectionName);
    std::vector<std::string> options;
    std::map<std::string, std::string>::const_iterator i = opts.begin();
    for (; i != opts.end(); ++i) {
        options.push_back(i->first);
    }

    return options;
}

// }}}
// {{{ void IniConfig::setOption()
//设置option
void IniConfig::setOption(const std::string &sectionName, const std::string &optionName, const std::string &optionValue) {
    validateOption(sectionName, optionName);

    std::map<std::string, std::string> &options = config[sectionName];
    options[optionName] = optionValue;
}

// }}}
// {{{ void IniConfig::removeSection()
//删除所有的section
void IniConfig::removeSection(const std::string &sectionName) {
    validateSection(sectionName);

    config.erase(sectionName);
}

// }}}
// {{{ void IniConfig::removeOption()
//删除option
void IniConfig::removeOption(const std::string &sectionName, const std::string &optionName) {
    validateOption(sectionName, optionName);

    config[sectionName].erase(optionName);
}

// }}}
// {{{ void IniConfig::addSection()
//添加sction
void IniConfig::addSection(const std::string &sectionName) {
    if (hasSection(sectionName)) {
        std::string msg = "Duplicate section (" + sectionName + ") found";
        throw DuplicateSectionException(msg.c_str());
    }

    std::map<std::string, std::string> emptyMap;
    config[sectionName] = emptyMap;
}

// }}}
// {{{ void IniConfig::addOption()
//添加option
void IniConfig::addOption(const std::string &sectionName, const std::string &optionName, const std::string &optionValue) {
    validateSection(sectionName);

    std::map<std::string, std::string> &options = config[sectionName];
    if (hasOption(sectionName, optionName)) {
        std::string msg = "Duplicate option (" + optionName + ") found";
        throw DuplicateOptionException(msg.c_str());
    }
    options[optionName] = optionValue;
}

// }}}
// {{{ void IniConfig::validateOption()
//不可用就抛出异常
void IniConfig::validateOption(const std::string &sectionName, const std::string &optionName) const {
    if (!hasOption(sectionName, optionName)) {
        std::string msg = "Section (" + sectionName + "), Option (" + optionName + ") not found";
        throw InvalidOptionException(msg.c_str());
    }
}

// }}}
// {{{ void IniConfig::validateSection()
//不可用就抛出异常
void IniConfig::validateSection(const std::string &sectionName) const {
    if (!hasSection(sectionName)) {
        std::string msg = "Section (" + sectionName + ") not found";
        throw InvalidSectionException(msg.c_str());
    }
}

// }}}
}

