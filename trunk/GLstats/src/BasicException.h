#ifndef BASICEXCEPTION_H
#define BASICEXCEPTION_H

#include <string>
#include <exception>

class BasicException : public std::exception {
public:
  BasicException(const std::string &message)
    : m_message(message) {}
  virtual ~BasicException() throw() {}

  virtual const char* what()
  {
    return m_message.c_str();
  }

private:
  std::string m_message;
};

#endif
