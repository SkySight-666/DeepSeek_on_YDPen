#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sqlite3.h>
#include <curl/curl.h>
#include <unistd.h>
#include <ctime>
#include <tuple>

// 获取当前时间戳字符串
std::string getCurrentTimestamp() {
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "[%Y-%m-%d %H:%M:%S] ", localTime);
    return std::string(buffer);
}

// 替换字符串中的换行符为空格
std::string replaceNewlines(const std::string& str) {
    std::string result = str;
    size_t pos = 0;
    while ((pos = result.find('\n', pos)) != std::string::npos) {
        result.replace(pos, 1, " ");
        pos += 1;
    }
    return result;
}

// 重启主程序函数
void kill_miniapp() {
    std::cout << getCurrentTimestamp() << "正在重启主程序..." << std::endl;
    system("TARGET=miniapp;PID_LIST=$(ps  | grep \"[^g]$TARGET\" | awk '{print $1}');for PID in $PID_LIST; do kill \"$PID\"; kill -9 \"$PID\";done");
    std::cout << getCurrentTimestamp() << "主程序重启完成。" << std::endl;
}

// 从配置文件中读取 API Key、检查间隔时间、模型类型、提示词和 API URL
std::tuple<std::string, int, std::string, std::string, std::string> readConfig() {
    std::ifstream configFile("dsProject.cfg");
    std::string apiKey;
    int interval = 5; // 默认间隔为 5 秒
    std::string model = "deepseek-chat"; // 默认模型
    std::string systemPrompt = "You are a helpful assistant."; // 默认提示词
    std::string apiUrl = "https://api.deepseek.com/chat/completions"; // 默认 API URL

    if (configFile.is_open()) {
        std::string line;
        int lineCount = 0;
        while (std::getline(configFile, line)) {
            // 去除行首尾的空白字符
            line.erase(0, line.find_first_not_of(" \t"));
            line.erase(line.find_last_not_of(" \t") + 1);

            if (!line.empty() && line.find("请在这里填写") == std::string::npos) {
                if (lineCount == 0) {
                    apiKey = line;
                } else if (lineCount == 1) {
                    try {
                        interval = std::stoi(line);
                    } catch (...) {
                        std::cerr << getCurrentTimestamp() << "读取间隔时间失败，使用默认值 5 秒。" << std::endl;
                    }
                } else if (lineCount == 2) {
                    model = line;
                } else if (lineCount == 3) {
                    systemPrompt = line;
                } else if (lineCount == 4) {
                    apiUrl = line;
                }
                lineCount++;
            }
        }
        configFile.close();
        std::cout << getCurrentTimestamp() << "成功从配置文件读取 API Key、检查间隔时间、模型类型、提示词和 API URL。" << std::endl;
    } else {
        std::ofstream newConfig("dsProject.cfg");
        if (newConfig.is_open()) {
            newConfig << "请在这里填写 DeepSeek API Key\n5\ndeepseek-chat\nYou are a helpful assistant.\nhttps://api.deepseek.com/chat/completions";
            newConfig.close();
            std::cout << getCurrentTimestamp() << "dsProject.cfg 文件已创建，请在文件中填写 DeepSeek API Key、检查间隔时间（秒）、模型类型、提示词和 API URL。" << std::endl;
        } else {
            std::cerr << getCurrentTimestamp() << "无法创建 dsProject.cfg 文件。" << std::endl;
        }
    }
    return {apiKey, interval, model, systemPrompt, apiUrl};
}

// 回调函数，用于获取包含 wordbook 的表名
static int getTableNameCallback(void *data, int argc, char **argv, char **azColName) {
    std::vector<std::string> *tableNames = static_cast<std::vector<std::string>*>(data);
    for (int i = 0; i < argc; ++i) {
        std::string tableName = argv[i];
        if (tableName.find("wordbook") != std::string::npos && tableName != "table_wordbook_InvalidId") {
            tableNames->push_back(tableName);
        }
    }
    return 0;
}

// 回调函数，用于处理查询结果
static int queryCallback(void *data, int argc, char **argv, char **azColName) {
    std::vector<std::string> *words = static_cast<std::vector<std::string>*>(data);
    for (int i = 0; i < argc; ++i) {
        if (std::string(azColName[i]) == "word") {
            words->push_back(argv[i] ? argv[i] : "");
        }
    }
    return 0;
}

// 解析 JSON 中的 content 字段
std::string parseJsonContent(const std::string& json) {
    size_t startPos = json.find("\"content\":\"");
    if (startPos != std::string::npos) {
        startPos += 11;
        size_t endPos = json.find("\"", startPos);
        if (endPos != std::string::npos) {
            return json.substr(startPos, endPos - startPos);
        }
    }
    return "";
}

// cURL 写入回调函数
size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *s) {
    size_t newLength = size * nmemb;
    try {
        s->append((char*)contents, newLength);
    } catch(std::bad_alloc &e) {
        return 0;
    }
    return newLength;
}

