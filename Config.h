#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <map>

#include "Common.h"

class Config
{
public:
    static Config* GetInstance()
    {
        if (m_instance == NULL)
        {
            m_instance = new Config();
        }
        return m_instance;
    }
    void Destroy()
    {
        SAFE_DELETE(m_instance);
    }
    int Init(const std::string& path);
    int GetInt(const std::string& key, int defaultValue = 0);
    const std::string& GetStr(const std::string& key, const std::string& defaultValue = "");

public:
    static bool IsNumber(const char* str);

private:
    typedef std::map<std::string, std::string> MapConf;
    MapConf m_mapConf;

private:
    Config() {}
    static Config* m_instance;
};

#define CONFIG Config::GetInstance()
#define CFGINT(key) CONFIG->GetInt(key)
#define CFGSTR(key) CONFIG->GetStr(key)

#endif /* CONFIG_H */
