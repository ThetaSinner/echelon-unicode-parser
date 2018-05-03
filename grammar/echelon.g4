grammar Echelon;

/*
 * Parser rules
 */

root: INTEGER;

number: INTEGER;

/*
 * Lexer rules
 */

fragment DIGIT: [0-9]+;

INTEGER: DIGIT;

SPACE: ' ' -> skip;