// 向 API 发送请求
std::string sendRequest(const std::string& apiKey, const std::string& message, const std::string& model, const std::string& systemPrompt, const std::string& apiUrl) {
    std::cout << getCurrentTimestamp() << "正在向 API 发送请求，查询内容: " << message << std::endl;
    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    std::string requestBody = "{\"model\": \"" + model + "\", \"messages\": [{\"role\": \"system\", \"content\": \"" + systemPrompt + "\"}, {\"role\": \"user\", \"content\": \"" + message + "\"}], \"stream\": false}";

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, apiUrl.c_str());
        // --- 开始添加：这部分就相当于命令行中的 -k 或 --insecure ---
        // 1. 禁用对等方（服务器）证书的验证
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        // 2. 禁用主机名验证
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        // --- 结束添加 ---
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        std::string authHeader = "Authorization: Bearer " + apiKey;
        headers = curl_slist_append(headers, authHeader.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, requestBody.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            std::cerr << getCurrentTimestamp() << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
    std::cout << getCurrentTimestamp() << "API 请求完成。" << std::endl;
    return readBuffer;
}

// 更新数据库中的 word 字段
void updateWordInDB(sqlite3 *db, const std::string& tableName, const std::string& oldWord, const std::string& newWord) {
    std::string replacedNewWord = replaceNewlines(newWord);
    std::string replacedOldWord = replaceNewlines(oldWord);
    std::cout << getCurrentTimestamp() << "正在更新数据库中的 word 字段，旧值: " << replacedOldWord << ", 新值: " << replacedNewWord << std::endl;
    std::string updateQuery = "UPDATE " + tableName + " SET word = '" + replacedNewWord + "' WHERE word = '" + replacedOldWord + "';";
    char *zErrMsg = 0;
    int rc = sqlite3_exec(db, updateQuery.c_str(), 0, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        std::cerr << getCurrentTimestamp() << "SQL 错误: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    } else {
        std::cout << getCurrentTimestamp() << "数据库更新成功。" << std::endl;
    }
}

int main() {
    sqlite3 *db;
    int rc = sqlite3_open("/userdisk/database/wordbook.db", &db);
    if (rc) {
        std::cerr << getCurrentTimestamp() << "无法打开数据库: " << sqlite3_errmsg(db) << std::endl;
        return(0);
    } else {
        std::cout << getCurrentTimestamp() << "成功打开数据库" << std::endl;
    }

    auto [apiKey, interval, model, systemPrompt, apiUrl] = readConfig();
    if (apiKey.empty() || apiKey.find("请在这里填写") != std::string::npos) {
        std::cout << getCurrentTimestamp() << "请在 dsProject.cfg 文件中填写有效的 API Key。" << std::endl;
        sqlite3_close(db);
        return 1;
    }

    bool needRestart = false;

    while (true) {
        // 获取表名
        std::vector<std::string> tableNames;
        std::string sql = "SELECT name FROM sqlite_master WHERE type='table';";
        std::cout << getCurrentTimestamp() << "正在查询数据库中的表名..." << std::endl;
        rc = sqlite3_exec(db, sql.c_str(), getTableNameCallback, &tableNames, NULL);
        if (rc != SQLITE_OK) {
            std::cerr << getCurrentTimestamp() << "SQL 错误: " << sqlite3_errmsg(db) << std::endl;
            sleep(interval);
            continue;
        }

        if (tableNames.empty()) {
            std::cout << getCurrentTimestamp() << "未找到合适的表。" << std::endl;
            sleep(interval);
            continue;
        }

        std::string tableName = tableNames[0];
        std::cout << getCurrentTimestamp() << "使用表: " << tableName << std::endl;

        // 查询所有 word 字段
        std::vector<std::string> words;
        sql = "SELECT word FROM " + tableName + ";";
        std::cout << getCurrentTimestamp() << "正在查询表中的 word 字段..." << std::endl;
        rc = sqlite3_exec(db, sql.c_str(), queryCallback, &words, NULL);
        if (rc != SQLITE_OK) {
            std::cerr << getCurrentTimestamp() << "SQL 错误: " << sqlite3_errmsg(db) << std::endl;
            sleep(interval);
            continue;
        }

        bool allSuccess = true;
        needRestart = false;
        for (const auto& word : words) {
            size_t pos = word.find("@ds");
            if (pos != std::string::npos) {
                std::string query = word.substr(0, pos);
                std::cout << getCurrentTimestamp() << "找到包含 @ds 的单词，查询内容: " << query << std::endl;

                std::string response = sendRequest(apiKey, query, model, systemPrompt, apiUrl);
                std::string content = parseJsonContent(response);
                if (!content.empty()) {
                    std::string replacedContent = replaceNewlines(content);
                    std::cout << getCurrentTimestamp() << "API 响应内容: " << replacedContent << std::endl;
                    updateWordInDB(db, tableName, word, replacedContent);
                    needRestart = true;
                } else {
                    std::cerr << getCurrentTimestamp() << "解析 API 响应失败。" << std::endl;
                    allSuccess = false;
                }
            }
        }

        if (allSuccess && needRestart) {
            kill_miniapp();
            // 等待一段时间，让主程序有时间重启
            sleep(5); 
            // 重新打开数据库
            sqlite3_close(db);
            rc = sqlite3_open("/userdisk/database/wordbook.db", &db);
            if (rc) {
                std::cerr << getCurrentTimestamp() << "无法重新打开数据库: " << sqlite3_errmsg(db) << std::endl;
                return(0);
            } else {
                std::cout << getCurrentTimestamp() << "成功重新打开数据库" << std::endl;
            }
        }

        sleep(interval);
    }

    sqlite3_close(db);
    return 0;
}
