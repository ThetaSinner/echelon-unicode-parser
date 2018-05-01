grammar echelon;

/*
 * Parser rules
 */

number: INTEGER;

/*
 * Lexer rules
 */

fragment DIGIT: [0-9]+;

INTEGER: DIGIT;
