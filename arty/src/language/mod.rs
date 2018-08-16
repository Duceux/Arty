use std::fmt;

use std::result;
use std::error::Error;

type Result<T> = result::Result<T, Box<Error>>;

#[derive(Clone, PartialEq, Debug)]
pub enum Token {
    None,
    Eof,
    Error(String),
    Number(String),
    String(String),
    // Math operations
    Plus,
    Minus,
    Times,
    Divide,
    Modulo,
    ParO,
    ParC,
    // CTRL OP
    Sequencial,
    Background,
    // Shell
    Cmd(String),
    Opts(String),
    Args(String),
    ChangeDir,
    Path(String),
}

impl Token {
    pub fn description(&self) -> String {
        return String::from(match *self {
            Token::Eof => "Eof",
            Token::Error(ref _str) => "Error",
            Token::Number(ref _str) => "Number",
            Token::String(ref _str) => "String",
            Token::Cmd(ref _str) => "Cmd",
            Token::Opts(ref _str) => "Opts",
            Token::Args(ref _str) => "Args",
            Token::Path(ref _str) => "Path",
            Token::Plus => "Plus",
            Token::Minus => "Minus",
            Token::Times => "Times",
            Token::Divide => "Divide",
            Token::ParO => "ParO",
            Token::ParC => "ParC",
            Token::ChangeDir => "ChangeDir",
            _ => "None",
        })
    }

    pub fn rprec(&self) -> u32 {
        return match *self {
            Token::Eof => 0,
            Token::Error(ref _str) => 0,
            // Math OP
            Token::Plus => 100,
            Token::Minus => 100,
            Token::Times => 101,
            Token::Divide => 101,
            Token::Modulo => 102,
            Token::Number(ref _str) => 1000,
            // Groups
            Token::ParO => 0,
            Token::ParC => 0,
            // shell
            Token::Cmd(ref _str) => 500,
            Token::Opts(ref _str) => 1000,
            Token::Args(ref _str) => 1000,
            Token::ChangeDir => 500,
            Token::Path(ref _str) => 500,
            _ => 1,
        }
    }


    pub fn lprec(&self) -> u32 {
        return match *self {
            Token::Minus => 1000,
            _ => self.rprec(),
        }
    }


    pub fn as_string(&self) -> String {
        return match *self {
            Token::Error(ref str) => str.clone(),
            Token::Number(ref str) => str.clone(),
            Token::String(ref str) => str.clone(),
            Token::Cmd(ref str) => str.clone(),
            Token::Args(ref str) => str.clone(),
            Token::Opts(ref str) => str.clone(),
            Token::Path(ref str) => str.clone(),
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
            Token::Cmd(ref str) => write!(f, "cmd({})", str),
            Token::Opts(ref str) => write!(f, "opts({})", str),
            Token::Args(ref str) => write!(f, "args({})", str),
            Token::Path(ref str) => write!(f, "path({})", str),
            Token::Plus => write!(f, "+"),
            Token::Minus => write!(f, "-"),
            Token::Times => write!(f, "*"),
            Token::Divide => write!(f, "/"),
            Token::ParO => write!(f, "("),
            Token::ParC => write!(f, ")"),
            Token::ChangeDir => write!(f, "cd"),
            _ => write!(f, "none"),
        }
    }
}

use std::ops;

pub struct Number {
    data: f64, //TODO(Guillaume): implement something great
}

impl Number {
    pub fn from_string(string: String) -> Result<Self> {
        let tmp = string.parse()?;
        return Ok(Number {
            data: tmp,
        })
    }

    pub fn from_token(token: Token) -> Result<Self> {
        if let Token::Number(str) = token {
            return Ok(Number {
                data: str.parse()?,
            })
        }
        return Err(From::from("No a number".to_string()))
    }

    pub fn as_string(&self) -> String {
        return self.data.to_string()
    }

    pub fn as_token(&self) -> Token {
        return Token::Number(self.data.to_string())
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

impl ops::Sub<Number> for Number {
    type Output = Number;

    fn sub(self, rhs: Number) -> Number {
        return Number {
            data: self.data - rhs.data,
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

impl ops::Div<Number> for Number {
    type Output = Number;

    fn div(self, rhs: Number) -> Number {
        return Number {
            data: self.data / rhs.data,
        }
    }
}
