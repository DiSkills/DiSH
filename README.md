# DiSH is a simple command interpreter

## TODO
- [ ] Lexer quotes mode
- [ ] Parser
- [ ] Executor

## Lexer
### Token table
| Token | Lexeme |
| --- | --- |
| BACKGROUND | & |
| SEQUENCE | ; |
| INPUT_REDIRECTION | < |
| OUTPUT_REDIRECTION | > |
| APPEND_REDIRECTION | >> |
| PIPE | \| |
| LBRACKET | ( |
| RBRACKET | ) |
| AND | && |
| OR | \|\| |
| WORD | everything else |

## Parser

### Priority table
| Priority | Operator |
| --- | --- |
| 0 | () |
| 1 | \| |
| 2 | < > >> |
| 3 | && \|\| |
| 4 | ; & |

### Grammar
    COMMAND_LINE -> SEQUENCE
              | SEQUENCE ;
              | SEQUENCE &
    SEQUENCE -> SEQUENCE ; ANDOR
              | SEQUENCE & ANDOR
              | ANDOR
    ANDOR -> ANDOR && COMMAND
           | ANDOR || COMMAND
           | COMMAND
    COMMAND -> PIPELINE INPUT OUTPUT
    INPUT -> < word
           | e
    OUTPUT -> > word
            | >> word
            | e
    PIPELINE -> PIPELINE | SIMPLE_COMMAND
              | SIMPLE_COMMAND
    SIMPLE_COMMAND -> (SEQUENCE)
                    | word ARGS
    ARGS -> ARGS word
          | e
