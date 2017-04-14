// Output Writer

#ifndef _OUTPUT_WRITER_H_
#define _OUTPUT_WRITER_H_

#include <string>

class OutputWriter {
 public:
  OutputWriter();
  virtual ~OutputWriter();
  virtual void writeLine(const std::string& line) = 0;
  virtual void writeLine(const char* format, ...);
};

#endif // _OUTPUT_WRITER_H_
