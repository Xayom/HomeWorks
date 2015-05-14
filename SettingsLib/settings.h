#ifndef SETTINGS_H
#define SETTINGS_H

#include <exception>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <map>
#include <string>

class EmptyProperty  : public std::exception
{
    public:
        EmptyProperty(const std::string & name)throw():error_message("Trying to cast empty property: " + name + "."){}
        virtual const char* what()const throw(){return error_message.c_str();}
        ~EmptyProperty()throw(){}
    private:
        std::string error_message;
};

class settings {
    public:
        class param {
            friend class settings;
            private:
                param(param const &);
                param(const std::string & name, std::string value, settings *parent, bool empty = false);
                std::string value;
                std::string name;
                bool isBlank;
                settings *parent;
            public:
                /**
                 * Cast value to std::string
                 * @throw EmptyProperty
                 * @throw std::invalid_argument
                 */
                operator std::string() const;

                /**
                 * Cast value to int
                 * @throw EmptyProperty
                 * @throw std::invalid_argument
                 */
                operator int() const;

                /**
                 * Cast value to bool
                 * @throw EmptyProperty
                 * @throw std::invalid_argument
                 */
                operator bool() const;

                /**
                 * Cast value to double
                 * @throw EmptyProperty
                 * @throw std::invalid_argument
                 */
                operator double() const;

                /**
                 * Assigns the given std::string to this value and returns a reference to this value.
                 * @return reference to this
                 */
                param & operator=(std::string const &);

                /**
                 * Assigns the given char* to this value and returns a reference to this value.
                 * @return reference to this
                 */
                param & operator=(const char*);

                /**
                 * Assigns the given int to this value and returns a reference to this value.
                 * @return reference to this
                 */
                param & operator=(int);

                /**
                 * Assigns the given bool to this value and returns a reference to this value.
                 * @return reference to this
                 */
                param & operator=(bool);

                /**
                 * Assigns the given double to this value and returns a reference to this value.
                 * @return reference to this
                 */
                param & operator=(double);


                /**
                 * Appends the std::string onto the end of this value and returns a reference to this value.
                 * @throw EmptyProperty
                 * @return reference to this
                 */
                param & operator+=(std::string const &);

                /**
                 * Appends the char* onto the end of this value and returns a reference to this value.
                 * @throw EmptyProperty
                 * @return reference to this
                 */
                param & operator+=(const char*);

                /**
                 * Adds int to this value if this value can be casted to int.
                 * @throw EmptyProperty
                 * @throw std::invalid_argument
                 * @return reference to this
                 */
                param & operator+=(int);

                /**
                 * Adds double to this value if this value can be casted to double.
                 * @throw EmptyProperty
                 * @throw std::invalid_argument
                 * @return reference to this
                 */
                param & operator+=(double);


                /**
                 * Subtracts int of this value if this value can be casted to int.
                 * @throw EmptyProperty
                 * @throw std::invalid_argument
                 * @return reference to this
                 */
                param & operator-=(int);

                /**
                 * Subtracts double of this value if this value can be casted to double.
                 * @throw EmptyProperty
                 * @throw std::invalid_argument
                 * @return reference to this
                 */
                param & operator-=(double);


                /**
                 * Multiplies int by this value if this value can be casted to int.
                 * @throw EmptyProperty
                 * @throw std::invalid_argument
                 * @return reference to this
                 */
                param & operator*=(int);

                /**
                 * Multiplies double of this value if this value can be casted to double.
                 * @throw EmptyProperty
                 * @throw std::invalid_argument
                 * @return reference to this
                 */
                param & operator*=(double);


                /**
                 * Divides this value by int if this value can be casted to int.
                 * @throw EmptyProperty
                 * @throw std::invalid_argument
                 * @return reference to this
                 */
                param & operator/=(int);

                /**
                 * Divides this value by double if this value can be casted to double.
                 * @throw EmptyProperty
                 * @throw std::invalid_argument
                 * @return reference to this
                 */
                param & operator/=(double);


                /**
                 * Assigns given value and this value if this value can be casted to bool.
                 * @throw EmptyProperty
                 * @throw std::invalid_argument
                 * @return reference to this
                 */
                param & operator|=(bool);

                /**
                 * Assigns given value or this value if this value can be casted to bool.
                 * @throw EmptyProperty
                 * @throw std::invalid_argument
                 * @return reference to this
                 */
                param & operator&=(bool);

                /**
                 * Returns false if this property is empty otherwise returns true.
                 */
                bool is_empty() const;
        };

        // Main functions

        friend class param;

        /**
         * Construct settings store
         * and load data from file (if exists)
         * \param filename Path to file with settings
         */
        settings(std::string const & filename);
        /**
         * Get setting value
         * \param name Setting unique identifier
         * \param def Default setting value
         * \return Stored value for given name or default value
         */
        std::string const & get(std::string const & name,
            std::string const & def = "") const;
        /**
         * Set or replace setting value and save changes to file
         * \param name Setting unique identifier
         * \param value New setting value
         */
        void set(std::string const & name,
            std::string const & value);
        /**
         * Reset all settings
         */
        void reset();
        /**
         * Reload all settings from file
         */
        void reload();

        // Advanced funсtions

        /**
          * Get constant setting wrapper
          * \param name Setting unique identifier
          */
        const param operator[](std::string const & name) const;
        /**
          * Get constant setting wrapper
          * \param name Setting unique identifier
          */
        param operator[](std::string const & name);

private:
        void synch(std::string &name, std::string &value);
        void synch();
        std::string filename;
        std::map<std::string, std::string> list;
};
#endif // SETTINGS_SETTINGS_H
