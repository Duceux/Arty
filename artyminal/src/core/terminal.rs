pub trait Terminal {
    fn clear(&self);
    fn clear_guesses(&self);
    fn write_prompt(&mut self, line: String);
    fn write_line(&self, line: String);
    fn write_guesses(&self, line: Vec<String>);
    fn write_new_line(&self);
    fn write_cursor(&mut self, pos: usize);
}

pub enum Key {
    Char(char),
    Up,
    Left,
    Down,
    Right,
    Ctrl(char),
    Alt(char),
    Esc,
    Backspace,
    Delete,
    Tab,
    Enter,
}

pub trait Keyboard {
    fn get_key(&self) -> Key;
}
