#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

/* This is skeleton code for reading characters from 
standard input (e.g., a file or console input) one by one until 
the end of the file (EOF) is reached. It keeps track of the current 
line number and is designed to be extended with additional 
functionality, such as processing or transforming the input data. 
In this specific task, the goal is to implement logic that removes 
C-style comments from the input. */

void code(char ch, int *state) {
  switch (ch) {
    case '/':
      *state = 1;
      break;
    case '"':
      putchar(ch);
      *state = 2;
      break;
    case '\'':
      putchar(ch);
      *state = 3;
      break;
    default:
      putchar(ch);
  }
  return;
}

void slash(char ch, int *state, int *line_com, int *line_cur) {
  if (ch == '/') {
    *state = 4;
    putchar(' ');
  }
  else if (ch == '*') {
    *state = 5;
    *line_com = *line_cur;
    putchar(' ');
  }
  else {
    *state = 0;
    putchar('/');
    code(ch, state);
  }
  return;
}

void comment(char ch, int *state) {
  if (ch == '\n') {
    *state = 0;
    putchar('\n');
  }
  return;
}

void multiline_comment(char ch, int *state) {
  if (ch == '\n') {
    putchar('\n');
  }
  else if (ch == '*') {
    *state = 6;
    return;
  }
  else {
    return;
  }
}

void asterisk(char ch, int *state) {
  if (ch == '/') {
    *state = 0;
    return;
  }
  else if (ch != '*') {
    *state = 5;
    multiline_comment(ch, state);
    return;
  }
}

int string(char ch, char delimiter, int *state, int *line_cur) {
  int ich;
  while (1) {
    if (ch == '\\') {
      putchar('\\');
      ich = getchar();
      if (ich == EOF) {
        return 1;
      }
      putchar((char)ich);
    }
    else {
      putchar(ch);
      if (ch == delimiter) {
        *state = 0;
        return 0;
      }
    }
    ich = getchar();
    if (ich == EOF) {
      return 1;
    }
    ch = (char)ich;
    if (ch == '\n') {
      (*line_cur)++;
    }
  }
}


int main(void)
{
  // ich: int type variable to store character input from getchar() (abbreviation of int character)
  int ich;
  // line_cur & line_com: current line number and comment line number (abbreviation of current line and comment line)
  int line_cur, line_com;
  // ch: character that comes from casting (char) on ich (abbreviation of character)
  char ch;
  int state;

  line_cur = 1;
  line_com = -1;
  state = 0;

  // This while loop reads all characters from standard input one by one
  while (1) {
    int got_eof = 0;

    ich = getchar();
    if (ich == EOF) {
      break;
    }

    ch = (char)ich;
    switch (state) {
      case 0:
        code(ch, &state);
        break;
      case 1:
        slash(ch, &state, &line_com, &line_cur);
        break;
      case 2:
        got_eof = string(ch, '"', &state, &line_cur);
        break;
      case 3:
        got_eof = string(ch, '\'', &state, &line_cur);
        break;
      case 4:
        comment(ch, &state);
        break;
      case 5:
        multiline_comment(ch, &state);
        break;
      case 6:
        asterisk(ch, &state);
        break;
    }

    if (ch == '\n')
      line_cur++;
    if (got_eof)
      break;
  }
  if (state < 5) {
    if (state == 1) {
      putchar('/');
    }
    return(EXIT_SUCCESS);
  }
  else {
    fprintf(stderr, "Error: line %i: unterminated comment\n", line_com);
    return (EXIT_FAILURE);
  }
}
