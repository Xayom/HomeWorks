#include "settings.h"

settings::param::param(const string & name, std::string value, const settings *owner)
{
    this->value = value;
    this->name = name;
    this->owner = const_cast<settings* const>(owner);
}

settings::param::operator std::string() const
{
    return string(value);
}
settings::param::operator bool() const
{
    return (value == "true");
}
settings::param & settings::param::operator+=(std::string const & value)
{
    this->value += value;
    owner->set(this->name, this->value);
    return *this;
}
settings::param & settings::param::operator+=(int value)
{
    this->value = to_string(stoi(this->value) + value);
    owner->set(this->name, this->value);
    return *this;
}
settings::param & settings::param::operator+=(double value)
{
    this->value = to_string(stod(this->value) + value);
    owner->set(this->name, this->value);
    return *this;
}
settings::param & settings::param::operator=(int value)
{
    this->value = to_string(value);
    owner->set(this->name, this->value);
    return *this;
}
settings::param & settings::param::operator=(bool value)
{
    this->value = to_string(value);
    owner->set(this->name, this->value);
    return *this;
}
settings::param & settings::param::operator=(double value)
{
    this->value = to_string(value);
    owner->set(this->name, this->value);
    return *this;
}

settings::param & settings::param::operator-=(int value)
{
    this->value = to_string(stoi(this->value) - value);
    owner->set(this->name, this->value);
    return *this;
}
settings::param & settings::param::operator-=(double value)
{
    this->value = to_string(stod(this->value) - value);
    owner->set(this->name, this->value);
    return *this;
}
settings::param::operator int() const
{
    return stoi(value);
}
settings::param::operator double() const
{
    return stod(value);
}
settings::param & settings::param::operator=(char const * value)
{
    return this->operator=(std::string(value));
}
settings::param & settings::param::operator=(std::string const & value)
{
    this->value = value;
    owner->set(this->name, this->value);
    return *this;
}
settings::param & settings::param::operator*=(int value)
{
    this->value = to_string(stoi(this->value) * value);
    owner->set(this->name, this->value);
    return *this;
}
settings::param & settings::param::operator/=(int value)
{
    this->value = to_string(stoi(this->value) / value);
    owner->set(this->name, this->value);
    return *this;
}
settings::param & settings::param::operator/=(double value)
{
    this->value = to_string(stod(this->value) / value);
    owner->set(this->name, this->value);
    return *this;
}

settings::param & settings::param::operator|=(bool value)
{
    this->value = (value ? "true" : this->value);
    owner->set(this->name, this->value);
    return *this;
}
settings::param & settings::param::operator&=(bool value)
{
    this->value = (value ? this->value : "false");
    owner->set(this->name, this->value);
    return *this;
}

settings::param & settings::param::operator*=(double value)
{
    this->value = to_string(stod(this->value) * value);
    owner->set(this->name, this->value);
    return *this;
}

bool settings::param::is_empty() const
{
    return this->value.empty();
}

settings::settings(const string & filename)
{
    if (!std::ifstream(filename).good())
    {
        std::ofstream(filename);
    }
    this->filename = filename;
    reload();
}
string const & settings::get(std::string const & name, std::string const & def) const
{
    return (params.find(name) == params.end() ? def : params.find(name)->second);
}
void settings::set(std::string const &name, std::string const &value)
{
    this->params[name] = value;
    this->updateFile();
}
void settings::reset()
{
    std::ofstream(filename, std::ofstream::trunc);
    params.clear();
}
void settings::reload()
{
    params.clear();
    std::ifstream input(filename);
    string name, value;
    while (getline(input, name))
    {
        getline(input, value);
        params[name] = value;
    }
    input.close();
}

const settings::param settings::operator[](std::string const &name) const
{
    return param(name, params.find(name)->second, this);
}

settings::param settings::operator[](std::string const & name)
{
    return param(name, params.find(name)->second, this);
}

void settings::updateFile()
{
    std::ofstream output(filename);
    for (auto item : params)
    {
        output << item.first << endl;
        output << item.second << endl;
    }
    output.close();
}
