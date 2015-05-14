#include "settings.h"

settings::param::param(const std::string & name, std::string value, settings *parent, bool emp)
{
    this->value = value;
    this->name = name;
    this->parent = parent;
    isBlank = emp;
}

settings::param::operator int() const
{
    if (isBlank){
        throw EmptyProperty(name);
    }
    size_t *pos = new size_t;
    int tmp = stoi(value, pos);
    size_t *posd = new size_t;
    double tmpd = stod(value, posd);
    if (*pos != value.length() && *posd != value.length()){
        throw std::invalid_argument(value);
    }
    return tmp;
}

settings::param::operator std::string() const
{
    if (isBlank)
        throw EmptyProperty(name);
    return value;
}

settings::param::operator double() const
{
    if (isBlank)
        throw EmptyProperty(name);
    size_t *pos = new size_t;
    double tmp = stod(value, pos);
    if (*pos != value.length())
        throw std::invalid_argument(value);
    return tmp;
}

settings::param::operator bool() const
{
    if (isBlank)
        throw EmptyProperty(name);
    bool result;
    if (value == "true" || value == "1")
        result = true;
    else if (value == "false" || value == "0")
        result = false;
    else
        throw std::invalid_argument(value);
    return result;
}

settings::param &settings::param::operator=(const std::string &newValue)
{
    value = newValue;
    isBlank = false;
    parent->synch(name, value);
    return *this;
}

settings::param &settings::param::operator=(const char *newValue)
{
    value = std::string(newValue);
    isBlank = false;
    parent->synch(name, value);
    return *this;
}

settings::param &settings::param::operator=(int newValue)
{
    value = std::to_string(newValue);
    isBlank = false;
    parent->synch(name, value);
    return *this;
}

settings::param &settings::param::operator=(bool newValue)
{
    value = newValue?"true":"false";
    isBlank = false;
    parent->synch(name, value);
    return *this;
}

settings::param &settings::param::operator=(double newValue)
{
    value = std::to_string(newValue);
    isBlank = false;
    parent->synch(name, value);
    return *this;
}

settings::param &settings::param::operator+=(const std::string &newValue)
{
    if (isBlank)
        throw EmptyProperty(name);
    value += newValue;
    parent->synch(name, value);
    return *this;
}

settings::param &settings::param::operator+=(const char * newValue)
{

    if (isBlank)
        throw EmptyProperty(name);
    value += std::string(newValue);
    parent->synch(name, value);
    return *this;
}

settings::param &settings::param::operator+=(int newValue)
{
    if (isBlank)
        throw EmptyProperty(name);
    value = std::to_string(this->operator int() + newValue);
    parent->synch(name, value);
    return *this;
}

settings::param &settings::param::operator+=(double newValue)
{
    if (isBlank)
        throw EmptyProperty(name);
    value = std::to_string(this->operator double() + newValue);
    parent->synch(name, value);
    return *this;
}

settings::param &settings::param::operator-=(int newValue)
{
    if (isBlank)
        throw EmptyProperty(name);
    value = std::to_string(this->operator int() - newValue);
    parent->synch(name, value);
    return *this;
}

settings::param &settings::param::operator*=(int newValue)
{
    if (isBlank)
        throw EmptyProperty(name);
    value = std::to_string(this->operator int() * newValue);
    parent->synch(name, value);
    return *this;
}

settings::param &settings::param::operator-=(double newValue)
{
    if (isBlank)
        throw EmptyProperty(name);
    value = std::to_string(this->operator double() - newValue);
    parent->synch(name, value);
    return *this;
}

settings::param &settings::param::operator*=(double newValue)
{
    if (isBlank)
        throw EmptyProperty(name);
    value = std::to_string(this->operator double() * newValue);
    parent->synch(name, value);
    return *this;
}

settings::param &settings::param::operator/=(int newValue)
{
    if (isBlank)
        throw EmptyProperty(name);
    value = std::to_string(this->operator int() / newValue);
    parent->synch(name, value);
    return *this;
}

settings::param &settings::param::operator/=(double newValue)
{
    if (isBlank)
        throw EmptyProperty(name);
    value = std::to_string(this->operator double() / newValue);
    parent->synch(name, value);
    return *this;
}

settings::param &settings::param::operator|=(bool newValue)
{
    if (isBlank)
        throw EmptyProperty(name);
    value = std::to_string(this->operator bool() || newValue);
    parent->synch(name, value);
    return *this;
}

settings::param &settings::param::operator&=(bool newValue)
{
    if (isBlank)
        throw EmptyProperty(name);
    value = std::to_string(this->operator bool() && newValue);
    parent->synch(name, value);
    return *this;
}

bool settings::param::is_empty() const
{
    return isBlank;
}


settings::settings(const std::string &filename)
{
    if (!std::ifstream(filename).good()){
        std::ofstream(filename);
    }
    this->filename = filename;
    reload();
}

const std::string &settings::get(const std::string &name, const std::string &def) const
{
    try {
        return list.at(name);
    }
    catch (std::out_of_range) {
        return def;
    }
}

void settings::set(const std::string &name, const std::string &value)
{
    list[name] = value;
    synch();
}

void settings::reset()
{
    std::ofstream(filename, std::ofstream::trunc);
    list.clear();
}

void settings::reload()
{
    list.clear();
    std::ifstream in(filename);
    std::string current;
    while (std::getline(in, current)){
        std::size_t pos = current.find("=\"");
        std::string name = current.substr(0, pos);
        std::size_t lpos = current.find_last_of("\"");
        std::string value = current.substr(pos + 2, lpos - (pos + 2));
        list[name] = value;
    }
}

void settings::synch()
{
    std::ofstream out(filename);
    for (auto it = list.begin(); it != list.end(); it++){
        std::string val = it->second;
        out << it->first << "=\"" << val << "\"" << std::endl;
    }
    out.close();
}

const settings::param settings::operator[](const std::string &name) const
{
    bool emp = false;
    std::string result = "";
    try {
        result = list.at(name);
    }
    catch (std::out_of_range) {
        emp = true;
    }
    return param(name, result, this, emp);
}

settings::param settings::operator[](const std::string &name)
{
    bool emp = false;
    std::string result = "";
    try {
        result = list.at(name);
    }
    catch (std::out_of_range) {
        emp = true;
    }
    return param(name, result, this, emp);
}

void settings::synch(std::string & name, std::string & value)
{
    list[name] = value;
    synch();
}
