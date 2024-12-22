#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>
#include <regex>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

struct FileInfo {
    std::string fileName;
    std::string folderName;
    std::string untrustedLibs;
};

std::string wrapText(const std::string& text, size_t width) {
    if (text.length() <= width) return text;
    
    std::stringstream ss;
    std::string remaining = text;
    bool firstLine = true;
    
    while (!remaining.empty()) {
        if (!firstLine) {
            ss << "\n" << std::string(2, ' ') << "| " << std::string(25, ' ')  // Align with column start
               << "| " << std::string(20, ' ');  // folder name width
        }
        
        size_t splitPos = remaining.length() > width ? remaining.substr(0, width).find_last_of(" ,") : remaining.length();
        if (splitPos == std::string::npos) splitPos = width;
        
        ss << (firstLine ? "" : "") << remaining.substr(0, splitPos);
        if (splitPos < remaining.length()) {
            remaining = remaining.substr(splitPos + 1);
        } else {
            remaining = "";
        }
        firstLine = false;
    }
    return ss.str();
}

void printTableHeader() {
    std::cout << std::string(100, '=') << "\n";
    std::cout << "| " << std::left << std::setw(30) << "File Name"
              << "| " << std::setw(25) << "Folder Name"
              << "| " << std::setw(30) << "Untrusted Libraries |\n";
    std::cout << std::string(100, '-') << "\n";
}

void printFileInfo(const FileInfo& file) {
    const int w1 = 30;  // File name width
    const int w2 = 25;  // Folder name width
    const int w3 = 30;  // Libraries width
    
    // Wrap long texts
    std::string wrappedUntrusted = wrapText(file.untrustedLibs, w3);
    
    // Split into lines
    std::stringstream ssUntrust(wrappedUntrusted);
    std::string lineUntrust;
    
    // Print first line
    std::cout << "| " << std::left << std::setw(w1) << file.fileName
              << "| " << std::setw(w2) << file.folderName
              << "| " << std::setw(w3) << (std::getline(ssUntrust, lineUntrust) ? lineUntrust : "N/A") << "|\n";
    
    // Print continuation lines if any column has wrapped text
    while (std::getline(ssUntrust, lineUntrust)) {
        std::cout << "| " << std::string(w1, ' ')  // Empty file name column
                  << "| " << std::string(w2, ' ')  // Empty folder name column
                  << "| " << std::setw(w3) << lineUntrust << "|\n";
    }
}

class Program {
private:
    std::string code;
    std::vector<std::string> trustedLibraries;

public:
    Program(const std::string& codeContent)
        : code(codeContent), trustedLibraries({
            "algorithm", "any", "array", "atomic", "barrier", "bitset",
            "bit", "charconv", "chrono", "codecvt", "compare", "complex",
            "concepts", "condition_variable", "coroutine", "deque",
            "exception", "execution", "filesystem", "format",
            "forward_list", "fstream", "functional", "future",
            "initializer_list", "iomanip", "ios", "iosfwd", "iostream",
            "istream", "iterator", "latch", "limits", "list", "locale",
            "map", "memory_resource", "memory", "mutex", "new",
            "numbers", "numeric", "optional", "ostream", "queue",
            "random", "ranges", "ratio", "regex", "scoped_allocator",
            "semaphore", "set", "shared_mutex", "span", "sstream",
            "stack", "stdexcept", "stop_token", "streambuf",
            "string_view", "string", "strstream", "syncstream",
            "system_error", "thread", "tuple", "type_traits",
            "typeindex", "typeinfo", "unordered_map", "unordered_set",
            "utility", "valarray", "variant", "vector", "version"
        }) {}

    std::vector<std::string> getAllIncludedLibraries() {
        std::regex includeRegex("#include\\s*<(\\w+\\.\\w+|\\w+)>");
        std::sregex_iterator iter(code.begin(), code.end(), includeRegex);
        std::sregex_iterator end;

        std::vector<std::string> includedLibraries;

        while (iter != end) {
            std::string library = (*iter)[1].str();
            includedLibraries.push_back(library);
            ++iter;
        }

        return includedLibraries;
    }

    std::vector<std::string> checkUntrustedLibraries() {
        std::vector<std::string> untrustedLibraries;
        auto includedLibraries = getAllIncludedLibraries();

        for (const auto& lib : includedLibraries) {
            if (std::find(trustedLibraries.begin(), trustedLibraries.end(), lib) == trustedLibraries.end()) {
                untrustedLibraries.push_back(lib);
            }
        }

        return untrustedLibraries;
    }
};

std::string readFileContent(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filePath);
    }

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return content;
}

void processFile(const fs::path& filePath, std::vector<FileInfo>& files) {
    try {
        std::string code = readFileContent(filePath.string());
        Program program(code);
        auto untrustedLibraries = program.checkUntrustedLibraries();

        std::string untrustedLibs;
        if (untrustedLibraries.empty()) {
            untrustedLibs = "N/A";
        } else {
            for (const auto& lib : untrustedLibraries) {
                untrustedLibs += lib + ", ";
            }
            untrustedLibs.pop_back();
            untrustedLibs.pop_back();
        }

        files.push_back({filePath.filename().string(), filePath.parent_path().filename().string(), untrustedLibs});

    } catch (const std::exception& e) {
        std::cerr << "Error processing file " << filePath << ": " << e.what() << "\n";
    }
}

void processDirectory(const fs::path& directoryPath, std::vector<FileInfo>& files) {
    for (const auto& entry : fs::recursive_directory_iterator(directoryPath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".cpp") {
            processFile(entry.path(), files);
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <directory path>" << std::endl;
        return 1;
    }

    std::string directoryPath = argv[1];
    std::vector<FileInfo> files;

    try {
        processDirectory(directoryPath, files);
        printTableHeader();
        for (const auto& file : files) {
            printFileInfo(file);
        }
        std::cout << std::string(100, '=') << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
