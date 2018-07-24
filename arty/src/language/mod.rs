use std::fmt;

#[derive(Clone, PartialEq, Debug)]
pub enum Token {
    Eof,
    Error(String),
    Number(String),
    String(String),
    Identifier(String),
    // Math operations
    Plus,
    Minus,
    Times,
    Divide,
    Modulo,
    // CTRL OP
    Sequencial,
    Background,
}

impl Token {
    pub fn precedence(token: Token) -> u32 {
        return match token {
            Token::Eof => 0,
            Token::Error(_str) => 0,
            // Math OP
            Token::Plus => 100,
            Token::Minus => 100,
            Token::Times => 101,
            Token::Divide => 101,
            Token::Modulo => 102,
            _ => 0,
        }
    }

    pub fn from(token: Token) -> String {
        return match token {
            Token::Error(str) => str,
            Token::Number(str) => str,
            Token::String(str) => str,
            Token::Identifier(str) => str,
            _ => String::new(),
        }
    }
}

impl fmt::Display for Token {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        match *self {
            Token::Eof => write!(f, "eof"),
            Token::Error(ref str) => write!(f, "error({})", str),
            Token::Number(ref str) => write!(f, "number({})", str),
            Token::String(ref str) => write!(f, "string({})", str),
            Token::Identifier(ref str) => write!(f, "identifier{})", str),
            Token::Plus => write!(f, "+"),
            Token::Minus => write!(f, "-"),
            _ => write!(f, "na"),
        }
    }
}

use std::ops;

pub struct Number {
    data: f64, //TODO(Guillaume): implement something great
}

impl Number {
    pub fn from(string: String) -> Number {
        return Number {
            data: string.parse().unwrap(),
        }
    }

    pub fn to(&self) -> String {
        return self.data.to_string()
    }
}

impl ops::Add<Number> for Number {
    type Output = Number;

    fn add(self, rhs: Number) -> Number {
        return Number {
            data: self.data + rhs.data,
        }
    }
}

impl ops::Mul<Number> for Number {
    type Output = Number;

    fn mul(self, rhs: Number) -> Number {
        return Number {
            data: self.data * rhs.data,
        }
    }
}