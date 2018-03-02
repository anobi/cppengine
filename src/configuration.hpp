#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>

class Configuration {
    public:
        static Configuration& Get()
        {
            static Configuration instance;
            return instance;
        }

        std::string workingDirectory;
    private:
        Configuration() {};
        Configuration(const Configuration&);
        Configuration& operator = (const Configuration&);
};
#endif