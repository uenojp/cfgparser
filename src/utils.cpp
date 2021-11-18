#include "utils.h"

std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream iss(str);
    while (std::getline(iss, token, delimiter)) {
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }
    return tokens;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
    os << "[ ";
    for (auto e : v)
        os << e << " ";
    return os << "]";
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::set<T>& v) {
    os << "[ ";
    for (auto e : v)
        os << e << " ";
    return os << "]";
}

template <typename T, typename U>
std::ostream& operator<<(std::ostream& os, const std::pair<T, U> p) {
    return os << "[ " << p.first << " " << p.second << " ]";
}
