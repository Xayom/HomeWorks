#include "settings.h"

settings::param::param(const std::string & name, std::string value, settings *parent, bool empty)
{
    this->value = value;
    this->name = name;
    this->parent = parent;
    isBlank = empty;
}

settings::param::operator int() const
{
    if (isBlank)
    {
        throw EmptyProperty(name);
    }
    size_t *pos = new size_t;
    int amont = stoi(value, pos);
    size_t *posd = new size_t;
    double amontd = stod(value, posd);
    if (*pos != value.length() && *posd != value.length()){
        throw std::invalid_argument(value);
    }
    return amont;
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
        throw (name);
    size_t *pos = new size_t;
    double amont = stod(value, pos);
    if (*pos != value.length())
        throw std::invalid_argument(value);
    return amont;
}

settings::param::operator bool() const
{
    if (isBlank)
        throw (name);
    bool result;
    if (value == "true" || value == "1")
        result = true;
    else if (value == "false" || value == "0")
        result = false;
    else
        throw std::invalid_argument(value);
    return result;
}

settings::param &settings::param::operator=(const std::string &newElement)
{
    value = newElement;
    isBlank = false;
    parent->synch(name, value);
    return *this;
}

settings::param &settings::param::operator=(const char *newElement)
{
    value = std::string(newElement);
    isBlank = false;
    parent->synch(name, value);
    return *this;
}

settings::param &settings::param::operator=(int newElement)
{
    value = std::to_string(newElement);
    isBlank = false;
    parent->synch(name, value);
    return *this;
}

settings::param &settings::param::operator=(bool newElement)
{
    value = newElement?"true":"false";
    isBlank = false;
    parent->synch(name, value);
    return *this;
}

settings::param &settings::param::operator=(double newElement)
{
    value = std::to_string(newElement);
    isBlank = false;
    parent->synch(name, value);
    return *this;
}

settings::param &settings::param::operator+=(const std::string &newElement)
{
    if (isBlank)
        throw (name);
    value += newElement;
    parent->synch(name, value);
    return *this;
}

settings::param &settings::param::operator+=(const char *newElement)
{

    if (isBlank)
        throw (name);
    value += std::string(newElement);
    parent->synch(name, value);
    return *this;
}

settings::param &settings::param::operator+=(int newElement)
{
    if (isBlank)
        throw (name);
    value = std::to_string(this->operator int() + newElement);
    parent->synch(name, value);
    return *this;
}

settings::param &settings::param::operator+=(double newElement)
{
    if (isBlank)
        throw (name);
    value = std::to_string(this->operator double() + newElement);
    parent->synch(name, value);
    return *this;
}

settings::param &settings::param::operator-=(int newElement)
{
    if (isBlank)
        throw EmptyProperty(name);
    value = std::to_string(this->operator int() - newElement);
    parent->synch(name, value);
    return *this;
}

settings::param &settings::param::operator*=(int newElement)
{
    if (isBlank)
        throw EmptyProperty(name);
    value = std::to_string(this->operator int() * newElement);
    parent->synch(name, value);
    return *this;
}

settings::param &settings::param::operator-=(double newElement)
{
    if (isBlank)
        throw EmptyProperty(name);
    value = std::to_string(this->operator double() - newElement);
    parent->synch(name, value);
    return *this;
}

settings::param &settings::param::operator/=(int newElement)
{
    if (isBlank)
        throw EmptyProperty(name);
    value = std::to_string(this->operator int() / newElement);
    parent->synch(name, value);
    return *this;
}

settings::param &settings::param::operator*=(double newElement)
{
    if (isBlank)
        throw EmptyProperty(name);
    value = std::to_string(this->operator double() * newElement);
    parent->synch(name, value);
    return *this;
}

settings::param &settings::param::operator/=(double newElement)
{
    if (isBlank)
        throw EmptyProperty(name);
    value = std::to_string(this->operator double() / newElement);
    parent->synch(name, value);
    return *this;
}

settings::param &settings::param::operator|=(bool newElement)
{
    if (isBlank)
        throw EmptyProperty(name);
    value = std::to_string(this->operator bool() || newElement);
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

settings::param &settings::param::operator&=(bool newElement)
{
    if (isBlank)
        throw EmptyProperty(name);
    value = std::to_string(this->operator bool() && newElement);
    parent->synch(name, value);
    return *this;
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
    while (std :: getline(in, current)){
        std :: size_t pos = current.find("=\"");
        std :: string name = current.substr(0, pos);
        std :: size_t lpos = current.find_last_of("\"");
        std :: string value = current.substr(pos + 2, lpos - (pos + 2));
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

settings::param settings::operator[](const std::string &name)
{
    bool empty = false;
    std::string result = "";
    try
    {
        result = list.at(name);
    }
    catch (std::out_of_range) {
        empty = true;
    }
    return param(name, result, this, empty);
}

void settings::synch(std::string & name, std::string & value)
{
    list[name] = value;
    synch();
}

const settings::param settings::operator[](const std::string &name) const
{
    bool empty = false;
    std :: string result = "";
    try {
        result = list.at(name);
    }
    catch (std::out_of_range) {
        empty = true;
    }
    return param(name, result, this, empty);
}

