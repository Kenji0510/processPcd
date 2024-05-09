#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

std::vector<std::string> getFilesInDirectory(std::string directoryPath) {
    std::vector<std::string> filePaths;

    // Check if directory exists
    for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
        auto filePath = entry.path();

        // Check if file is pcd
        if (filePath.extension() == ".pcd") {
            filePaths.push_back(filePath.string());
        }
    }
    return filePaths;
}

std::string getNewFileName(const std::string& originalFileName) {
    // 検索する文字列
    std::string searchString1 = "saved_cloud_";
    std::string searchString2 = "_20240430";
    
    // 検索する文字列が見つかった位置を取得
    size_t pos1 = originalFileName.find(searchString1);
    size_t pos2 = originalFileName.find(searchString2);

    // それぞれの文字列が見つかった場合
    if (pos1 != std::string::npos && pos2 != std::string::npos) {
        // `_20240430`より前の部分と、`saved_cloud_pub_`より後の部分を結合して新しいファイル名を生成
        std::string extractedName = originalFileName.substr(pos1 + searchString1.length(), pos2 - pos1 - searchString1.length());
        return extractedName;
    } else {
        // いずれかの文字列が見つからない場合はそのままのファイル名を返す
        return originalFileName;
    }
}

int main() {
    std::string pcdDirectoryPath = "/home/kenji/workspace/resarch/pcdFile/originalData";
    // Get Pcd Files
    auto pcdFiles = getFilesInDirectory(pcdDirectoryPath);

    for (const auto& path : pcdFiles) {
        std::string newFileName = getNewFileName(path);
        std::cout << "New File Name: " << newFileName << std::endl;
    }
    
    return 0;
}
