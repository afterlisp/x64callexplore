#include <boost/regex.hpp>
#include <iostream>
#include <string>

// >test_boost.exe
// Subject: test Re: test2
//>test Re: test2
// Subject: test
// test
// CTRL-C to exit
int main()
{
    std::string line;
    boost::regex pat( "^Subject: (Re: |Aw: )*(.*)" );

    while (std::cin)
    {
        std::getline(std::cin, line);
        boost::smatch matches;
        if (boost::regex_match(line, matches, pat))
            std::cout << matches[2] << std::endl;
    }
}