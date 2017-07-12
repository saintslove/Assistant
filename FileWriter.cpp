/*
 * FileWriter.cpp
 *
 *  Created on: 2017年6月21日
 *      Author: wong
 */

#include "FileWriter.h"
#include <stdio.h>
#include <stdarg.h>
#include "base/Logging.h"

FileWriter::FileWriter(const std::string& path)
    : m_path(path),
      m_yday(0),
      m_file(NULL) {
  if (m_path.at(m_path.length() - 1) != '/') {
    m_path.append("/");
  }
  TryRollFile();
}

FileWriter::~FileWriter() {
  if (m_file) {
    ::fclose(m_file);
    m_file = NULL;
  }
}

void FileWriter::TryRollFile() {
  time_t rawtime;
  struct tm* timeinfo;
  time(&rawtime);
  timeinfo = localtime(&rawtime);
  if (m_file == NULL || m_yday != timeinfo->tm_yday) {
    m_yday = timeinfo->tm_yday;
    if (m_file) {
      ::fclose(m_file);
      m_file = NULL;
    }
    strftime(m_timeStr, 11, "%Y-%m-%d", timeinfo);
    LOG_INFO << "RollFile " << m_path << " " << m_timeStr;
    m_file = ::fopen((m_path + m_timeStr).c_str(), "ae");
  }
}

void FileWriter::WriteLine(const char* buf) {
  TryRollFile();
  ::fwrite_unlocked(buf, 1, strlen(buf), m_file);
  ::fwrite_unlocked("\n", 1, 1, m_file);
}

void FileWriter::WriteLine(size_t count, ...)
{
  TryRollFile();
  va_list ptr;
  va_start(ptr, count);
  for (size_t i = 0; i < count; ++i) {
    const char* buf = va_arg(ptr, char*);
    ::fwrite_unlocked(buf, 1, strlen(buf), m_file);
  }
  va_end(ptr);
  ::fwrite_unlocked("\n", 1, 1, m_file);
}

void FileWriter::Flush() {
  ::fflush_unlocked(m_file);
}
