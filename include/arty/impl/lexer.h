#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <memory>
#include <vector>

#include <arty/core/token.h>

namespace arty {

enum FsmState {
  Acc,
  Ong,
  Rej,
};

class BaseFsm {
 protected:
  FsmState _state;
  std::size_t _pos;

 public:
  BaseFsm();
  virtual ~BaseFsm();
  virtual FsmState eat(char c, std::size_t pos);
  virtual Token token();
};

class SingleCharFsm : public BaseFsm {
 private:
  char _char;
  TokenType _type;

 public:
  SingleCharFsm(char c, TokenType t) : _char(c), _type(t) {}
  ~SingleCharFsm() = default;

  FsmState eat(char c, std::size_t pos);
  Token token();
};

class WordFsm : public BaseFsm {
 private:
  std::string _word;
  TokenType _type;
  size_t _index;

 public:
  WordFsm(std::string w, TokenType t) : _word(w), _type(t), _index(0) {}
  ~WordFsm() = default;

  FsmState eat(char c, std::size_t pos);
  Token token();
};

class ErrorFsm : public BaseFsm {
 private:
  std::string _word;

 public:
  FsmState eat(char c, std::size_t pos);
  Token token();
};

class NumberFsm : public BaseFsm {
 private:
  std::string _word;

 public:
  FsmState eat(char c, std::size_t pos);
  Token token();
};

class CmdFsm : public BaseFsm {
 private:
  std::string _cmd;

 public:
  FsmState eat(char c, std::size_t pos);
  Token token();
};

class Lexer {
 public:
  typedef std::shared_ptr<Lexer> Ptr;

  Lexer(std::string& input);
  Token next();

  friend std::ostream& operator<<(std::ostream& out, Lexer const& l);

 private:
  void process();
  void reset();

  std::string _input;
  size_t _pos;
  size_t _last_token_pos;
  std::vector<std::unique_ptr<BaseFsm>> _automatas;
  std::vector<Token> _tokens;
};

std::ostream& operator<<(std::ostream& out, Lexer const& l);

}  // namespace arty

#endif  // LEXER_H
