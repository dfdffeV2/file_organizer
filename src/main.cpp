#include <iostream>
#include <filesystem>
#include <unordered_map>

namespace fs = std::filesystem;

std::unordered_map<std::string, std::string> fileCategories {
    {".png", "Images"},
    {".jpg", "Images"},
    {".cpp", "Code"},
    {".mp3", "Music"},
    {".exe", "Executable"}
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: organizer.exe <directory>\n";
        return 1;
    }

    fs::path directory = argv[1];

    if (!fs::exists(directory)) {
        std::cerr << "Directory not found\n";
        return 1;
    }

    if (!fs::is_directory(directory)) {
        std::cerr << "Path is not a directory\n";
        return 1;
    }

    // --- Sort ---
    int count{};
    int errors{};
    int unknown{};
    
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (!entry.is_regular_file()) {
            continue;
        }

        auto it = fileCategories.find(entry.path().extension().string());
        if (it != fileCategories.end()) {
            try {
                auto destinationPath = directory / it->second;
                if (!fs::exists(destinationPath)) {
                    fs::create_directory(destinationPath);
                }

                fs::rename(entry.path(), destinationPath / entry.path().filename());
            }
            catch (const fs::filesystem_error& e) {
                std::cerr << "Filesystem error: " << e.what() << '\n';
                std::cerr << "Source path: " << e.path1() << '\n';
                std::cerr << "Destination path: " << e.path2() << '\n';
                errors++;
                continue;
            }

            std::cout << "Moved: " << entry.path().filename() << " -> " << it->second << '\n';
            count++;

        } else {
            std::cout << entry.path().filename() << " -> Unknown\n";
            unknown++;
        }
    }

    std::cout << "Moved files: " << count << '\n'
        << "Unknown files: " << unknown << '\n'
        << "Errors: " << errors << '\n'; 

    return 0;
}