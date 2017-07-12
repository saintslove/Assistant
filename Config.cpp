#include "Config.h"

#include <stdio.h>
#include <stdlib.h>

#include "base/Logging.h"

Config* Config::m_instance = NULL;

int Config::Init(const std::string& path)
{
    FILE * fp;
    fp = fopen(path.c_str(), "r");
    if (fp == NULL)
    {
        LOG_ERROR << "Config::Init() fopen failed! path = " << path;
        return -1;
    }

    LOG_INFO << ">>>>>>>>>>>>>>>Config>>>>>>>>>>>>>>>";
    char line[1024] = {0};
    char key[512] = {0};
    char value[512] = {0};
    while (fgets(line, 1024, fp))
    {
        if (line[0] == '#' || strcmp(line, "") == 0)
        {
            continue;
        }
        key[0] = 0;
        value[0] = 0;
        sscanf(line, "%s %s", key, value);
        if (strcmp(key, "") == 0 || strcmp(value, "") == 0)
        {
            continue;
        }
        m_mapConf[key] = value;
        LOG_INFO << "  > " << key << " = " << value;
    }
    LOG_INFO << "<<<<<<<<<<<<<<<Config<<<<<<<<<<<<<<<";

    fclose(fp);
    return 0;
}

int Config::GetInt(const std::string& key, int defaultValue/* = 0*/)
{
    MapConf::iterator it = m_mapConf.find(key);
    return (it == m_mapConf.end()) ? defaultValue : atoi(it->second.c_str());
}

const std::string& Config::GetStr(const std::string& key, const std::string& defaultValue/* = ""*/)
{
    MapConf::iterator it = m_mapConf.find(key);
    return (it == m_mapConf.end()) ? defaultValue : it->second;
}

bool Config::IsNumber(const char* str)
{
    if (strlen(str) == 0 || str[0] < '0')
    {
        return false;
    }
    for (size_t i = 0; i < strlen(str); ++i)
    {
        if (str[i] < '0' || str[i] > '9')
        {
            return false;
        }
    }
    return true;
}
