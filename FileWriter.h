/*
 * FileWriter.h
 *
 *  Created on: 2017年6月21日
 *      Author: wong
 */

#ifndef FILEWRITER_H_
#define FILEWRITER_H_

#include <string>

class FileWriter {
 public:
  FileWriter(const std::string& path);
  virtual ~FileWriter();

 public:
  void WriteLine(const char* buf);
  // CATION: 可变参数必须均为char*类型
  void WriteLine(size_t count, ...);
  void Flush();

 private:
  inline void TryRollFile();

 private:
  std::string m_path;
  int m_yday;
  char m_timeStr[11];
  FILE* m_file;
};

#endif /* FILEWRITER_H_ */
