# DiSH is a simple command interpreter
![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-064F8C?style=for-the-badge&logo=cmake&logoColor=white)
![GitHub-Actions](https://img.shields.io/badge/Github%20Actions-282a2e?style=for-the-badge&logo=githubactions&logoColor=367cfe)

## Tasks
- [x] Lexer
    - [x] Recognize tokens
        - [x] UnitTests
    - [ ] Quotes mode
- [ ] Parser
    - [ ] Simple command
        - [ ] UnitTests
    - [ ] Pipeline
        - [ ] UnitTests
- [ ] Executor
    - [ ] Simple command
        - [ ] Python tests
    - [ ] Pipeline
        - [ ] Python tests

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

### BNF
    START ::= PIPELINE "\n"
    PIPELINE ::= SIMPLE_COMMAND ("|" SIMPLE_COMMAND)*
    SIMPLE_COMMAND ::= word (word)*
