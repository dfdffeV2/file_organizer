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

std::string lower(std::string s) {
    std::string newS;
    for (char ch : s) {
        newS.push_back(std::tolower(ch));
    }
    return newS;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: organizer.exe <directory>\n";
        return 1;
    }

    bool dryRun = false;
    for (int i = 2; i < argc; i++) {
        if (std::string(argv[i]) == "--dry-run") {
            dryRun = true;
        }
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

    if (dryRun) std::cout << "[DRY RUN]\n";
    
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (!entry.is_regular_file()) {
            continue;
        }

        std::string extension = entry.path().extension().string();
        auto it = fileCategories.find(lower(extension));
        if (it != fileCategories.end()) {
            if (!dryRun) {
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
            }

            std::cout << "Moved: " << entry.path().filename() << " -> " << it->second << '\n';
            count++;

        } else {
            std::cout << entry.path().filename() << " -> Unknown\n";
            unknown++;
        }
    }

    if (!dryRun) {
        std::cout << "Moved files: " << count << '\n'
            << "Unknown files: " << unknown << '\n'
            << "Errors: " << errors << '\n'; 
    }

    return 0;
}